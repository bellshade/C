
#include "nn.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int
main(void)
{
  srand(time(NULL));

  printf("=============[ NEURAL NETWORK - IRIS CLASSIFICATION ]============\n");

  // Inisialisasi arena memory
  struct MemoryArena arena = arena_create(1024 * 1024 * 10); // 10MB

  // Load dataset iris
  printf("・ Loading dataset iris.csv...\n");
  struct Matrix dataset = dataset_load_from_csv(&arena, "iris.csv", 1); // Skip header
  printf("・ Dataset loaded: %zu samples, %zu features\n", dataset.num_rows, dataset.num_columns);

  // Normalisasi data input (4 kolom pertama)
  printf("・ Normalizing input features...\n");
  matrix_normalize_minmax(dataset, 4, 0.0f, 1.0f);

  // Shuffle dataset
  printf("・ Shuffling dataset...\n");
  matrix_shuffle_rows(dataset);

  // Split dataset: 80% training, 20% testing
  size_t train_size = (size_t)(dataset.num_rows * 0.8f);
  size_t test_size = dataset.num_rows - train_size;

  struct Matrix train_data = matrix_create_row_slice(dataset, 0, train_size);
  struct Matrix test_data = matrix_create_row_slice(dataset, train_size, test_size);

  printf("-- Training set: %zu samples\n", train_data.num_rows);
  printf("-- Test set: %zu samples\n", test_data.num_rows);

  // Definisi arsitektur neural network
  // Input: 4 features (sepal length, sepal width, petal length, petal width)
  // Hidden: 8 neurons
  // Output: 3 classes (setosa, versicolor, virginica)
  size_t arch[] = {4, 8, 3};
  size_t arch_count = sizeof(arch) / sizeof(arch[0]);

  printf("\n・ Creating neural network with architecture: ");
  for (size_t i = 0; i < arch_count; ++i) {
    printf("%zu", arch[i]);
    if (i < arch_count - 1)
      printf(" - ");
  }
  printf("\n");

  // Alokasi neural network
  struct NeuralNetwork nn = neural_network_allocate(&arena, arch, arch_count);

  // Initialize weights dengan nilai random
  printf("・ Initializing random weights...\n");
  neural_network_randomize_weights(nn, -1.0f, 1.0f);

  // Test sebelum training
  printf("\n・ Before training:\n");
  printf("-- Training accuracy: %.2f%%\n", 100.0f * neural_network_calculate_accuracy(nn, train_data));
  printf("-- Test accuracy: %.2f%%\n", 100.0f * neural_network_calculate_accuracy(nn, test_data));
  printf("-- Training cost: %.4f\n", neural_network_calculate_accuracy(nn, train_data));

  // Training parameters
  size_t epochs = 1000;
  size_t batch_size = 32;
  float learning_rate = 0.1f;

  printf("\n・ Training parameters:\n");
  printf("-- Epochs: %zu\n", epochs);
  printf("-- Batch size: %zu\n", batch_size);
  printf("-- Learning rate: %.3f\n", learning_rate);
  printf("\n======================[ STARTING TRAINING ]======================\n");

  // Create separate arena for temporary calculations during training
  struct MemoryArena temp_arena =
      arena_create(1024 * 1024 * 5); // 5MB for temporary calculations

  // Training loop
  for (size_t epoch = 0; epoch < epochs; ++epoch) {
    // Shuffle training data setiap epoch
    matrix_shuffle_rows(train_data);

    struct BatchProcessor batch = {0};
    while (!batch.is_epoch_finished) {
      batch_process_training_data(&temp_arena, &batch, batch_size, nn, train_data, learning_rate);
      arena_reset(&temp_arena); // Only reset temporary arena
    }

    // Print progress setiap 100 epoch
    if ((epoch + 1) % 100 == 0 || epoch == 0 || epoch == epochs - 1) {
      float train_acc = neural_network_calculate_accuracy(nn, train_data);
      float test_acc = neural_network_calculate_accuracy(nn, test_data);
      float cost = neural_network_calculate_cost(nn, train_data);

      printf("Epoch %4zu | Cost: %.4f | Train Acc: %.2f%% | Test Acc: %.2f%%\n",
             epoch + 1, cost, 100.0f * train_acc, 100.0f * test_acc);
    }
  }

  printf("======================[ TRAINING COMPLETED ]=====================\n\n");

  // Final evaluation
  printf("・ Final Results\n");
  float final_train_acc = neural_network_calculate_accuracy(nn, train_data);
  float final_test_acc = neural_network_calculate_accuracy(nn, test_data);
  float final_cost = neural_network_calculate_cost(nn, train_data);

  printf("-- Final training accuracy: %.2f%%\n", 100.0f * final_train_acc);
  printf("-- Final test accuracy: %.2f%%\n", 100.0f * final_test_acc);
  printf("-- Final training cost: %.4f\n", final_cost);

  // Demo prediksi dengan beberapa sample dari test set
  printf("\n======================[ SAMPLE PREDICTIONS ]=====================\n");
  printf("・ Actual -> Predicted (Confidence)\n");
  printf("・ 0 = Setosa | 1 = Versicolor | 2 = Virginica\n");
  printf("-----------------------------------------------------------------\n");

  for (size_t i = 0; i < 10 && i < test_data.num_rows; ++i) {
    struct Row sample = matrix_get_row(test_data, i);
    struct Row input = row_create_slice(sample, 0, 4);
    struct Row target = row_create_slice(sample, 4, 3);

    // Copy input ke network
    row_copy_data(nn.activation_vectors[0], input);
    neural_network_forward_pass(nn);

    size_t actual = row_find_max_index(target);
    size_t predicted = row_find_max_index(nn.activation_vectors[nn.total_layers - 1]);
    float confidence = row_at(nn.activation_vectors[nn.total_layers - 1], predicted);

    printf("   %zu    ->    %zu     (%.3f) %s\n", actual, predicted, confidence,
           (actual == predicted) ? "✓" : "✗");
  }

  printf("\n・ Classification Complete\n");

  return 0;
}

/* vim: set ts=4 sw=4 sts=4 et */


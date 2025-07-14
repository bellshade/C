
/**
 * @file nn.c
 * @brief Implementasi Neural Network sederhana untuk klasifikasi
 * @author 0xfa99
 * @version 1.0
 *
 * File ini berisi implementasi dari semua fungsi yang dideklarasikan di nn.h.
 * Termasuk operasi aktivasi, manajemen memori arena, operasi matrix,
 * neural network, dan dataset loading.
 */

#include "nn.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// ==================[ ACTIVATION FUNCTIONS - IMPLEMENTATION ]==================

/**
 * @brief Implementasi fungsi sigmoid
 * @param input_value Nilai input yang akan diproses
 * @return Hasil sigmoid dalam range 0-1
 */
float
activation_sigmoid(float input_value)
{
    return 1.0f / (1.0f + expf(-input_value));
}

/**
 * @brief Implementasi fungsi ReLU (Rectified Linear Unit)
 * @param input_value Nilai input yang akan diproses
 * @return 0 jika input <= 0, input_value jika input > 0
 */
float
activation_relu(float input_value)
{
    return input_value > 0.0f ? input_value : 0.0f;
}

/**
 * @brief Implementasi fungsi hyperbolic tangent
 * @param input_value Nilai input yang akan diproses
 * @return Hasil tanh dalam range -1 sampai 1
 */
float
activation_tanh(float input_value)
{
    float exp_positive = expf(input_value);
    float exp_negative = expf(-input_value);
    return (exp_positive - exp_negative) / (exp_positive + exp_negative);
}

/**
 * @brief Menerapkan fungsi aktivasi berdasarkan tipe yang dipilih
 * @param input_value Nilai input yang akan diproses
 * @param activation_type Tipe aktivasi yang akan diterapkan
 * @return Hasil aktivasi sesuai tipe yang dipilih
 */
float
activation_apply(float input_value, enum ActivationType activation_type)
{
    switch (activation_type) {
        case ACTIVATION_SIGMOID: return activation_sigmoid(input_value);
        case ACTIVATION_RELU: return activation_relu(input_value);
        case ACTIVATION_TANH: return activation_tanh(input_value);
        case ACTIVATION_NONE: return input_value;
    }

    assert(0 && "Unknown activation type");
    return 0.0f;
}

/**
 * @brief Menghitung turunan fungsi aktivasi untuk backpropagation
 * @param activated_value Nilai yang sudah melalui fungsi aktivasi
 * @param activation_type Tipe aktivasi yang digunakan
 * @return Nilai turunan fungsi aktivasi
 */
float
activation_compute_derivative(float activated_value, enum ActivationType activation_type)
{
    switch (activation_type) {
        case ACTIVATION_SIGMOID: return activated_value * (1.0f - activated_value);
        case ACTIVATION_RELU: return activated_value >= 0.0f ? 1.0f : 0.0f;
        case ACTIVATION_TANH: return 1.0f - activated_value * activated_value;
        case ACTIVATION_NONE: return 1.0f;
    }

    assert(0 && "Unknown activation type");
    return 0.0f;
}

// ====================[ MEMORY MANAGEMENT - IMPLEMENTATION ]===================

/**
 * @brief Membuat arena baru untuk manajemen memori
 * @param size_in_bytes Ukuran arena yang diinginkan dalam bytes
 * @return Struktur MemoryArena yang siap digunakan
 */
struct MemoryArena
arena_create(size_t size_in_bytes)
{
    struct MemoryArena arena = {0};
    size_t buffer_element_size = sizeof(*arena.memory_buffer);
    size_t total_capacity = (size_in_bytes + buffer_element_size - 1) / buffer_element_size;
    uintptr_t *memory_buffer = (uintptr_t *)malloc(total_capacity * buffer_element_size);

    assert(memory_buffer != NULL);

    arena.total_capacity = total_capacity;
    arena.memory_buffer = memory_buffer;

    return arena;
}

/**
 * @brief Mengalokasikan memori dari arena atau menggunakan malloc biasa
 * @param arena_ptr Pointer ke arena (NULL untuk menggunakan malloc)
 * @param size_in_bytes Ukuran memori yang dibutuhkan dalam bytes
 * @return Pointer ke memori yang dialokasikan, atau NULL jika gagal
 */
void*
arena_allocate_memory(struct MemoryArena *arena_ptr, size_t size_in_bytes)
{
    if (arena_ptr == NULL) return calloc(1, size_in_bytes);

    size_t buffer_element_size = sizeof(*arena_ptr->memory_buffer);
    size_t required_buffers = (size_in_bytes + buffer_element_size - 1) / buffer_element_size;

    assert(arena_ptr->used_buffers + required_buffers <= arena_ptr->total_capacity);

    if (arena_ptr->used_buffers + required_buffers > arena_ptr->total_capacity) return NULL;

    void *allocated_memory = &arena_ptr->memory_buffer[arena_ptr->used_buffers];
    arena_ptr->used_buffers += required_buffers;

    memset(allocated_memory, 0, size_in_bytes);
    return allocated_memory;
}

/**
 * @brief Mereset arena untuk menggunakan ulang memori
 * @param arena_ptr Pointer ke arena yang akan direset
 */
void
arena_reset(struct MemoryArena *arena_ptr)
{
    assert(arena_ptr != NULL);
    arena_ptr->used_buffers = 0;
}

// ====================[ MATRIX OPERATIONS - IMPLEMENTATION ]===================

/**
 * @brief Mengalokasikan matrix baru dengan ukuran tertentu
 * @param arena_ptr Arena untuk alokasi memori
 * @param num_rows Jumlah baris matrix
 * @param num_columns Jumlah kolom matrix
 * @return Struktur Matrix yang siap digunakan
 */
struct Matrix
matrix_allocate(struct MemoryArena *arena_ptr, size_t num_rows, size_t num_columns)
{
    struct Matrix new_matrix;

    new_matrix.num_rows = num_rows;
    new_matrix.num_columns = num_columns;
    new_matrix.element = arena_allocate_memory(arena_ptr, sizeof(*new_matrix.element) * num_rows * num_columns);

    assert(new_matrix.element != NULL);

    return new_matrix;
}

/**
 * @brief Mendapatkan baris tertentu dari matrix sebagai struktur Row
 * @param source_matrix Matrix sumber
 * @param row_index Indeks baris yang diinginkan
 * @return Struktur Row yang merepresentasikan baris tersebut
 */
struct Row
matrix_get_row(struct Matrix source_matrix, size_t row_index)
{
    assert(row_index < source_matrix.num_rows);

    return (struct Row) {
        .num_columns = source_matrix.num_columns,
        .element = &matrix_at(source_matrix, row_index, 0)
    };
}

/**
 * @brief Mengkonversi Row menjadi Matrix 1xN
 * @param source_row Row yang akan dikonversi
 * @return Matrix dengan 1 baris yang berisi data dari Row
 */
struct Matrix
row_convert_to_matrix(struct Row source_row)
{
    return (struct Matrix) {
        .num_rows = 1,
        .num_columns = source_row.num_columns,
        .element = source_row.element
    };
}

/**
 * @brief Membuat slice (potongan) dari Row
 * @param source_row Row sumber
 * @param start_index Indeks awal slice
 * @param slice_length Panjang slice
 * @return Row baru yang merupakan potongan dari row asli
 */
struct Row
row_create_slice(struct Row source_row, size_t start_index, size_t slice_length)
{
    assert(start_index < source_row.num_columns);
    assert(start_index + slice_length <= source_row.num_columns);

    return (struct Row) {
        .num_columns = slice_length,
        .element = &row_at(source_row, start_index)
    };
}

/**
 * @brief Mengisi semua elemen matrix dengan nilai tertentu
 * @param target_matrix Matrix yang akan diisi
 * @param fill_value Nilai yang akan diisikan ke semua elemen
 */
void
matrix_fill_with_value(struct Matrix target_matrix, float fill_value)
{
    for (size_t row_idx = 0; row_idx < target_matrix.num_rows; ++row_idx) {
        for (size_t col_idx = 0; col_idx < target_matrix.num_columns; ++col_idx) {
            matrix_at(target_matrix, row_idx, col_idx) = fill_value;
        }
    }
}

/**
 * @brief Mengisi matrix dengan nilai random dalam range tertentu
 * @param target_matrix Matrix yang akan diisi
 * @param min_value Nilai minimum untuk random
 * @param max_value Nilai maksimum untuk random
 */
void
matrix_fill_random(struct Matrix target_matrix, float min_value, float max_value)
{
    for (size_t row_idx = 0; row_idx < target_matrix.num_rows; ++row_idx) {
        for (size_t col_idx = 0; col_idx < target_matrix.num_columns; ++col_idx) {
            matrix_at(target_matrix, row_idx, col_idx) = RAND_FLOAT() * (max_value - min_value) + min_value;
        }
    }
}

/**
 * @brief Menjumlahkan dua matrix secara element-wise
 * @param destination_matrix Matrix pertama (hasil akan disimpan di sini)
 * @param source_matrix Matrix kedua yang akan ditambahkan
 */
void
matrix_add_elementwise(struct Matrix destination_matrix, struct Matrix source_matrix)
{
    assert(destination_matrix.num_rows == source_matrix.num_rows);
    assert(destination_matrix.num_columns == source_matrix.num_columns);

    for (size_t row_idx = 0; row_idx < destination_matrix.num_rows; ++row_idx) {
        for (size_t col_idx = 0; col_idx < destination_matrix.num_columns; ++col_idx) {
            matrix_at(destination_matrix, row_idx, col_idx) += matrix_at(source_matrix, row_idx, col_idx);
        }
    }
}

/**
 * @brief Melakukan perkalian matrix (dot product)
 * @param result_matrix Matrix untuk menyimpan hasil
 * @param matrix_a Matrix pertama
 * @param matrix_b Matrix kedua
 */
void
matrix_multiply_dot_product(struct Matrix result_matrix, struct Matrix matrix_a, struct Matrix matrix_b)
{
    assert(matrix_a.num_columns == matrix_b.num_rows);
    assert(result_matrix.num_rows == matrix_a.num_rows);
    assert(result_matrix.num_columns == matrix_b.num_columns);

    for (size_t row_idx = 0; row_idx < result_matrix.num_rows; ++row_idx) {
        for (size_t col_idx = 0; col_idx < result_matrix.num_columns; ++col_idx) {

            matrix_at(result_matrix, row_idx, col_idx) = 0.0f;

            for (size_t inner_idx = 0; inner_idx < matrix_a.num_columns; ++inner_idx) {
                matrix_at(result_matrix, row_idx, col_idx) +=
                        matrix_at(matrix_a, row_idx, inner_idx) * matrix_at(matrix_b, inner_idx, col_idx);
            }
        }
    }
}

/**
 * @brief Menyalin isi matrix sumber ke matrix tujuan
 * @param destination_matrix Matrix tujuan
 * @param source_matrix Matrix sumber
 */
void
matrix_copy_data(struct Matrix destination_matrix, struct Matrix source_matrix)
{
    assert(destination_matrix.num_rows == source_matrix.num_rows);
    assert(destination_matrix.num_columns == source_matrix.num_columns);

    for (size_t row_idx = 0; row_idx < destination_matrix.num_rows; ++row_idx) {
        for (size_t col_idx = 0; col_idx < destination_matrix.num_columns; ++col_idx) {
            matrix_at(destination_matrix, row_idx, col_idx) = matrix_at(source_matrix, row_idx, col_idx);
        }
    }
}

/**
 * @brief Menerapkan fungsi aktivasi ke semua elemen matrix
 * @param target_matrix Matrix yang akan diproses
 * @param activation_type Tipe fungsi aktivasi yang akan diterapkan
 */
void
matrix_apply_activation(struct Matrix target_matrix, enum ActivationType activation_type)
{
    for (size_t row_idx = 0; row_idx < target_matrix.num_rows; ++row_idx) {
        for (size_t col_idx = 0; col_idx < target_matrix.num_columns; ++col_idx) {
            matrix_at(target_matrix, row_idx, col_idx) =
                activation_apply(matrix_at(target_matrix, row_idx, col_idx), activation_type);
        }
    }
}

/**
 * @brief Mencetak matrix ke console dengan format yang rapi
 * @param input_matrix Matrix yang akan dicetak
 * @param matrix_name Nama matrix untuk label
 * @param indent_padding Jumlah spasi untuk indentasi
 */
void
matrix_print_to_console(struct Matrix input_matrix, const char *matrix_name, size_t indent_padding)
{
    printf("%*s%s = [\n", (int)indent_padding, "", matrix_name);
    for (size_t row_idx = 0; row_idx < input_matrix.num_rows; ++row_idx) {
        printf("%*s    ", (int)indent_padding, "");
        for (size_t col_idx = 0; col_idx < input_matrix.num_columns; ++col_idx) {
            printf("%f ", matrix_at(input_matrix, row_idx, col_idx));
        }
        printf("\n");
    }
    printf("%*s]\n", (int)indent_padding, "");
}

// =====================[ NEURAL NETWORK - IMPLEMENTATION ]=====================

/**
 * @brief Mengalokasikan neural network baru dengan arsitektur tertentu
 * @param arena_ptr Arena untuk alokasi memori
 * @param layer_architecture Array berisi ukuran setiap layer
 * @param total_layers Jumlah layer dalam arsitektur
 * @return Neural network yang siap digunakan
 */
struct NeuralNetwork
neural_network_allocate(struct MemoryArena *arena_ptr, size_t *layer_architecture, size_t total_layers)
{
    assert(total_layers > 1);

    struct NeuralNetwork neural_network;
    neural_network.layer_sizes = layer_architecture;
    neural_network.total_layers = total_layers;

    // Alokasi array untuk weights, biases, activations, dan activation types
    neural_network.weight_matrices = arena_allocate_memory(
            arena_ptr, sizeof(*neural_network.weight_matrices) * (total_layers - 1));
    assert(neural_network.weight_matrices != NULL);

    neural_network.bias_vectors = arena_allocate_memory(
            arena_ptr, sizeof(*neural_network.bias_vectors) * (total_layers - 1));
    assert(neural_network.bias_vectors != NULL);

    neural_network.activation_vectors = arena_allocate_memory(
            arena_ptr, sizeof(*neural_network.activation_vectors) * total_layers);
    assert(neural_network.activation_vectors != NULL);

    neural_network.activation_types = arena_allocate_memory(
            arena_ptr, sizeof(*neural_network.activation_types) * total_layers);
    assert(neural_network.activation_types != NULL);

    // Setup input layer (tidak ada aktivasi)
    neural_network.activation_vectors[0] = row_allocate(arena_ptr, layer_architecture[0]);
    neural_network.activation_types[0] = ACTIVATION_NONE;

    // Setup hidden dan output layers
    for (size_t layer_idx = 1; layer_idx < total_layers; ++layer_idx) {
        neural_network.weight_matrices[layer_idx - 1] =
            matrix_allocate(arena_ptr, layer_architecture[layer_idx - 1], layer_architecture[layer_idx]);
        neural_network.bias_vectors[layer_idx - 1] = row_allocate(arena_ptr, layer_architecture[layer_idx]);
        neural_network.activation_vectors[layer_idx] = row_allocate(arena_ptr, layer_architecture[layer_idx]);
        neural_network.activation_types[layer_idx] = ACTIVATION_RELU; // Default hidden layer activation
    }

    // Output layer menggunakan sigmoid untuk klasifikasi
    neural_network.activation_types[total_layers - 1] = ACTIVATION_SIGMOID;

    return neural_network;
}

/**
 * @brief Mengisi semua weights dan biases dengan nol
 * @param network Neural network yang akan di-zero
 */
void
neural_network_zero_weights(struct NeuralNetwork network)
{
    size_t last_layer_index = network.total_layers - 1;

    for (size_t layer_idx = 0; layer_idx < last_layer_index; ++layer_idx) {
        matrix_fill_with_value(network.weight_matrices[layer_idx], 0);
        row_fill_with_value(network.bias_vectors[layer_idx], 0);
        row_fill_with_value(network.activation_vectors[layer_idx], 0);
    }

    row_fill_with_value(network.activation_vectors[last_layer_index], 0);
}

/**
 * @brief Melakukan forward propagation pada neural network
 * @param network Neural network yang akan diproses
 */
void
neural_network_forward_pass(struct NeuralNetwork network)
{
    assert(network.total_layers > 1);

    // Proses setiap layer dari input ke output
    for (size_t layer_idx = 0; layer_idx < network.total_layers - 1; ++layer_idx) {
        // Perkalian matrix: activation[i] * weights[i]
        matrix_multiply_dot_product(
                row_convert_to_matrix(network.activation_vectors[layer_idx + 1]),
                row_convert_to_matrix(network.activation_vectors[layer_idx]),
                network.weight_matrices[layer_idx]);

        // Tambahkan bias
        matrix_add_elementwise(
                row_convert_to_matrix(network.activation_vectors[layer_idx + 1]),
                row_convert_to_matrix(network.bias_vectors[layer_idx]));

        // Terapkan fungsi aktivasi
        matrix_apply_activation(
                row_convert_to_matrix(network.activation_vectors[layer_idx + 1]),
                network.activation_types[layer_idx + 1]);
    }
}

/**
 * @brief Menghitung gradient menggunakan backpropagation
 * @param arena_ptr Arena untuk alokasi temporary
 * @param network Neural network
 * @param training_data Matrix berisi data training (input + output)
 * @return Neural network berisi gradient
 */
struct NeuralNetwork
neural_network_compute_gradients(struct MemoryArena *arena_ptr,
                                 struct NeuralNetwork network,
                                 struct Matrix training_data)
{
    size_t sample_count = training_data.num_rows;
    size_t input_columns = network.layer_sizes[0];
    size_t output_columns = network.layer_sizes[network.total_layers - 1];

    // Buat gradient network dengan struktur yang sama
    struct NeuralNetwork gradient_network =
        neural_network_allocate(arena_ptr, network.layer_sizes, network.total_layers);
    neural_network_zero_weights(gradient_network);

    // Proses setiap sample dalam training data
    for (size_t sample_idx = 0; sample_idx < sample_count; ++sample_idx) {
        struct Row sample_row = matrix_get_row(training_data, sample_idx);
        assert(input_columns + output_columns <= sample_row.num_columns);

        struct Row input_data = row_create_slice(sample_row, 0, input_columns);
        struct Row target_output = row_create_slice(sample_row, input_columns, output_columns);

        // Forward pass
        row_copy_data(network.activation_vectors[0], input_data);
        neural_network_forward_pass(network);

        // Reset gradient activations untuk sample ini
        for (size_t layer_idx = 0; layer_idx < network.total_layers; ++layer_idx)
            row_fill_with_value(gradient_network.activation_vectors[layer_idx], 0.0f);

        // Hitung error di output layer
        struct Row network_output = network.activation_vectors[network.total_layers - 1];
        struct Row gradient_output = gradient_network.activation_vectors[network.total_layers - 1];

        for (size_t output_idx = 0; output_idx < output_columns; ++output_idx)
            gradient_output.element[output_idx] =
                network_output.element[output_idx] - target_output.element[output_idx];

        // Backpropagation dari output ke input
        for (size_t layer_idx = network.total_layers - 1; layer_idx > 0; --layer_idx) {
            size_t current_layer_size = network.activation_vectors[layer_idx].num_columns;
            size_t previous_layer_size = network.activation_vectors[layer_idx - 1].num_columns;

            struct Row current_activation = network.activation_vectors[layer_idx];
            struct Row current_gradient = gradient_network.activation_vectors[layer_idx];
            struct Row previous_gradient = gradient_network.activation_vectors[layer_idx - 1];
            struct Matrix current_weights = network.weight_matrices[layer_idx - 1];
            struct Matrix gradient_weights = gradient_network.weight_matrices[layer_idx - 1];
            struct Row gradient_bias = gradient_network.bias_vectors[layer_idx - 1];

            // Hitung gradient untuk layer ini
            for (size_t neuron_idx = 0; neuron_idx < current_layer_size; ++neuron_idx) {
                float activation_value = current_activation.element[neuron_idx];
                float error_value = current_gradient.element[neuron_idx];
                float derivative_value =
                    activation_compute_derivative(activation_value, network.activation_types[layer_idx]);

                // Gradient bias
                gradient_bias.element[neuron_idx] += error_value * derivative_value;

                // Gradient weights dan propagasi error ke layer sebelumnya
                for (size_t prev_neuron_idx = 0; prev_neuron_idx < previous_layer_size; ++prev_neuron_idx) {
                    float previous_activation = network.activation_vectors[layer_idx - 1].element[prev_neuron_idx];
                    float weight_gradient = error_value * derivative_value * previous_activation;

                    matrix_at(gradient_weights, prev_neuron_idx, neuron_idx) += weight_gradient;
                    previous_gradient.element[prev_neuron_idx] +=
                            error_value * derivative_value * matrix_at(current_weights, prev_neuron_idx, neuron_idx);
                }
            }
        }
    }

    // Rata-rata gradient dari semua sample
    for (size_t layer_idx = 0; layer_idx < network.total_layers - 1; ++layer_idx) {
        struct Matrix weight_gradients = gradient_network.weight_matrices[layer_idx];
        struct Row bias_gradients = gradient_network.bias_vectors[layer_idx];

        for (size_t row_idx = 0; row_idx < weight_gradients.num_rows; ++row_idx)
            for (size_t col_idx = 0; col_idx < weight_gradients.num_columns; ++col_idx)
                matrix_at(weight_gradients, row_idx, col_idx) /= sample_count;

        for (size_t bias_idx = 0; bias_idx < bias_gradients.num_columns; ++bias_idx)
            bias_gradients.element[bias_idx] /= sample_count;
    }

    return gradient_network;
}

/**
 * @brief Menerapkan gradient untuk memperbarui weights dan biases
 * @param network Neural network yang akan diupdate
 * @param gradient_network Neural network berisi gradient
 * @param learning_rate Learning rate untuk update
 */
void
neural_network_apply_gradients(struct NeuralNetwork network,
                               struct NeuralNetwork gradient_network,
                               float learning_rate)
{
    // Update weights dan biases menggunakan gradient descent
    for (size_t layer_idx = 0; layer_idx < network.total_layers - 1; ++layer_idx) {
        // Update weights
        for (size_t row_idx = 0; row_idx < network.weight_matrices[layer_idx].num_rows; ++row_idx) {
            for (size_t col_idx = 0; col_idx < network.weight_matrices[layer_idx].num_columns; ++col_idx) {
                matrix_at(network.weight_matrices[layer_idx], row_idx, col_idx) -=
                        learning_rate * matrix_at(gradient_network.weight_matrices[layer_idx], row_idx, col_idx);
            }
        }

        // Update biases
        for (size_t bias_idx = 0; bias_idx < network.bias_vectors[layer_idx].num_columns; ++bias_idx) {
            row_at(network.bias_vectors[layer_idx], bias_idx) -=
                    learning_rate * row_at(gradient_network.bias_vectors[layer_idx], bias_idx);
        }
    }
}

// ===================[ DATASET OPERATIONS - IMPLEMENTATION ]===================

/**
 * @brief Memuat dataset dari file CSV
 * @param arena_ptr Arena untuk alokasi memori
 * @param csv_filename Nama file CSV yang akan dimuat
 * @param skip_header_lines Jumlah baris header yang akan dilewati
 * @return Matrix berisi data dari CSV
 */
struct Matrix dataset_load_from_csv(struct MemoryArena *arena_ptr, const char *csv_filename, size_t skip_header_lines)
{
    enum {
        MAX_ROWS = 2048,
        INPUT_FEATURES = 4,    // Features in Iris dataset
        OUTPUT_CLASSES = 3,    // Class count: setosa, versicolor, virginica
        TOTAL_COLUMNS = INPUT_FEATURES + OUTPUT_CLASSES,
        MAX_LINE_LENGTH = 4096
    };

    FILE *csv_file = fopen(csv_filename, "r");
    assert(csv_file && "Gagal membuka file CSV");

    struct Matrix dataset = matrix_allocate(arena_ptr, MAX_ROWS, TOTAL_COLUMNS);
    matrix_fill_with_value(dataset, 0.0f);

    char line[MAX_LINE_LENGTH];
    size_t row_index = 0;

    // Skip header lines
    for (size_t i = 0; i < skip_header_lines; ++i) {
        if (!fgets(line, sizeof(line), csv_file))
            break;  // Jika header lebih banyak dari jumlah baris, keluar
    }

    while (fgets(line, sizeof(line), csv_file) && row_index < dataset.num_rows) {
        float features[INPUT_FEATURES];
        int label = -1;

        int parsed = sscanf(line, "%f,%f,%f,%f,%d",
                            &features[0], &features[1], &features[2], &features[3], &label);

        if (parsed != 5)
            continue;

        // Pastikan label valid
        assert(label >= 0 && label < OUTPUT_CLASSES && "Label class diluar jangkauan");

        // Simpan fitur input
        for (size_t i = 0; i < INPUT_FEATURES; ++i) {
            matrix_at(dataset, row_index, i) = features[i];
        }

        // One-hot encoding untuk label output
        matrix_at(dataset, row_index, INPUT_FEATURES + (size_t)label) = 1.0f;

        ++row_index;
    }

    fclose(csv_file);

    dataset.num_rows = row_index;
    dataset.num_columns = TOTAL_COLUMNS;

    return dataset;
}

/**
 * @brief Membuat slice dari beberapa baris matrix
 * @param source_matrix Matrix sumber
 * @param start_row Baris awal slice
 * @param num_rows Jumlah baris yang diambil
 * @return Matrix baru yang merupakan slice dari matrix asli
 */
struct Matrix
matrix_create_row_slice(struct Matrix source_matrix, size_t start_row, size_t num_rows)
{
    assert(start_row + num_rows <= source_matrix.num_rows);

    return (struct Matrix) {
        .num_rows = num_rows,
        .num_columns = source_matrix.num_columns,
        .element = &matrix_at(source_matrix, start_row, 0)
    };
}

// ====================[ BATCH PROCESSING - IMPLEMENTATION ]====================

/**
 * @brief Memproses satu batch data untuk training
 * @param arena_ptr Arena untuk alokasi temporary
 * @param batch_processor Struktur batch yang melacak progress
 * @param batch_size Ukuran batch
 * @param network Neural network yang akan dilatih
 * @param training_dataset Dataset training
 * @param learning_rate Learning rate untuk update
 */
void
batch_process_training_data(struct MemoryArena *arena_ptr,
                            struct BatchProcessor *batch_processor,
                            size_t batch_size,
                            struct NeuralNetwork network,
                            struct Matrix training_dataset,
                            float learning_rate)
{
    // Reset jika epoch selesai
    if (batch_processor->is_epoch_finished) {
        batch_processor->current_start_idx = 0;
        batch_processor->accumulated_cost = 0.0f;
        batch_processor->is_epoch_finished = false;
    }

    // Tentukan ukuran batch actual (mungkin lebih kecil di batch terakhir)
    size_t actual_batch_size = (batch_processor->current_start_idx + batch_size >= training_dataset.num_rows)
                    ? (training_dataset.num_rows - batch_processor->current_start_idx) : batch_size;

    struct Matrix current_batch = matrix_create_row_slice(
            training_dataset, batch_processor->current_start_idx, actual_batch_size);

    // Simpan state arena untuk reset setelah selesai
    size_t arena_checkpoint = arena_ptr->used_buffers;

    // Hitung gradient dan update network
    struct NeuralNetwork batch_gradients = neural_network_compute_gradients(arena_ptr, network, current_batch);
    neural_network_apply_gradients(network, batch_gradients, learning_rate);

    // Akumulasi cost untuk monitoring
    batch_processor->accumulated_cost += neural_network_calculate_cost(network, current_batch);
    batch_processor->current_start_idx += actual_batch_size;

    // Cek apakah epoch selesai
    if (batch_processor->current_start_idx >= training_dataset.num_rows) {
        size_t total_batch_count = (training_dataset.num_rows + batch_size - 1) / batch_size;
        batch_processor->accumulated_cost /= total_batch_count;
        batch_processor->is_epoch_finished = true;
    }

    // Reset arena ke checkpoint (hanya free temporary allocations)
    arena_ptr->used_buffers = arena_checkpoint;
}

/**
 * @brief Melatih neural network dengan dataset lengkap
 * @param network Neural network yang akan dilatih
 * @param arena_ptr Arena untuk alokasi temporary
 * @param training_dataset Dataset training
 * @param batch_size Ukuran batch untuk training
 * @param learning_rate Learning rate
 * @param num_epochs Jumlah epoch training
 */
void
neural_network_train(struct NeuralNetwork network,
                     struct MemoryArena *arena_ptr,
                     struct Matrix training_dataset, size_t batch_size,
                     float learning_rate, size_t num_epochs)
{
    for (size_t epoch_idx = 0; epoch_idx < num_epochs; ++epoch_idx) {
        struct BatchProcessor batch_processor = {0};

        // Proses semua batch dalam satu epoch
        while (!batch_processor.is_epoch_finished)
            batch_process_training_data(
                    arena_ptr, &batch_processor, batch_size, network, training_dataset, learning_rate);

        // Print progress
        printf("Epoch %zu selesai. Loss rata-rata: %.4f, Akurasi: %.2f%%\n",
                epoch_idx + 1,
                batch_processor.accumulated_cost,
                100.0f * neural_network_calculate_accuracy(network, training_dataset));
    }
}

/**
 * @brief Mengacak urutan baris dalam matrix (untuk shuffling dataset)
 * @param target_matrix Matrix yang baris-barisnya akan diacak
 */
void
matrix_shuffle_rows(struct Matrix target_matrix)
{
    if (target_matrix.num_rows <= 1) return;

    // Fisher-Yates shuffle algorithm
    for (size_t current_idx = 0; current_idx < target_matrix.num_rows; ++current_idx) {
        size_t random_idx = current_idx + rand() % (target_matrix.num_rows - current_idx);
        if (current_idx != random_idx) {
            // Tukar baris current_idx dengan baris random_idx
            for (size_t col_idx = 0; col_idx < target_matrix.num_columns; ++col_idx) {
                float temp_value = matrix_at(target_matrix, current_idx, col_idx);
                matrix_at(target_matrix, current_idx, col_idx) = matrix_at(target_matrix, random_idx, col_idx);
                matrix_at(target_matrix, random_idx, col_idx) = temp_value;
            }
        }
    }
}

/**
 * @brief Menghitung cost function (mean squared error) pada dataset
 * @param network Neural network
 * @param test_dataset Dataset untuk evaluasi
 * @return Nilai cost rata-rata
 */
float
neural_network_calculate_cost(struct NeuralNetwork network, struct Matrix test_dataset)
{
    size_t sample_count = test_dataset.num_rows;
    size_t input_size = network.activation_vectors[0].num_columns;
    size_t output_size = network.activation_vectors[network.total_layers - 1].num_columns;

    float total_cost = 0.0f;

    // Hitung MSE untuk setiap sample
    for (size_t sample_idx = 0; sample_idx < sample_count; ++sample_idx) {
        struct Row sample_row = matrix_get_row(test_dataset, sample_idx);
        struct Row input_data = row_create_slice(sample_row, 0, input_size);
        struct Row expected_output = row_create_slice(sample_row, input_size, output_size);

        // Forward pass
        row_copy_data(network.activation_vectors[0], input_data);
        neural_network_forward_pass(network);

        // Hitung squared error
        for (size_t output_idx = 0; output_idx < output_size; ++output_idx) {
            float prediction_diff = row_at(network.activation_vectors[network.total_layers - 1], output_idx) -
                                    row_at(expected_output, output_idx);

            total_cost += prediction_diff * prediction_diff;
        }
    }

    return total_cost / sample_count;
}

/**
 * @brief Mencari indeks elemen dengan nilai maksimum dalam row
 * @param input_row Row yang akan dicari nilai maksimumnya
 * @return Indeks elemen dengan nilai terbesar
 */
size_t
row_find_max_index(struct Row input_row)
{
    size_t max_element_index = 0;
    for (size_t element_idx = 1; element_idx < input_row.num_columns; ++element_idx) {
        if (input_row.element[element_idx] > input_row.element[max_element_index]) {
            max_element_index = element_idx;
        }
    }
    return max_element_index;
}

/**
 * @brief Menghitung akurasi klasifikasi neural network
 * @param network Neural network yang akan dievaluasi
 * @param test_dataset Dataset untuk evaluasi
 * @return Akurasi antara 0.0 hingga 1.0
 */
float
neural_network_calculate_accuracy(struct NeuralNetwork network, struct Matrix test_dataset)
{
    size_t correct_predictions = 0;
    size_t total_samples = test_dataset.num_rows;

    // Evaluasi setiap sample
    for (size_t sample_idx = 0; sample_idx < total_samples; ++sample_idx) {
        struct Row sample_row = matrix_get_row(test_dataset, sample_idx);
        struct Row input_data = row_create_slice(sample_row, 0, network.activation_vectors[0].num_columns);
        struct Row expected_output = row_create_slice(
                sample_row, network.activation_vectors[0].num_columns,
                network.activation_vectors[network.total_layers - 1].num_columns);

        // Forward pass
        row_copy_data(network.activation_vectors[0], input_data);
        neural_network_forward_pass(network);

        // Bandingkan prediksi dengan label sebenarnya
        size_t predicted_class = row_find_max_index(network.activation_vectors[network.total_layers - 1]);
        size_t actual_class = row_find_max_index(expected_output);

        if (predicted_class == actual_class) {
            ++correct_predictions;
        }
    }

    return (float)correct_predictions / total_samples;
}

/**
 * @brief Normalisasi min-max pada kolom input matrix
 * @param target_matrix Matrix yang akan dinormalisasi
 * @param num_input_columns Jumlah kolom input yang akan dinormalisasi
 * @param new_min_value Nilai minimum baru setelah normalisasi
 * @param new_max_value Nilai maksimum baru setelah normalisasi
 */
void
matrix_normalize_minmax(struct Matrix target_matrix, size_t num_input_columns, float new_min_value, float new_max_value)
{
    // Normalisasi setiap kolom secara terpisah
    for (size_t col_idx = 0; col_idx < num_input_columns; ++col_idx) {
        // Cari min dan max untuk kolom col_idx
        float column_min_value = matrix_at(target_matrix, 0, col_idx);
        float column_max_value = matrix_at(target_matrix, 0, col_idx);

        for (size_t row_idx = 1; row_idx < target_matrix.num_rows; ++row_idx) {
            float current_value = matrix_at(target_matrix, row_idx, col_idx);
            if (current_value < column_min_value) column_min_value = current_value;
            if (current_value > column_max_value) column_max_value = current_value;
        }

        // Hindari pembagian dengan nol
        float value_range = column_max_value - column_min_value;
        if (value_range == 0.0f) value_range = 1.0f;

        // Terapkan normalisasi min-max
        for (size_t row_idx = 0; row_idx < target_matrix.num_rows; ++row_idx) {
            float *current_value_ptr = &matrix_at(target_matrix, row_idx, col_idx);
            *current_value_ptr = ((*current_value_ptr - column_min_value) / value_range) *
                                 (new_max_value - new_min_value) + new_min_value;
        }
    }
}

/**
 * @brief Inisialisasi weights neural network dengan nilai random
 * @param network Neural network yang akan diinisialisasi
 * @param min_weight Nilai minimum untuk random weights
 * @param max_weight Nilai maksimum untuk random weights
 */
void
neural_network_randomize_weights(struct NeuralNetwork network, float min_weight, float max_weight)
{
    // Inisialisasi semua weights dengan random, biases dengan nol
    for (size_t layer_idx = 0; layer_idx < network.total_layers - 1; ++layer_idx) {
        matrix_fill_random(network.weight_matrices[layer_idx], min_weight, max_weight);
        row_fill_with_value(network.bias_vectors[layer_idx], 0.0f);
    }
}

/* vim: set ts=4 sw=4 sts=4 et */


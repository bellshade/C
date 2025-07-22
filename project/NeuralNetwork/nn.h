
/**
 * @file nn.h
 * @brief Library Neural Network sederhana untuk klasifikasi
 * @author 0xfa99
 * @version 1.0
 *
 * Library ini menyediakan implementasi neural network sederhana dengan:
 * - Berbagai fungsi aktivasi (sigmoid, ReLU, tanh)
 * - Forward dan backward propagation
 * - Training dengan batch processing
 * - Manajemen memori menggunakan arena allocator
 * - Utilitas untuk matrix dan dataset
 */

#ifndef NN_H
#define NN_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

// ==================================[ MACROS ]=================================

/**
 * @brief Makro untuk menghasilkan bilangan float random antara 0-1
 */
#define RAND_FLOAT() ((float)(rand()) / (float)(RAND_MAX))

/**
 * @brief Makro untuk mengakses elemen matrix
 * @param matrix_data Matrix yang akan diakses
 * @param row_idx Indeks baris
 * @param col_idx Indeks kolom
 */
#define matrix_at(matrix_data, row_idx, col_idx)                               \
    (matrix_data).element[(row_idx) * (matrix_data).num_columns + (col_idx)]

/**
 * @brief Makro untuk mengakses elemen row
 * @param row_data Row yang akan diakses
 * @param col_idx Indeks kolom
 */
#define row_at(row_data, col_idx) (row_data).element[col_idx]

//==================================[ MACROS ]=================================

/**
 * @brief Enum untuk tipe fungsi aktivasi yang tersedia
 */
enum ActivationType
{
    ACTIVATION_SIGMOID, // Fungsi sigmoid (0-1)
    ACTIVATION_TANH,    // Fungsi hyperbolic tangent (-1 sampai 1)
    ACTIVATION_RELU,    // Rectified Linear Unit (0 atau nilai positif)
    ACTIVATION_NONE     // Tanpa aktivasi (linear)
};

// ================================[ STRUCTURES ]===============================

/**
 * @brief Struktur arena untuk manajemen memori efisien
 *
 * Arena allocator memungkinkan alokasi memori yang cepat dan mudah
 * untuk reset semua alokasi sekaligus
 */
struct MemoryArena
{
    size_t total_capacity;      // Kapasitas total arena dalam satuan buffer
    size_t used_buffers;        // Jumlah buffer yang sudah digunakan
    uintptr_t *memory_buffer;   // Pointer ke buffer memori
};

/**
 * @brief Struktur untuk merepresentasikan matrix 2D
 */
struct Matrix
{
    size_t num_rows;    // Jumlah baris
    size_t num_columns; // Jumlah kolom
    float *element;     // Pointer ke elemen-elemen matrix
};

/**
 * @brief Struktur untuk merepresentasikan satu baris (1D array)
 */
struct Row
{
    size_t num_columns; // Jumlah kolom/elemen
    float *element;     // Pointer ke elemen-elemen row
};

/**
 * @brief Struktur utama Neural Network
 *
 * Berisi semua komponen yang diperlukan untuk neural network:
 * arsitektur, weights, biases, aktivasi, dan tipe aktivasi
 */
struct NeuralNetwork
{
    size_t *layer_sizes;                    // Array ukuran setiap layer
    size_t total_layers;                    // Jumlah layer
    struct Matrix *weight_matrices;         // Array matrix weights antar layer
    struct Row *bias_vectors;               // Array bias untuk setiap layer
    struct Row *activation_vectors;         // Array aktivasi untuk setiap layer
    enum ActivationType *activation_types;  // Array tipe aktivasi untuk setiap layer
};

/**
 * @brief Struktur untuk batch processing
 *
 * Memungkinkan training dalam batch kecil untuk efisiensi memori
 */
struct BatchProcessor
{
    size_t current_start_idx;   // Indeks awal batch saat ini
    float accumulated_cost;     // Akumulasi cost dari batch
    bool is_epoch_finished;     // Flag apakah sudah selesai semua batch
};

// ===========================[ ACTIVATION FUNCTIONS ]==========================

/**
 * @brief Menghitung fungsi sigmoid
 * @param input_value Nilai input
 * @return Hasil sigmoid antara 0 dan 1
 */
float activation_sigmoid(float input_value);

/**
 * @brief Menghitung fungsi ReLU (Rectified Linear Unit)
 * @param input_value Nilai input
 * @return 0 jika input_value <= 0, input_value jika input_value > 0
 */
float activation_relu(float input_value);

/**
 * @brief Menghitung fungsi hyperbolic tangent
 * @param input_value Nilai input
 * @return Hasil tanh antara -1 dan 1
 */
float activation_tanh(float input_value);

/**
 * @brief Menerapkan fungsi aktivasi sesuai tipe yang dipilih
 * @param input_value Nilai input
 * @param activation_type Tipe aktivasi yang akan diterapkan
 * @return Hasil aktivasi
 */
float activation_apply(float input_value, enum ActivationType activation_type);

/**
 * @brief Menghitung turunan fungsi aktivasi (untuk backpropagation)
 * @param activated_value Nilai yang sudah melalui aktivasi
 * @param activation_type Tipe aktivasi
 * @return Nilai turunan fungsi aktivasi
 */
float activation_compute_derivative(float activated_value, enum ActivationType activation_type);

// ============================[ MEMORY MANAGEMENT ]============================

/**
 * @brief Membuat arena baru dengan ukuran tertentu
 * @param size_in_bytes Ukuran arena dalam bytes
 * @return Struktur MemoryArena yang siap digunakan
 */
struct MemoryArena arena_create(size_t size_in_bytes);

/**
 * @brief Mengalokasikan memori dari arena
 * @param arena_ptr Pointer ke arena (NULL untuk malloc biasa)
 * @param size_in_bytes Ukuran memori yang dibutuhkan dalam bytes
 * @return Pointer ke memori yang dialokasikan, atau NULL jika gagal
 */
void *arena_allocate_memory(struct MemoryArena *arena_ptr, size_t size_in_bytes);

/**
 * @brief Mereset arena untuk menggunakan ulang memori
 * @param arena_ptr Pointer ke arena yang akan direset
 */
void arena_reset(struct MemoryArena *arena_ptr);

// ============================[ MATRIX OPERATIONS ]============================

/**
 * @brief Mengalokasikan matrix baru
 * @param arena_ptr Arena untuk alokasi memori
 * @param num_rows Jumlah baris
 * @param num_columns Jumlah kolom
 * @return Matrix yang telah dialokasikan
 */
struct Matrix matrix_allocate(struct MemoryArena *arena_ptr, size_t num_rows, size_t num_columns);

/**
 * @brief Mendapatkan row tertentu dari matrix
 * @param source_matrix Matrix sumber
 * @param row_index Indeks baris yang diinginkan
 * @return Struktur Row yang merepresentasikan baris tersebut
 */
struct Row matrix_get_row(struct Matrix source_matrix, size_t row_index);

/**
 * @brief Mengkonversi row menjadi matrix 1x N
 * @param source_row Row yang akan dikonversi
 * @return Matrix dengan 1 baris
 */
struct Matrix row_convert_to_matrix(struct Row source_row);

/**
 * @brief Membuat slice (potongan) dari row
 * @param source_row Row sumber
 * @param start_index Indeks awal
 * @param slice_length Jumlah kolom yang diambil
 * @return Row baru yang merupakan potongan dari row asli
 */
struct Row row_create_slice(struct Row source_row, size_t start_index, size_t slice_length);

/**
 * @brief Mencari indeks dengan nilai maksimum dalam row
 * @param input_row Row yang akan dicari nilai maksimumnya
 * @return Indeks elemen dengan nilai terbesar
 */
size_t row_find_max_index(struct Row input_row);

/**
 * @brief Membuat slice dari beberapa baris matrix
 * @param source_matrix Matrix sumber
 * @param start_row Baris awal
 * @param num_rows Jumlah baris yang diambil
 * @return Matrix baru yang merupakan potongan dari matrix asli
 */
struct Matrix matrix_create_row_slice(struct Matrix source_matrix, size_t start_row, size_t num_rows);

/**
 * @brief Mengisi semua elemen matrix dengan nilai tertentu
 * @param target_matrix Matrix yang akan diisi
 * @param fill_value Nilai yang akan diisikan
 */
void matrix_fill_with_value(struct Matrix target_matrix, float fill_value);

/**
 * @brief Mengisi matrix dengan nilai random dalam range tertentu
 * @param target_matrix Matrix yang akan diisi
 * @param min_value Nilai minimum
 * @param max_value Nilai maksimum
 */
void matrix_fill_random(struct Matrix target_matrix, float min_value, float max_value);

/**
 * @brief Menjumlahkan dua matrix (element-wise)
 * @param destination_matrix Matrix pertama (hasil akan disimpan di sini)
 * @param source_matrix Matrix kedua
 */
void matrix_add_elementwise(struct Matrix destination_matrix, struct Matrix source_matrix);

/**
 * @brief Perkalian matrix (dot product)
 * @param result_matrix Matrix tujuan untuk hasil
 * @param matrix_a Matrix pertama
 * @param matrix_b Matrix kedua
 */
void matrix_multiply_dot_product(struct Matrix result_matrix, struct Matrix matrix_a, struct Matrix matrix_b);

/**
 * @brief Menyalin isi source_matrix ke destination_matrix
 * @param destination_matrix Matrix tujuan
 * @param source_matrix Matrix sumber
 */
void matrix_copy_data(struct Matrix destination_matrix, struct Matrix source_matrix);

/**
 * @brief Menerapkan fungsi aktivasi ke semua elemen matrix
 * @param target_matrix Matrix yang akan diproses
 * @param activation_type Tipe fungsi aktivasi
 */
void matrix_apply_activation(struct Matrix target_matrix, enum ActivationType activation_type);

/**
 * @brief Mencetak matrix ke console
 * @param input_matrix Matrix yang akan dicetak
 * @param matrix_name Nama matrix (untuk label)
 * @param indent_padding Jumlah spasi untuk indentasi
 */
void matrix_print_to_console(struct Matrix input_matrix, const char *matrix_name, size_t indent_padding);

/**
 * @brief Mengacak urutan baris dalam matrix
 * @param target_matrix Matrix yang akan diacak baris-barisnya
 */
void matrix_shuffle_rows(struct Matrix target_matrix);

/**
 * @brief Normalisasi min-max pada kolom input matrix
 * @param target_matrix Matrix yang akan dinormalisasi
 * @param num_input_columns Jumlah kolom input yang akan dinormalisasi
 * @param new_min_value Nilai minimum baru
 * @param new_max_value Nilai maksimum baru
 */
void matrix_normalize_minmax(struct Matrix target_matrix,
                             size_t num_input_columns,
                             float new_min_value,
                             float new_max_value);

// ============================[ DATASET OPERATIONS ]===========================

/**
 * @brief Memuat dataset dari file CSV
 * @param arena_ptr Arena untuk alokasi memori
 * @param csv_filename Nama file CSV
 * @param skip_header_lines Jumlah baris yang akan dilewati (biasanya header)
 * @return Matrix berisi data dari CSV
 */
struct Matrix dataset_load_from_csv(struct MemoryArena *arena_ptr, const char *csv_filename, size_t skip_header_lines);

// ========================[ NEURAL NETWORK OPERATIONS ]========================

/**
 * @brief Mengalokasikan neural network baru
 * @param arena_ptr Arena untuk alokasi memori
 * @param layer_architecture Array yang mendefinisikan ukuran setiap layer
 * @param total_layers Jumlah layer dalam arsitektur
 * @return Neural network yang siap digunakan
 */
struct NeuralNetwork neural_network_allocate(struct MemoryArena *arena_ptr,
                                             size_t *layer_architecture,
                                             size_t total_layers);

/**
 * @brief Melakukan forward propagation
 * @param network Neural network yang akan diproses
 */
void neural_network_forward_pass(struct NeuralNetwork network);

/**
 * @brief Mengisi semua weights dan biases dengan nol
 * @param network Neural network yang akan di-zero
 */
void neural_network_zero_weights(struct NeuralNetwork network);

/**
 * @brief Melakukan backpropagation dan menghitung gradient
 * @param arena_ptr Arena untuk alokasi temporary
 * @param network Neural network
 * @param training_data Matrix berisi data training (input + output)
 * @return Neural network berisi gradient
 */
struct NeuralNetwork neural_network_compute_gradients(struct MemoryArena *arena_ptr,
                                                      struct NeuralNetwork network,
                                                      struct Matrix training_data);

/**
 * @brief Memperbarui weights dan biases menggunakan gradient
 * @param network Neural network yang akan diupdate
 * @param gradient_network Neural network berisi gradient
 * @param learning_rate Learning rate
 */
void neural_network_apply_gradients(struct NeuralNetwork network,
                                    struct NeuralNetwork gradient_network,
                                    float learning_rate);

/**
 * @brief Melatih neural network dengan dataset
 * @param network Neural network yang akan dilatih
 * @param arena_ptr Arena untuk alokasi temporary
 * @param training_dataset Matrix berisi data training
 * @param batch_size Ukuran batch untuk training
 * @param learning_rate Learning rate
 * @param num_epochs Jumlah epoch training
 */
void neural_network_train(struct NeuralNetwork network,
                          struct MemoryArena *arena_ptr,
                          struct Matrix training_dataset,
                          size_t batch_size,
                          float learning_rate,
                          size_t num_epochs);

/**
 * @brief Menghitung akurasi neural network pada dataset
 * @param network Neural network yang akan dievaluasi
 * @param test_dataset Dataset untuk evaluasi
 * @return Akurasi antara 0.0 hingga 1.0
 */
float neural_network_calculate_accuracy(struct NeuralNetwork network, struct Matrix test_dataset);

/**
 * @brief Menghitung cost/loss function (mean squared error)
 * @param network Neural network
 * @param test_dataset Dataset untuk evaluasi
 * @return Nilai cost
 */
float neural_network_calculate_cost(struct NeuralNetwork network, struct Matrix test_dataset);

/**
 * @brief Menginisialisasi weights dengan nilai random
 * @param network Neural network
 * @param min_weight Nilai minimum untuk random
 * @param max_weight Nilai maksimum untuk random
 */
void neural_network_randomize_weights(struct NeuralNetwork network, float min_weight, float max_weight);

// =============================[ BATCH PROCESSING ]============================

/**
 * @brief Memproses satu batch data untuk training
 * @param arena_ptr Arena untuk alokasi temporary
 * @param batch_processor Struktur batch yang melacak progress
 * @param batch_size Ukuran batch
 * @param network Neural network yang akan dilatih
 * @param training_dataset Dataset training
 * @param learning_rate Learning rate
 */
void batch_process_training_data(struct MemoryArena *arena_ptr,
                                 struct BatchProcessor *batch_processor,
                                 size_t batch_size,
                                 struct NeuralNetwork network,
                                 struct Matrix training_dataset,
                                 float learning_rate);

// ==============================[ ROW OPERATIONS ]=============================

/**
 * @brief Makro untuk menyalin row
 */
#define row_copy_data(dst_row, src_row)                                        \
    matrix_copy_data(row_convert_to_matrix(dst_row),                           \
                     row_convert_to_matrix(src_row))

/**
 * @brief Makro untuk mengalokasikan row
 */
#define row_allocate(arena_ptr, num_columns)                                   \
    matrix_get_row(matrix_allocate(arena_ptr, 1, num_columns), 0)

/**
 * @brief Makro untuk mencetak row
 */
#define row_print_to_console(input_row, row_name, indent_padding)              \
    matrix_print_to_console(row_convert_to_matrix(input_row),                  \
                            row_name,                                          \
                            indent_padding)

/**
 * @brief Makro untuk mengisi row dengan nilai tertentu
 */
#define row_fill_with_value(target_row, fill_value)                            \
    matrix_fill_with_value(row_convert_to_matrix(target_row), fill_value)

#if defined(__cplusplus)
}
#endif

#endif

/* vim: set ts=4 sw=4 sts=4 et */


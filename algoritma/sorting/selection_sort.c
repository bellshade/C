/**
 * @file selection_sort.c 
 * @brief implementasi algoritma selection sort.
 * 
 * file ini berisi fungsi selection sort untuk mengurutkan array integer
 * dan fungsi untuk mencetak array.
 * */

#include <stdio.h>

/**
 * @brief fungsi sederhana untuk menukar nilai dari 2 variabel
 *
 * @param int *x
 * @param int *y
 *
 * @return void
 **/

void swap(int *x, int *y) {
  int tmp;

  tmp = *x;
  *x = *y;
  *y = tmp;
}

/**
 * @brief mengurutkan array integer menggunakan insertion sort.
 *
 * Fungsi ini mengimplementasikan
 *  - in-place
 *  - mencari elemen paling kecil dalam array yang belum terurut 
 *    dan menukar posisi elemen tersebut dengan elemen pertama
 *  - mengulang proses tersebut sampai semua elemen berurutan, 
 *    dalam kasus ini dari yang terkecil sampai yang terbesar
 *
 * @param array Array integer yang akan diurutkan.
 * @param jumlah_elemen berupa integer dari array.
 *
 *
 * @return void
 * */

void selection_sort(int *array, int jumlah_elemen) {
  int indeks_i, indeks_j, min_j;

  for (indeks_i = 0; indeks_i < jumlah_elemen - 1; indeks_i++) {
    min_j = indeks_i;

    for (indeks_j = indeks_i + 1; indeks_j < jumlah_elemen; indeks_j++) {
      if (array[indeks_j] < array[min_j])
        min_j = indeks_j;
    }

    if (min_j != indeks_i)
      swap(&array[indeks_i], &array[min_j]);
  }
}

/**
 * @brief fungsi sederhana untuk mencetak keluaran dari array.
 *
 * @param array Array integer yang akan dicetak.
 * @param jumlah_elemen berupa integer dari array.
 *
 * @return void
 **/

void cetak_array(int array[], int jumlah_elemen) {
  for (int i = 0; i < jumlah_elemen; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main() {
  int array[10] = {3, 2, 6, 1, 7, 4, 9, 0, 5, 8};

  printf("Bentuk array sebelum diurutkan: ");
  cetak_array(array, 10);
  printf("\n");

  selection_sort(array, 10);

  printf("Bentuk array setelah diurutkan: ");
  cetak_array(array, 10);
  printf("\n");
  
  return 0;
}

/**
 * @file bubble_sort.c
 * @brief Implementasi algoritma Bubble Sort.
 *
 * File ini berisi fungsi `swap` untuk menukar 2 nilai dari objek integer,
 * bubble_sort` untuk mengurutkan array integer, dan 
 * fungsi `cetak_array` untuk menampilkan isinya.
 * Algoritma bubble sort cocok untuk array dengan ukuran kecil.
 */

#include <stdio.h>

/**
 * @brief Menukar nilai dua variabel integer.
 *
 * Fungsi ini menukar nilai yang ditunjuk oleh pointer A dan B.
 *
 * @param A Pointer ke integer pertama.
 * @param B Pointer ke integer kedua.
 *
 * @return void
 */

void tukar(int *A, int *B) {
  int tmp;
  tmp = *A;
  *A = *B;
  *B = tmp;
}

/**
 * @brief Mengurutkan array integer menggunakan bubble sort.
 *
 * Fungsi ini mengimplementasikan algoritma bubble sort yang:
 * - Beroperasi secara in-place (tidak memerlukan memori tambahan yang signifikan).
 * - Membandingkan elemen-elemen yang berdekatan dan menukarnya jika urutannya salah,
 * secara berulang hingga array terurut. Elemen terbesar "menggelembung" ke akhir array
 * pada setiap iterasi.
 *
 * @param array Array integer yang akan diurutkan.
 * @param jumlah_element Jumlah elemen dalam array.
 *
 * @return void
 */

void bubble_sort(int array[], int jumlah_elemen) {
  int newn, indeks;

  do {
    newn = 0;
    for (indeks = 1; indeks < jumlah_elemen; indeks++) {
      if (array[indeks-1] > array[indeks]) {
        tukar(&array[indeks-1], &array[indeks]);
        newn = indeks;
      }
    }
    jumlah_elemen = newn;
  } while (jumlah_elemen > 1);
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
  int array[10] = {4, 2, 0, 1, 3, 9, 5, 7, 8, 6};

  printf("Bentuk array sebelum diurutkan: ");
  cetak_array(array, 10);
  printf("\n");

  bubble_sort(array, 10);

  printf("Bentuk array setelah diurutkan: ");
  cetak_array(array, 10);
  printf("\n");
  
  return 0;
}

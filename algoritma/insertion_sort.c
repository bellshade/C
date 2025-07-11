/**
 * @file insertion_sort.c 
 * @brief implementasi algoritma insertion sort.
 * 
 * file ini berisi fungsi insertion sort untuk mengurutkan array integer
 * dan fungsi untuk mencetak array. Algoritma ini cocok untuk ukuran array
 * integer yang kecil.
 * */

#include <stdio.h>

/**
 * @brief mengurutkan array integer menggunakan insertion sort.
 *
 * Fungsi ini mengimplementasikan
 *  - in-place
 *  - setiap elemen akan disisipkan ke posisi yang benar di antara elemen-elemen
 *
 * @param array Array integer yang akan diurutkan.
 * @param jumlah_element berupa integer dari array.
 *
 *
 * @return void
 * */

void insertion_sort(int array[], int jumlah_elemen) {
  int indeks, j, x;
  for (indeks = 1; indeks < jumlah_elemen; indeks++) {
    x = array[indeks];
    j = indeks;
    while ((j > 0) && (array[j-1] > x)) {
      array[j] = array[j-1];
      --j;
    }
    array[j] = x;
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
  int array[10] = {4, 2, 0, 1, 3, 9, 5, 7, 8, 6};

  printf("Bentuk array sebelum diurutkan: ");
  cetak_array(array, 10);
  printf("\n");

  insertion_sort(array, 10);

  printf("Bentuk array setelah diurutkan: ");
  cetak_array(array, 10);
  printf("\n");
  
  return 0;
}

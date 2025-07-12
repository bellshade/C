/**
 * @file linear_search.c 
 * @brief implementasi algoritma linear searching.
 * 
 * file ini berisi fungsi linear search untuk mencari target Dalam array integer
 * dan fungsi untuk mencetak array. Algoritma ini cocok untuk ukuran array
 * integer yang kecil.
 * */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief mencari integer target dalam array integer menggunakan linear search.
 *
 * Fungsi ini akan mengembalikan indeks dari target yang diberikan di dalam array
 * integer tersebut. jika target tidak terdapat di dalam array, program akan
 * dihentikan
 *
 * @param array Array integer.
 * @param jumlah_element berupa integer dari array.
 * @param target berupa integer yang akan dicari di dalam array.
 *
 * @return int
 * */

int linear_search(int array[], int target, int jumlah_elemen) {
  for (int i = 0; i < jumlah_elemen; i++) {
    if (array[i] == target)
      return i;
  }
  return -1;
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
  int jumlah_elemen = 6; 
  int target = 100;
  int array[6] = { 30, 1, 91,63, 3, 32 };

  int hasil = linear_search(array, target, jumlah_elemen);  //  hasil adalah indeks dari kembalian fungsi tersebut.
  

  printf("Dalam array: ");
  cetak_array(array, jumlah_elemen);

  if (hasil < 0) {
    printf("target, yaitu %d, tidak ditemukan di dalam array\n", target);
    exit(1);
  }

  printf("target, yaitu %d, berada di indeks ke %d\n", target, hasil);

  return 0;
}

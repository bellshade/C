/**
 * @file fungsi_mutlak.c
 * @brief program sederhana untuk menghitung nilai mutlak dari bilangan bulat
 *
 * Program ini akan mengembalika nilai bukan bilangan negatif
 * terlepas dari inputnya apakah berupa bilangan positif maupun negatif
*/

#include <stdio.h>

/**
 * @brief fungsi mutlak
 * 
 * kata yang enak buat dibaca apa ya????
 *
 * @param angka
 * @return bilangan positif
 */

int fungsi_mutlak(int angka) {
  if (angka >= 0) {
    // jika angka lebih dari sama dengan 0
    // akan mengembalikan inputnya
    return angka;
  }

  return -angka;  // ini dieksekusi jika angka lebih kecil dari 0, dan mengembalikan angka dikali negatif
}

int main() {

  int angka, hasil;

  printf("Masukan angka: ");
  scanf("%d", &angka);

  hasil  = fungsi_mutlak(angka);
  printf("fungsi mutlak dari %d adalah %d\n", angka, hasil);

  return 0;
}

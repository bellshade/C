/**
 * @file fibonacci.c
 * @brief program untuk menghitung nilai fibonacci ke-n menggunakan dynamic
 * programming
 *
 * program ini menerima input lalu menghitung nilai bilangan fibonacci
 * pada posisi tertentu menggunakan pendekatan iteratif
 * dengan dynamic array
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief fungsi untuk menghitung nilai fibonacci ke-n
 *
 * @param angka posisi dalam deret fibonacci (n >= 0)
 * @return nilai fibonacci ke-angka sebagai integer
 */
int fibonacci(int angka) {
  if (angka < 0) {
    printf("angka tidak boleh negatif\n");
    exit(0);
  }

  // alokasi memori untuk array menyimpan hasil fibonacci
  int *fungsi = (int *)malloc((angka + 2) * sizeof(int));
  int i;

  // inisialisasi dua nilai awal deret fibonacci
  fungsi[0] = 0; // F(0) = 0
  fungsi[1] = 1; // F(0) = 1

  // hitung nilai fibonacci hingga indeks `angka`
  for (i = 2; i <= angka; i++) {
    fungsi[i] = fungsi[i - 1] + fungsi[i - 2]; // F(n) = F(n - 1) + F(n - 2)
  }

  int output = fungsi[angka]; // simpan hasil nilai akhir
  free(fungsi);               // free alokasi memori
  return output;
}

int main(int argc, char *argv[]) {
  int angka;

  // jika argumen terseda, maka konversikan ke integer
  if (argc == 2) {
    angka = atoi(argv[1]);
  } else {
    // jika tidak ada argumen, minta input di awal
    printf("masukkan angka dari n(angka dimulai dari 0): ");
    scanf("%d", &angka);
  }

  // tampilkan hasil perhitungan fibonacci
  printf("nilai term ke n adalah: %d\n", fibonacci(angka));
  return 0;
}

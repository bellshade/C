/**
 * @file angka_katalan.c
 * @brief program untuk menghitung angka catalan menggunakan faktorial
 *
 * program ini menerima input bilangan bulat n dari pengguna
 * lalu menghitung angka catalan ke-n dengan menggunakan rumus
 *
 * C(n) = (2n)! / ((n+1)! * n!)
 */
#include <stdio.h>

/**
 * @brief menghitung nilai faktorial dari sebuah bilangan bulat positif
 *
 * fungsi ini menerima bilangan bulat dari x dan return hasil
 * perkalian berturut-turut dari semua bilangan bulat positif dari 1 hingga ke x
 *
 * @param x bilangan bulat positif yang akan difaktorialkan
 * @return hasil faktorial dari X sebagai long int
 */
long int faktorial(int x) {
  int i;
  long int x_faktorial;

  // inisialisasi hasil dari faktorial dengan nilai awal adalah x
  x_faktorial = x;
  // looping perhitung faktorial
  for (i = 1; i < x; i++) {
    x_faktorial = x_faktorial * (x - i);
  }
  return x_faktorial;
}

int main() {
  long int f1, f2, f3; // variabel untuk menyimpan hasil faktorial
  int n;               // input dari pengguna

  float angka_katalan; // untuk menyimpan hasil akhir perhitungan
  printf("masukkan angka: ");
  scanf("%d", &n);

  // hitung komponen faktorial yang diperlukan
  f1 = faktorial(2 * n); // (2n)!
  f2 = faktorial(n + 1); // (n+1)!
  f3 = faktorial(n);     // n!

  // hitung angka catalan dengan rumus: 2(n)! / [(n+1)! *n!]
  angka_katalan = (float)f1 / (f2 * f3);

  printf("hasil: %0.2f", angka_katalan);

  return 0;
}

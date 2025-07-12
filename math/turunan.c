/*
 * file turunan.c
 * @brief program untuk menentukan nilai turunan
 * program ini menerima input n yaitu derajat suatu fungsi polynomial f(x)
 *
 * untuk menentukan nilai turunan kita memakai pendekatan limit
 * f(x)' = limit h -> 0 = f(x + h) - f(x) / h
 * karena kita tidak dapat langsung menghitung nilai limit,maka kita ganti
 * nilai h menjadi suatu nilai sangat kecil mendekati nilai 0 misal 10^5 atau
 * 1e-5
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define h 1e-5
/**
 * @brief fungsi untuk mendapatkan nilai fungsi polynomial
 *
 *
 * @param x titik dimana nilai turunan yang ingin dihitunh
 * @param result variabel untuk menyimpan nilai fungsi
 * @param koefisien sebuah array berisi koefisien fungsi
 * @return mengambalikan nilai sebuah fungsi pada titik x
 */
double fungsi(double x, int koefisien[], int pangkat) {
  // int n = sizeof(koefisien) / sizeof(koefisien[0]);
  double result = 0;
  for (size_t i = 0; i <= pangkat; i++) {
    result += koefisien[i] * pow(x, pangkat - i); //
  }
  return result;
}
/**
 * @brief fungsi untuk mencari nilai turunan
 *
 * @param x adalah titik dimana turunan ingin dihitung
 * @param turunan variabel untuk menyimpan nilai turunan
 */
void derivative(double x, int koefisien[], int pangkat) {
  double turunan;
  turunan =
      (fungsi(x + h, koefisien, pangkat) - fungsi(x, koefisien, pangkat)) / h;

  printf("turunan dari x: %lf", turunan);
}
/**
 * contoh
Input : 1x^3
x     :3
Output : 27
 */
int main() {
  int n;
  double x;
  printf("masukkan pangkat pilonomial: ");
  scanf("%d", &n);
  printf("masukkan nilai x untuk turunan: ");
  scanf("%lf", &x);

  int *koefisien = malloc((n + 1) * sizeof(int));
  if (koefisien == NULL) {
    fprintf(stderr, "alokasi memori gagal\n");
    return 1;
  }
  for (size_t i = 0; i <= n; i++) {
    // scanf("%d",koefisien[i]);
    int data;
    scanf("%d", &data);
    koefisien[i] = data; // masukkan nilai koefisien
  }
  derivative(x, koefisien, n);
  free(koefisien);
  return 0;
  // compile dengan flag -lm
}

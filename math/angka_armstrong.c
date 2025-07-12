/**
 * @file angka_armstrong.c
 * @brief program untuk memeriksa apakah suatu angka merupakan angka armstrong
 *
 * program ini menerima sebuah input berupa bilangan bulat,
 * lalu mengecek apakah angka tersebut adalah angka armstrong
 * menggunakan metode jumlah square dari masing digit
 */
#include <stdio.h>

int main() {

  int angka, hasil = 0, i, angka_armstrong;

  printf("program cek angka armstrong\n");
  printf("masukkan angka : ");
  scanf("%d", &angka);

  // simpan nilai awal angka untuk dibandingkan
  angka_armstrong = angka;

  // looping untuk memproses setiap digit dari angka
  while (angka != 0) {
    i = angka % 10;              // ambil digit terakhir
    hasil = hasil + (i * i * i); // tambahkan square digit ke hasil
    angka = angka / 10;          // hilangkan digit terakhir
  }

  if (hasil == angka_armstrong) {
    printf("%d adalah angka armstrong\n", angka_armstrong);
  } else {
    printf("%d bukan angka armstrong\n", angka_armstrong);
  }

  return 0;
}

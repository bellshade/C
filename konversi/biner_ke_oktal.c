/**
 * @file biner_ke_oktal.c
 * @brief program untuk mengkonversi biner ke angka oktal
 *
 * contoh:
 * biner:    110 111
 * desimal    6   7
 * oktal: 67
 */

#include <stdio.h>

/**
 * @brief fungsi untuk mengambil tiga digit terakhir dari bilangan desimal
 *
 * fungsi ini digunakan untuk memisah bilangan biner menjadi 3 kelompok bit
 * karena dalam konversi biner ke oktal, kita bekerja per 3 bit
 *
 * @param n bilangan bulat positif (biner) yang akan kita proses
 * @return int nilai tiga digit terakhir sebagai bilangan bulat
 */
int tiga_digit(int n) {
  // inisialisasi variabel untuk menyimpan remainder, hasil sementara
  // dan pangkat
  int d = 0, p = 1;

  // ambil 3 digit terakhir secara berturu-turut
  for (int i = 0; i < 3; i++) {
    int r = n % 10; // ambil digit terakhir
    d += r * p;     // tambahkan ke hasil dengan posisi sesuai pangkat 10
    p *= 10;        // naikkan pangkat untuk digit berikutnya
    n /= 10;        // hilangkan digit terakhir
  }

  // return tiga digit terakhir
  return d;
}

int main() {
  int angka_biner, base = 1, remainder, td, hasil = 0, ord = 1;

  printf("masukkan angka biner: ");
  scanf("%d", &angka_biner);

  // looping sampai semua digit biner telah diproses
  while (angka_biner > 0) {
    // jika lebih dari 3 digit, ambil tiga digit terakhir
    if (angka_biner > 111) {
      td = tiga_digit(angka_biner);
    } else {
      // jika kurang atau sama dengan 111, maka kita gunakan langsung
      td = angka_biner;
    }

    angka_biner /= 1000; // hapus 3 digit terakhir dari angka biner
    base = 1;            // reset variabel untuk konversi biner ke desimal
    int d = 0;

    // looping untuk konversi tiga digit biner ke desimal
    while (td > 0) {
      remainder = td % 10;     // ambil digit terakhir
      td /= 10;                // hapus digit terakhir
      d += (base * remainder); // kalikan dengan pangkat 2 sesuai posisi
      base *= 2;               // naikkan pangkat 2 untuk digit berikutnya
    }

    // simpan desimal sebagai digit oktal dengan urutan yang benar
    hasil += d * ord;
    // geser tempat oktal (puluhan, ratusan, dll)
    ord *= 10;
  }

  printf("\nNilai oktalnya adalah: %d\n", hasil);
  return 0;
}

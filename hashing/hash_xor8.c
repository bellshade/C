/**
 * @file hash_xor8.c
 * @brief program menghitung hash sederhana dari string basis xor dan modular
 *
 * program ini menghasilkan nilai hash 8-bit yang bisa digunakan
 * untuk validasi data sederhana, checksum dan lain
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief fungsi untuk menghitung hash sederhana berbasi XOR dan modular
 *
 * fungsi ini menggunakan metode
 * - menjumlahkan semua karakter dalam string
 * - menggunakan bitwise AND untuk membatasi hasil ke 8-bit (0-255)
 * - melakukan XOR dengan 0xFF lalu menambahkan 1 sebagai final step
 *
 * @param s pointer ke string ASCII null-terminated
 * @return uint8_t nilai hash hasil perhitungan
 */
uint8_t xor8(const char *s) {
  uint8_t hash = 0; // inisialisasi nilai hash awal
  size_t i = 0;     // indeks karakter dalam string

  // looping hingga akhir dari string
  while (s[i] != '\0') {
    hash = (hash + s[i]) & 0xff; // tambahin karakter ke hash, batasin ke 8 bit
    i++;                         // gesert ke karakter berikutnya
  }
  // XOR dan 0xFF kemudian tambah dengan 1, dan tetap batasi ke nilai 8-bit
  return (((hash ^ 0xff) + 1) & 0xff);
}

int main() {
  const char *kata = "bellshade C";
  printf("hasil hash: %d\n", xor8(kata));

  return 0;
}

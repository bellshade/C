/**
 * @file hash_sdbm.c
 * @brief program untuk menghitung nilai hash menggunakan algoritma SDBM
 *
 * algoritma SDBM adalah fungsi hashing yang digunakan dalam berbagai aplikasi
 * termasuk database hash seperti berkeley DB, fungsi ini dikenal karena cepat
 * dan memiliki distribusi yang cukup sangat baik untuk string biasa
 */
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief fungsi hash SDBM untuk menghasilkan nilai hash dari sebuah string
 * ASCII
 *
 * rumus hashing = s[i] + (hash << 6) + (hash << 16) - hash
 *
 * @param s pointer string ASCII null-terminated
 * @return uint64_t nilai hash perhitungan
 */
uint64_t sdbm_hash(const char *s) {
  uint64_t hash = 0; // inisialisasi nilai hash awal sebagai nol
  size_t i = 0;      // indeks karakter dalam string

  // looping hingga akhir string (null terminator '\0')
  while (s[i] != '\0') {
    // hitung hash menggunakan rumus SDBM
    // hash = s[i] + (hash << 6) + (hash << 16) - hash
    // ini akan memindahkan bit-bit hash ke kiri (shifting left) dan menambahkan
    // karakter saat ini
    hash = s[i] + (hash << 6) + (hash << 16) - hash;
    i++; // geser ke karakter berikutnya
  }
  return hash;
}

int main() {
  const char *kata = "bellshade C";
  printf("hasil hash: %" PRIu64 "\n", sdbm_hash(kata));

  return 0;
}

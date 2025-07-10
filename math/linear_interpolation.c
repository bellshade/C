/**
 * @file linear_interpolation.c
 * @brief program sederhana untuk melakukan interpolasi linear menggunakan
 * fungsi lerp
 *
 * program ini akan menghasilkan interpolasi antara nilai awal 0
 * hingga nilai akhir menggunakan algoritma linear interpolation
 */
#include <stdio.h>

/**
 * @brief fungsi lerp
 *
 * melakukan interpolasi linear dua titik k0 dan k1
 * berdasrkan parameter t (0 <= t <= 1)
 *
 * @param k0 nilai awal (start value)
 * @param k1 nilai akhir (end value)
 * @param t parameter interpolasi, biasanya dalam rentang (0, 1)
 * @return hasil interpolasi sebagai float
 */
float lerp(float k0, float k1, float t) {
  // hitung hasil interpolasi
  return k0 + t * (k1 - k0);
}

int main() {
  float start = 0, finish = 5, langkah = 0;

  printf("masukkan angka, batas linear interpolasi yang lebih besar: ");
  scanf("%f", &finish);

  printf("masukan berapa banyak yang akan dibagikan: ");
  scanf("%f", &langkah);

  // lakukan iterasi dan cetak hasil interpolasi
  for (int i = 0; i < langkah; i++) {
    // hitung nilai interpolasi antara start dan finish
    // dengan t = i / langkah -> dari 0 hingga mendekati 1
    printf("%f\n", lerp(start, finish, i / langkah));
  }

  return 0;
}

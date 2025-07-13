/**
 * @file algoritma_euclid.c
 * @brief berisi program untuk menghitung FPB dari 2 bilangan bulat
 *
 * Program ini menerima 2 input bilangan bulat lalu menghitung kedua bilangan
 * itu untuk mencari Faktor Persekutuan Terbesar dari mereka, lalu program
 * akan mengembalikan nilai FPB dari perhitungan tersebut.
 * */
#include <stdio.h>
#include <stdlib.h>

/**
 * @param int A angka pertama
 * @param int B angka kedua
 *
 * @return int hasil FPB
 * */
int euklides(int A, int B) {
  if (A < B) {
    printf("Input kedua haruslah lebih kecil atau sama dengan dari input pertama.");
    exit(1);
  }

  int reminder;
  while (B != 0) {
    reminder = A % B;
    A = B;
    B = reminder;
  }

  return A;
}

int main() {
  int A, B, hasil;

  A = 36;
  B = 6;
  
  hasil = euklides(A, B);

  printf("Faktor Persekutuan Terbesar (FPB) antara %d dan %d adalah %d\n", A, B, hasil);
}

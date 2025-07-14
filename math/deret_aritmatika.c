/**
 * @file deret_aritmatika.c
 * @brief Program untuk menentukan elemen-elemen penting dalam deret aritmatika.
 * 
 * Program ini menerima input berupa deret aritmatika dari pengguna,
 * lalu menghitung:
 * - Beda antar suku (b)
 * - Menampilkan bentuk umum suku ke-n dalam bentuk `bn + c`
 * - Menghitung nilai suku ke-n berdasarkan input pengguna
 * 
 * Program ini cocok untuk pembelajaran konsep dasar deret aritmatika dan implementasi
 * alokasi memori dinamis di C (menggunakan `malloc`).
 * 
 * @author Yusuf
 * @date Juli 2025
 */
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief fungsi untuk mendapatkan beda(b) dalam suatu deret
 * @param deret sebuah array untuk menyimpan sebuah deret aritmatika
 * @return mengembalikan nilai  selisih antara dua suku berurutan atau disebut beda(b)
 */
int beda(const int deret[]){
    return deret[1] - deret[0];
}
/**
 * @brief fungsi untuk mendapatkan nilai suku pertama 
 * 
 * @param deret sebuah array untuk menyimpan sebuah deret aritmatika
 * @return mengambalikan nilai suku pertama
 */
int suku_pertama(const int deret[]){
    return deret[0];
}
/**
 * @brief fungsi untuk mendapatkan rumus suku ke n
 * @param deret sebuah array untuk menyimpan sebuah deret aritmatika
 * @details rumus suku ke n didefinisikan:
 * suku_ke_n = a + (n - 1) x b
 * dengan 
 * n = suku ke n 
 * a = suku pertama
 * b = beda
 */
void r_suku_n(const int deret[]){
    int a = suku_pertama(deret);
    int b = beda(deret);
    int sm = a + (b * -1);
    printf("%dn + %d", b, sm);
}
/**
 * @brief fungsi yang digunakan untuk mendapatkan nilai suku ke-n
 * @param deret sebuah array untuk menyimpan sebuah deret aritmatika di heap memory
 * @param x indeks suku ke-n
 * @param n banyak element dalam satu deret/besar dari sebuah array
 * @return nilai deret suku ke n
 * @details rumus suku ke n didefinisikan:
 * suku_ke_n = a + (n - 1) x b
 * dengan 
 * n = suku ke n 
 * a = suku pertama
 * b = beda
 */
int suku_n(const int deret[],int x,int n){
    if(x <= n && x >= 0){
        return deret[x];
    }
    int a = suku_pertama(deret);
    int b = beda(deret);
    int deno = (x - 1) * b;
    int ans;
    ans = a + deno;
    return ans;
}
/**
 * @brief Fungsi utama program.
 * 
 * Fungsi ini:
 * 1. Meminta pengguna untuk memasukkan jumlah elemen deret.
 * 2. Mengalokasikan memori untuk menyimpan deret aritmatika.
 * 3. Meminta input elemen-elemen deret dari pengguna.
 * 4. Menghitung dan menampilkan beda antar suku.
 * 5. Menampilkan bentuk umum suku ke-n (`bn + c`).
 * 6. Meminta input suku ke-n yang ingin dicari, lalu menghitung dan menampilkan nilainya.
 * 
 * Program mengelola memori secara manual menggunakan `malloc` dan `free`.
 * 
 * @return 0 jika program berhasil dijalankan.
 */
int main(){
    int n; //banyak element
    printf("Masukkan panjang deret");
    scanf("%d",&n);
    int* deret = malloc((n) * sizeof(int));//mengalokasi memory di heap sebanyak n * 4 byte
    for(size_t i = 0;i < n;i++){
        printf("Masukkan data: ");
        scanf("%d",&deret[i]);
    }
    int result_beda = beda(deret);
    printf("beda sebuah deret: %d\n",result_beda);
    printf("\n");
    r_suku_n(deret);
    printf("\n");
    int x;
    printf("masukkan suku ke n: ");
    scanf("%d",&x);
    int result_sukun = suku_n(deret,x,n);
    printf("\n");
    printf("nilai suku ke-n %d",result_sukun);
    free(deret);
    return 0;
}
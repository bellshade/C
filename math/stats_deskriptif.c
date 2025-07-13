/**
 * @file stats.deskriptif.c
 * @brief Menghitung ukuran pemusatan dan penyebaran data (statistik deskriptif)
 * 
 * File ini berisi implementasi fungsi-fungsi statistik deskriptif untuk data tunggal,
 * termasuk:
 * - ukuran pemusatan data: rata-rata (mean), median, dan modus
 * - ukuran penyebaran data: varians, simpangan baku, dan jangkauan
 * 
 * Program ini menerima input data dari pengguna melalui terminal, lalu menghitung dan 
 * menampilkan hasil analisis statistik tersebut.
 * 
 * @author 
 * Muh Yusuf
 * 
 * @date 
 * Juli 2025
 * 
 * @note 
 * Gunakan `qsort()` dari `<stdlib.h>` untuk pengurutan array sebelum menghitung median.
 * compile menggunakan flag -lm untuk load stl math.h
 * example:clang stats.deskriptif.c -o program -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
 /**
 * @brief Fungsi pembanding untuk digunakan dalam qsort()
 *
 * Fungsi ini digunakan untuk membandingkan dua elemen bertipe `int`
 * dalam proses pengurutan menggunakan `qsort()`. Fungsi ini
 * mengembalikan:
 * - nilai negatif jika `*a < *b`
 * - nol jika `*a == *b`
 * - nilai positif jika `*a > *b`
 *
 * @param a Pointer ke elemen pertama yang akan dibandingkan
 * @param b Pointer ke elemen kedua yang akan dibandingkan
 * @return int
 * Nilai negatif, nol, atau positif berdasarkan hasil perbandingan dua elemen.
 */
int sort(const void* a,const void* b){ //sort using qsort
    return (*(int*)a - *(int*)b);
}
/**
 * @brief fungsi untuk mengkalkulasikan nilai rata-rata dari data dengan rumus:
 * rata-rata = jumlah dari semua data / banyak data
 * 
 * @param arr array berisi kumpulan data
 * @param n banyak element dalam suatu data
 * @return rata-rata dari sebuah data
 * @details Rata-rata adalah ukuran pemusatan yang diperoleh dengan 
 * menjumlahkan semua data lalu membaginya dengan jumlah data. 
 * Nilai ini digunakan untuk merepresentasikan keseluruhan kumpulan data 
 * dengan satu angka yang mewakili.
 */
int avg(int arr[],int n){
    int sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += arr[i];
    }
    int rata_rata = sum / n;
    return rata_rata;
}
/**
 * @brief fungsi untuk mendapatkan nilai median atau nilai tengah dari sebuah data
 * @param arr array berisi kumpulan data
 * @param n banyak element dalam suatu data
 * @return mengembalikan nilai median sebuah data
 * @details median membagi data menjadi dua bagian sama besar
 *  jika data ganjil mengembalikan nilai data pada posisi n/2
 *  jika data genap mengambilkan nilai data pada posisi  (n / 2 - 1) + (n/2 ) / 2
 */
int median(int arr[],int n){
    //urutkan data  
    qsort(arr,n,sizeof(int),sort); //using qsort dari stl stdlib.h

    if(n & 1){//jika banyak data berjumlah ganjil maka data persis berada di n / 2 + 1
        //karena array memakai 0 based indexed maka data dikurangi 1
        return arr[n / 2];
    }
    /*
    untuk banyak data berjumlah genap,maka tidak ada data yang benar-benar ditengah
    maka median adalah rata-rata dari 2 nilai tengah yaitu
    *element n / 2 - 1
    *element ke n/2 
    *median = (n / 2 - 1) + (n/2 ) / 2
    *catatan:rumus diatas hanya untuk 0 based indexed,jika kamu memakai 1-indexed
    untuk mencari median maka anda dapat menambah 1 setiap nilai tengah
    *element ke n/2 - 1 + 1 -> n / 2
    *element ke n / 2 + 1 ->n/ 2 + 1
    */
    return (arr[n/2 - 1] + arr[n/2])/2.0;
}
/**
 * @brief fungsi untuk mendapatkan nilai modus atau  
 * kemunculan data terbanyak dalam suatu kumpulan data
 * @param arr array berisi kumpulan data
 * @param n banyak element dalam suatu data
 * @details
 * 
 * @return nilai modus atau frekuensi terbanyak dalam array
 */
int modus(int arr[],int n,int mx){
    int *freq = malloc((mx + 1) * sizeof(int));

    for(size_t i = 0;i < n;i++){
        freq[arr[i]]++;
    }
    int Modus = -1;
    for(int i = 0;i <= mx;i++){
        if(freq[i] > 0 && freq[i] > Modus){
            Modus = i;
        }
    }   
    free(freq);
    return Modus;
}
/**
 * @brief Menghitung nilai varians dari data tunggal.
 *
 * Varians merupakan ukuran penyebaran data terhadap nilai rata-ratanya.
 *
 * @param arr array berisi data
 * @param n jumlah elemen dalam array
 * @return varians (populasi) dari kumpulan data
 */
double varians(int arr[],int n){
    int rata_rata = avg(arr,n);
    double denominator = 0;

    for(size_t i = 0;i < n;i++){
        double ans = pow(arr[i] - rata_rata,2);
        denominator += ans;
    }
    return denominator / n;
}
/**
 * @brief Menghitung simpangan baku dari data tunggal.
 *
 * Simpangan baku adalah akar dari varians dan menunjukkan seberapa jauh nilai data dari rata-ratanya.
 *
 * @param arr array berisi data
 * @param n jumlah elemen dalam array
 * @return nilai simpangan baku dari data
 */
double simpangan_baku(int arr[],int n){
    double var = varians(arr,n);
    return sqrt(var);
}
/**
 * @brief Menghitung jangkauan dari kumpulan data.
 *
 * Jangkauan adalah selisih antara nilai maksimum dan minimum.
 *
 * @param arr array berisi data
 * @param n jumlah elemen dalam array
 * @return nilai jangkauan data
 */
int jangkauan(int arr[],int n){
    int mx = -1;
    int mn = INT_MAX;
    for(size_t i = 0;i < n;i++){
        if(arr[i] > mx){
            mx = arr[i];
        }else{
            mn = arr[i];
        }
    }

    return abs(mx - mn);
}
/**
 * @brief Fungsi utama program.
 *
 * Program akan meminta input dari pengguna berupa panjang data dan nilai-nilai data,
 * lalu menghitung dan menampilkan rata-rata, median, dan modus dari data tersebut.
 *
 * @return 0 jika program berjalan dengan sukses, 1 jika terjadi kegagalan alokasi memori
 */
int main(){
    int n;
    printf("Masukkan panjang data: ");
    scanf("%d",&n);
    int mx = -1;
    int *arr = malloc((n) * sizeof(int)); //mengalokasi blok memory sebanyal n * 4 byte(int)
    if(arr == NULL){
        fprintf(stderr,"alokasi memory gagal \n");
        return 1;
    }
    for(size_t i = 0;i < n;i++){
        printf("masukkan data: ");
        scanf("%d",&arr[i]);
        if(arr[i] > mx){
            mx = arr[i];
        }
    }
    int result_median = median(arr,n);
    int result_avg = avg(arr,n);
    int result_modus = modus(arr,n,mx);
    int result_jangkauan = jangkauan(arr,n);
    double result_varians = varians(arr,n);
    double result_simpangan = simpangan_baku(arr,n);
    printf("rata-rata dari data: %d\n",result_avg);
    printf("median dari data: %d\n",result_median);
    printf("modus dari data: %d\n",result_modus);
    printf("jangkauan dari data: %d\n",result_jangkauan);
    printf("Varians dari data: %lf\n",result_varians);
    printf("Simpangan baku dari data: %lf\n",result_simpangan);
    /*
    contoh test case yang dapat anda pakai
    n = 7
    arr = [1, 2, 2, 3, 3, 4, 5] 
    */
    return 0;
}
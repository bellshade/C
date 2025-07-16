/**
* @file linear_regression.c
*
*@brief Implementasi regresi linear sederhana dan perhitungan koefisien determinasi (R²).
*
* Fungsi ini menerima dua array data (independen dan dependen) dan menghitung parameter model regresi linear,
*
* termasuk slope (b1), intercept (b0), dan koefisien determinasi (R²) untuk mengevaluasi seberapa baik model
*
menjelaskan variasi data.
*
* @author Yusuf
*
* @date Juli 2025
* @details sumber:https://mathcyber1997.com/regresi-linear-sederhana/
*/
#include <stdio.h>
#include <stdlib.h>
/**
 * @brief Menghitung jumlah seluruh elemen dalam array X.
 *
 * @param data_x Array berisi nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return Jumlah total nilai dari semua elemen di data_x
 */ 
double sum_x(double data_x[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_x[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah seluruh elemen dalam array Y.
 *
 * @param data_y Array berisi nilai variabel dependen (Y)
 * @param n Jumlah elemen dalam array
 * @return Jumlah total nilai dari semua elemen di data_y
 */
double sum_y(double data_y[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_y[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah kuadrat dari setiap elemen dalam array X.
 *
 * @param data_x Array berisi nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return Jumlah dari X₁² + X₂² + ... + Xₙ²
 */
double square_x(double data_x[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        int square = data_x[i] * data_x[i];
        sum += square;
    }
    return sum;
}
/**
 * @brief Menghitung jumlah kuadrat dari setiap elemen dalam array Y.
 *
 * @param data_y Array berisi nilai variabel dependen (Y)
 * @param n Jumlah elemen dalam array
 * @return Jumlah dari Y₁² + Y₂² + ... + Yₙ²
 */
double square_y(double data_y[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        double square = data_y[i] * data_y[i];
        sum += square;
    }
    return sum;
}
/**
 * @brief Menghitung jumlah hasil perkalian elemen yang bersesuaian dari dua array (X * Y).
 *
 * @param data_x Array berisi nilai variabel independen (X)
 * @param data_y Array berisi nilai variabel dependen (Y)
 * @param n Jumlah elemen dalam array (panjang data_x dan data_y harus sama)
 * @return Jumlah dari X₁×Y₁ + X₂×Y₂ + ... + Xₙ×Yₙ
 */
double multiplication(double data_x[],double data_y[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_x[i] * data_y[i];
    }
    return sum;
}
/**
 * @brief Menghitung model regresi linear sederhana berdasarkan data input X dan Y.
 *
 * Fungsi ini akan:
 * - Menghitung jumlah total dan kuadrat X dan Y
 * - Menghitung slope dan intercept dari garis regresi linear
 * - Menghitung koefisien determinasi R²
 * - Menampilkan hasil perhitungan secara detail
 *
 * @param data_x Array berisi nilai variabel independen (X)
 * @param data_y Array berisi nilai variabel dependen (Y)
 * @param n Jumlah data dalam array
 *
 * @note Asumsinya adalah panjang `data_x` dan `data_y` sama dan minimal dua elemen.
 */
void linear_regression(double data_x[],double data_y[],int n){
   //Hitung jumlah total X dan Y
   double Sum_X = sum_x(data_x, n);
   double Sum_Y = sum_y(data_y, n);
   //Hitung jumlah kuadrat X dan Y
   double SquareX = square_x(data_x, n);
   double SquareY = square_y(data_y, n);
   //Hitung jumlah hasil perkalian X dan Y
   double Multiplication = multiplication(data_x, data_y, n);
   //Hitung jumlah kuadrat silang (JKxy)
   double Jkxy = Multiplication - ((Sum_X * Sum_Y) / (double)n);
   // Hitung jumlah kuadrat X (JKxx)
   double Jkxx = SquareX - ((Sum_X * Sum_X) / (double)n);
   // Hitung slope (gradien/kemiringan) dari regresi linear
   double b1 = Jkxx != 0 ? (Jkxy / Jkxx) : 0;  // Hindari pembagian nol
   // Hitung intercept (titik potong sumbu Y)
   double b0 = (Sum_Y - (b1 * Sum_X)) / (double)n;
   // Hitung JKyy untuk perhitungan koefisien determinasi
   double JKyy = SquareY - ((Sum_Y * Sum_Y) / (double)n);
   // Hitung koefisien determinasi R^2
   double R_Squared = (JKyy != 0) ? ((b1 * Jkxy) / JKyy) : 0;
   // 10. Konversi R^2 ke persen
   double persen = R_Squared * 100;
   printf("=== DETAIL PERHITUNGAN REGRESI LINEAR ===\n");
   printf("Sum X           = %.2f\n", Sum_X);
   printf("Sum Y           = %.2f\n", Sum_Y);
   printf("Sum X^2         = %.2f\n", SquareX);
   printf("Sum Y^2         = %.2f\n", SquareY);
   printf("Sum X * Y       = %.2f\n", Multiplication);
   printf("JKxy            = %.2f\n", Jkxy);
   printf("JKxx            = %.2f\n", Jkxx);
   printf("Slope (b1)      = %.2f\n", b1);
   printf("Intercept (b0)  = %.2f\n", b0);
   printf("Model regresi   : y = %.6f + %.6fx\n", b0, b1);
   printf("R^2             = %.2f\n", R_Squared);
   printf("Model memprediksi sebesar %.2f%% dari variasi variabel Y\n", persen);
}

int main(){
    int n;
    printf("masukkan panjang data: ");
    scanf("%d",&n);
    double *data_x = malloc((n) * sizeof(double));
    double *data_y = malloc((n) * sizeof(double));
    if (data_x == NULL || data_y == NULL) {
        printf("Alokasi memori gagal\n");
        return 1;
    }
    for(size_t i = 0;i < n;i++){
        printf("masukkan data independen: ");
        scanf("%lf",&data_x[i]);
        printf("masukkan data dependen: ");
        scanf("%lf",&data_y[i]);
    }
    linear_regression(data_x,data_y,n);
    free(data_x);
    free(data_y);
    return 0;
}
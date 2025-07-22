/**
 * @file quadratic.c
 * @brief Implementasi regresi kuadratik (polinomial derajat 2) untuk mencari
 *        model terbaik dari hubungan antara variabel independen (x) dan dependen (y).
 * 
 *        Fungsi utama akan menghitung parameter regresi kuadratik berupa:
 *        - a: koefisien dari x²
 *        - b: koefisien dari x
 *        - c: konstanta (intercept)
 *        
 *        Model akhir berbentuk: y = ax² + bx + c
 *        Perhitungan dilakukan menggunakan metode least squares tanpa library eksternal.
 * 
 * @author Yusuf
 * @date Juli 2025
 * @details sumber: https://www.varsitytutors.com/hotmath/hotmath_help/topics/quadratic-regression
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/**
 * @brief Menghitung jumlah kuadrat dari setiap elemen dalam array X.
 * 
 * @param data_x Array berisi nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return double Hasil dari Σx² (jumlah kuadrat X)
 */
double x_squared(double data_x[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_x[i] * data_x[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah seluruh elemen dalam array X.
 * 
 * @param data_x Array berisi nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return double Hasil dari Σx (jumlah semua nilai X)
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
 * @return double Hasil dari Σy (jumlah semua nilai Y)
 */
double sum_y(double data_y[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_y[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah pangkat tiga dari setiap elemen dalam array X.
 * 
 * @param data_x Array nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return double Hasil dari Σx³ (jumlah kubik X)
 */
double x_multi_y(double data_x[],double data_y[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_x[i] * data_y[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah pangkat tiga dari setiap elemen dalam array X.
 * 
 * @param data_x Array nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return double Hasil dari Σx³ (jumlah kubik X)
 */
double x_cubic(double data_x[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_x[i] * data_x[i] * data_x[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah pangkat empat dari setiap elemen dalam array X.
 * 
 * @param data_x Array nilai variabel independen (X)
 * @param n Jumlah elemen dalam array
 * @return double Hasil dari Σx⁴ (jumlah bi-kuadrat X)
 */
double x_biquadratic(double data_x[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += data_x[i] * data_x[i] * data_x[i] * data_x[i];
    }
    return sum;
}
/**
 * @brief Menghitung jumlah hasil perkalian x² dengan y dari setiap elemen.
 * 
 * @param data_x Array nilai variabel independen (X)
 * @param data_y Array nilai variabel dependen (Y)
 * @param n Jumlah elemen dalam array
 * @return double Hasil dari Σ(x² * y)
 */
double x_squared_multi_y(double data_x[],double data_y[],int n){
    double sum = 0;
    for(size_t i = 0;i < n;i++){
        sum += (data_x[i] * data_x[i]) * data_y[i];
    }
    return sum;
}
void quadratic_regression(double data_x[], double data_y[], int n) {
    double a, b, c;
    // Hitung semua jumlah yang dibutuhkan
    double Sum_x = sum_x(data_x, n);                        // Σx
    double Sum_y = sum_y(data_y, n);                        // Σy
    double sum_x2 = x_squared(data_x, n);                   // Σx²
    double sum_x3 = x_cubic(data_x, n);                     // Σx³
    double sum_x4 = x_biquadratic(data_x, n);               // Σx⁴
    double sum_xy = x_multi_y(data_x, data_y, n);           // Σxy
    double sum_x2y = x_squared_multi_y(data_x, data_y, n);  // Σx²y
    // Denominator bersama
    double denom = (sum_x4 * sum_x2) - (sum_x3 * sum_x3);
    // Periksa apakah denominator sangat kecil (hindari pembagian nol)
    if (fabs(denom) < 1e-9) {
        printf("Error: Denominator terlalu kecil, tidak dapat menghitung regresi.\n");
        return;
    }
    // Hitung a, b, dan c
    a = (sum_x2y * sum_x2 - sum_xy * sum_x3) / denom;
    b = (sum_xy * sum_x4 - sum_x2y * sum_x3) / denom;
    c = (Sum_y / n) - (b * (Sum_x / n)) - (a * (sum_x2 / n));
    // Tampilkan hasil
    printf("=== HASIL REGRESI KUADRATIK ===\n");
    printf("Jumlah x      : %.4f\n", Sum_x);
    printf("Jumlah y      : %.4f\n", Sum_y);
    printf("Jumlah x^2    : %.4f\n", sum_x2);
    printf("Jumlah x^3    : %.4f\n", sum_x3);
    printf("Jumlah x^4    : %.4f\n", sum_x4);
    printf("Jumlah xy     : %.4f\n", sum_xy);
    printf("Jumlah x^2y   : %.4f\n", sum_x2y);
    printf("a (x^2)       : %.6f\n", a);
    printf("b (x)         : %.6f\n", b);
    printf("c (konstanta) : %.6f\n", c);
    printf("Model regresi : y = %.6f x^2 + %.6f x + %.6f\n", a, b, c);
}
int main(){
    double data_x[10] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4};
    double data_y[10] = {50, 37, 26, 17, 10, 5, 10, 17, 26, 37};
    int n = 10; //panjang data
    quadratic_regression(data_x,data_y,n);
    return 0;  
}
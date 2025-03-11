/* main_sorting.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void (*SortFunc)(void*, size_t, size_t, int (*)(const void*, const void*));

/* Troca dois elementos de tamanho 'size' */
void swap(void *a, void *b, size_t size) {
    char *temp = malloc(size);
    if (!temp) { perror("malloc"); exit(EXIT_FAILURE); }
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

/* Bubble Sort (O(n^2)) */
void bubbleSort(void *base, size_t n, size_t size, int (*cmp)(const void*, const void*)) {
    char *arr = (char*)base;
    for (size_t i = 0; i < n - 1; i++)
        for (size_t j = 0; j < n - i - 1; j++)
            if (cmp(arr + j * size, arr + (j + 1) * size) > 0)
                swap(arr + j * size, arr + (j + 1) * size, size);
}

/* Merge Sort (O(n log n)) */
void merge(void *base, void *temp, size_t left, size_t mid, size_t right, size_t size,
           int (*cmp)(const void*, const void*)) {
    char *arr = (char*)base, *aux = (char*)temp;
    size_t i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (cmp(arr + i * size, arr + j * size) <= 0)
            memcpy(aux + k++ * size, arr + i++ * size, size);
        else
            memcpy(aux + k++ * size, arr + j++ * size, size);
    }
    while (i <= mid) { memcpy(aux + k++ * size, arr + i++ * size, size); }
    while (j <= right) { memcpy(aux + k++ * size, arr + j++ * size, size); }
    for (i = left; i <= right; i++) memcpy(arr + i * size, aux + i * size, size);
}
void mergeSortRecursive(void *base, void *temp, size_t left, size_t right, size_t size,
                        int (*cmp)(const void*, const void*)) {
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    mergeSortRecursive(base, temp, left, mid, size, cmp);
    mergeSortRecursive(base, temp, mid + 1, right, size, cmp);
    merge(base, temp, left, mid, right, size, cmp);
}
void mergeSort(void *base, size_t n, size_t size, int (*cmp)(const void*, const void*)) {
    void *temp = malloc(n * size);
    if (!temp) { perror("malloc"); exit(EXIT_FAILURE); }
    mergeSortRecursive(base, temp, 0, n - 1, size, cmp);
    free(temp);
}

/* Shell Sort (com gap sequence n/2, n/4, ...; comportamento médio ~ O(n^(3/2))) */
void shellSort(void *base, size_t n, size_t size, int (*cmp)(const void*, const void*)) {
    char *arr = (char*)base;
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < n; i++) {
            char temp[size];
            memcpy(temp, arr + i * size, size);
            size_t j = i;
            while (j >= gap && cmp(arr + (j - gap) * size, temp) > 0) {
                memcpy(arr + j * size, arr + (j - gap) * size, size);
                j -= gap;
            }
            memcpy(arr + j * size, temp, size);
        }
    }
}

/* Funções de comparação */
int cmpInt(const void *a, const void *b) {
    int ia = *(const int*)a, ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}
int cmpFloat(const void *a, const void *b) {
    float fa = *(const float*)a, fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}
int cmpDouble(const void *a, const void *b) {
    double da = *(const double*)a, db = *(const double*)b;
    return (da > db) - (da < db);
}
int cmpStr(const void *a, const void *b) {
    const char *sa = *(const char**)a, *sb = *(const char**)b;
    return strcmp(sa, sb);
}

/* Funções para impressão */
void printIntArray(int *arr, int n) { for (int i = 0; i < n; i++) printf("%d ", arr[i]); printf("\n"); }
void printFloatArray(float *arr, int n) { for (int i = 0; i < n; i++) printf("%.2f ", arr[i]); printf("\n"); }
void printDoubleArray(double *arr, int n) { for (int i = 0; i < n; i++) printf("%.5lf ", arr[i]); printf("\n"); }
void printStrArray(char **arr, int n) { for (int i = 0; i < n; i++) printf("%s ", arr[i]); printf("\n"); }

/* Gera array de inteiros */
int* genIntArray(int n, int ordered) {
    int *arr = malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }
    if (ordered) for (int i = 0; i < n; i++) arr[i] = i + 1;
    else for (int i = 0; i < n; i++) arr[i] = rand() % 100;
    return arr;
}
/* Gera array de floats */
float* genFloatArray(int n, int ordered) {
    float *arr = malloc(n * sizeof(float));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }
    if (ordered) for (int i = 0; i < n; i++) arr[i] = (float)(i + 1) / 10.0f;
    else for (int i = 0; i < n; i++) arr[i] = ((float)rand() / RAND_MAX) * 100.0f;
    return arr;
}
/* Gera array de doubles */
double* genDoubleArray(int n, int ordered) {
    double *arr = malloc(n * sizeof(double));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }
    if (ordered) for (int i = 0; i < n; i++) arr[i] = (double)(i + 1) / 10.0;
    else for (int i = 0; i < n; i++) arr[i] = ((double)rand() / RAND_MAX) * 100.0;
    return arr;
}
/* Gera uma string aleatória de tamanho 5 */
char* randString() {
    char *s = malloc(6 * sizeof(char));
    if (!s) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < 5; i++) s[i] = 'a' + rand() % 26;
    s[5] = '\0';
    return s;
}
/* Gera array de strings */
char** genStrArray(int n, int ordered) {
    char **arr = malloc(n * sizeof(char*));
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++) arr[i] = randString();
    if (ordered) qsort(arr, n, sizeof(char*), cmpStr);
    return arr;
}

/* Main: argv = <algoritmo> <tipo> <tamanho> <ordenado> */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <algoritmo> <tipo> <tamanho> <ordenado>\n", argv[0]);
        fprintf(stderr, "algoritmo: 1-Bubble, 2-Merge, 3-Shell\n");
        fprintf(stderr, "tipo: 1-int, 2-float, 3-double, 4-string\n");
        fprintf(stderr, "tamanho: inteiro\n");
        fprintf(stderr, "ordenado: 0-desordenado, 1-ordenado\n");
        return EXIT_FAILURE;
    }
    int alg = atoi(argv[1]), type = atoi(argv[2]), size = atoi(argv[3]), ordered = atoi(argv[4]);
    
    SortFunc sortFunc = NULL;
    if (alg == 1) sortFunc = bubbleSort;
    else if (alg == 2) sortFunc = mergeSort;
    else if (alg == 3) sortFunc = shellSort;
    else { fprintf(stderr, "Algoritmo inválido.\n"); return EXIT_FAILURE; }
    
    srand((unsigned int)time(NULL));
    
    switch (type) {
        case 1: {
            int *arr = genIntArray(size, ordered);
            printIntArray(arr, size);
            sortFunc(arr, size, sizeof(int), cmpInt);
            printIntArray(arr, size);
            free(arr);
            break;
        }
        case 2: {
            float *arr = genFloatArray(size, ordered);
            printFloatArray(arr, size);
            sortFunc(arr, size, sizeof(float), cmpFloat);
            printFloatArray(arr, size);
            free(arr);
            break;
        }
        case 3: {
            double *arr = genDoubleArray(size, ordered);
            printDoubleArray(arr, size);
            sortFunc(arr, size, sizeof(double), cmpDouble);
            printDoubleArray(arr, size);
            free(arr);
            break;
        }
        case 4: {
            char **arr = genStrArray(size, ordered);
            printStrArray(arr, size);
            sortFunc(arr, size, sizeof(char*), cmpStr);
            printStrArray(arr, size);
            for (int i = 0; i < size; i++) free(arr[i]);
            free(arr);
            break;
        }
        default:
            fprintf(stderr, "Tipo inválido.\n");
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

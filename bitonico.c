#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE1 100
#define SIZE2 1000
#define SIZE3 10000
#define SIZE4 100000
#define SIZE5 1000000
#define SIZE6 10000000

// Prototipação
void generate_bitonic_array(int arr[], int size);
int bitonico(int arr[], int low, int high);

int main() {
    int *arr;
    clock_t start, end;
    double cpu_time_used;

    int sizes[] = {SIZE1, SIZE2, SIZE3, SIZE4, SIZE5, SIZE6};
    int num_tests = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_tests; i++) {
        int size = sizes[i];
        arr = (int *)malloc(size * sizeof(int));
        if (arr == NULL) {
            printf("Erro de alocação de memória para size = %d\n", size);
            continue;
        }

        generate_bitonic_array(arr, size);

        start = clock();
        int max = bitonico(arr, 0, size - 1);
        end = clock();

        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Tamanho: %d | Máximo: %d | Tempo: %.6f segundos\n", size, max, cpu_time_used);

        free(arr);
    }

    return 0;
}

// Geração do array bitônico
void generate_bitonic_array(int arr[], int size) {
    int peak_index = rand() % (size - 2) + 1; // pico entre 1 e size-2
    int current = rand() % 10;

    for (int i = 0; i <= peak_index; i++) {
        arr[i] = current;
        current += rand() % 5 + 1;
    }

    for (int i = peak_index + 1; i < size; i++) {
        current -= rand() % 5 + 1;
        arr[i] = current;
    }
}

// Busca binária recursiva segura para encontrar o máximo em array bitônico
int bitonico(int arr[], int i, int f) {
    // Um elemento
    if (i == f) return arr[i];
    // Dois elementos
    if (f == i + 1) return (arr[i] > arr[f]) ? arr[i] : arr[f];

    // Meio
    int m = i + (f - i) / 2;
    // Verifica se é maior que os dois lados
    if (arr[m] > arr[m - 1] && arr[m] > arr[m + 1]) {
        // Achou
        return arr[m];
    // Verifica se é maior que a esquerda
    } else if (arr[m] > arr[m - 1]) {
        // Vai mais a direita
        return bitonico(arr, m + 1, f);
    // Não é maior que nada
    } else {
        // Vai mais a esquerda
        return bitonico(arr, i, m - 1);
    }
}

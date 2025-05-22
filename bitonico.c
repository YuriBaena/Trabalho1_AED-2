#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
long long tempo_ns() {
    LARGE_INTEGER freq, now;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&now);
    return (now.QuadPart * 1000000000LL) / freq.QuadPart;
}
#else
#include <time.h>
long long tempo_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}
#endif

#define SIZE1 10000
#define SIZE2 100000
#define SIZE3 1000000
#define SIZE4 10000000
#define SIZE5 100000000
#define SIZE6 1000000000

// Prototipação
void imprime(int arr[], int size);
void generate_bitonic_array(int arr[], int size);
int bitonico(int arr[], int low, int high);
int bitonico_iterativo(int arr[], int n);

int main() {
	srand(time(NULL));
    int *arr;
    
   	long long start_ns, end_ns;
    long long tempo_rec, tempo_it;

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

        start_ns = tempo_ns();
        int max = bitonico(arr, 0, size - 1);
        end_ns = tempo_ns();
        tempo_rec = end_ns - start_ns;
        
        start_ns = tempo_ns();
        int max2 = bitonico_iterativo(arr, size);
        end_ns = tempo_ns();
        tempo_it = end_ns - start_ns;
		
		printf("Tamanho: %d\n", size);
        printf("Maximo (recursivo): %d | Tempo: %.1f microssegundos\n", max, tempo_rec / 1000.0);
		printf("Maximo (iterativo): %d | Tempo: %.1f microssegundos\n", max2, tempo_it / 1000.0);

		
		printf("\n");	
		
        free(arr);
    }

    return 0;
}

void imprime(int arr[], int size){
	for(int i = 0; i < size; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// Geração do array bitônico
void generate_bitonic_array(int arr[], int size) {
    int peak_index = rand() % (size - 2) + 1; // índice do pico entre 1 e size - 2

    int val = rand() % 10;

    // Parte crescente
    for (int i = 0; i <= peak_index; i++) {
        arr[i] = val;
        val += rand() % 5 + 1;  // sempre aumentando
    }

    // Parte decrescente
    for (int i = peak_index + 1; i < size; i++) {
        val -= rand() % 5 + 1;
        if (val < 0) val = 0; // evitar negativos
        arr[i] = val;
    }
	
	
    //aqui garante que o pico é o maior
    if (arr[peak_index] <= arr[peak_index - 1]) {
        arr[peak_index] = arr[peak_index - 1] + 1;
    }
    if (arr[peak_index] <= arr[peak_index + 1]) {
        arr[peak_index] = arr[peak_index + 1] + 1;
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

int bitonico_iterativo(int arr[], int n) {
    int esq = 0;
    int dir = n - 1;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;

        //Esses dois if's verificam as bordas
        if (meio == 0) {
            return (arr[0] > arr[1]) ? arr[0] : arr[1];
        }
        if (meio == n - 1) {
            return (arr[n - 1] > arr[n - 2]) ? arr[n - 1] : arr[n - 2];
        }
		
		//esse if verifica se o maximo esta no meio
        if (arr[meio] > arr[meio - 1] && arr[meio] > arr[meio + 1]) {
            return arr[meio];
        } 
        //esse verifica se o maximo esta na parte da direita
		else if (arr[meio] > arr[meio - 1]) {
            esq = meio + 1;
        } 
        //aqui verifica se o maximo esta na parte da esquerda
		else {
            dir = meio - 1;
        }
    }

    return -1; //não deveria acontecer
}

#include "ordenacao.h"

// Antes de cada passagem, as posições de 0 até i - 1 já estão em ordem.
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int k = arr[i];
        int j = i - 1;

        // Abre espaço para k, deslocando os números maiores para a direita.
        while (j >= 0 && arr[j] > k) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = k;
    }
}

#include "ordenacao.h"

static void trocar(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Com os dois ramos já organizados, coloca o maior dos três na posição do pai.
static void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        trocar(arr, i, largest);
        heapify(arr, n, largest); // Acompanha o número que desceu.
    }
}

void heapSort(int arr[], int n) {
    // Começa no último pai e termina na raiz, onde fica o maior número.
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // O maior vai para o fim. Daqui em diante só as posições antes de i participam.
    for (int i = n - 1; i > 0; i--) {
        trocar(arr, 0, i);
        heapify(arr, i, 0);
    }
}

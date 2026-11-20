#include "ordenacao.h"

// Escolhe o menor número que sobrou e coloca na próxima posição do resultado.

void selectionSort(int numeros[], int n) { // Recebe o vetor e sua quantidade de elementos.
    for (int i = 0; i < n - 1; i++) { // Escolhe a proxima posicao que recebera o menor numero restante.
        int menor = i; // Assume que o primeiro elemento restante e o menor.
        for (int j = i + 1; j < n; j++) { // Procura um numero menor nas posicoes seguintes.
            if (numeros[j] < numeros[menor]) { // Verifica se o candidato atual e menor que o anterior.
                menor = j; // Guarda o indice do novo menor numero.
            }
        }
        int auxiliar = numeros[i]; // Preserva o numero que ocupa a posicao atual.
        numeros[i] = numeros[menor]; // Coloca o menor numero na posicao definitiva.
        numeros[menor] = auxiliar; // Coloca o numero preservado na antiga posicao do menor.
    }
}

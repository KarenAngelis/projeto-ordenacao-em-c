#include "ordenacao.h"

// A cada passagem, o maior número do trecho chega ao fim pelas trocas entre vizinhos.

void bubbleSort(int numeros[], int n) { // Recebe o vetor e a quantidade de elementos que devem ser ordenados.
    for (int i = 0; i < n - 1; i++) { // Faz as passagens que levam os maiores numeros ao final.
        for (int j = 0; j < n - 1 - i; j++) { // Compara vizinhos sem repetir a parte final ja ordenada.
            if (numeros[j] > numeros[j + 1]) { // Verifica se o numero da esquerda e maior que o da direita.
                int auxiliar = numeros[j]; // Guarda o primeiro valor antes de fazer a troca.
                numeros[j] = numeros[j + 1]; // Move o menor numero para a esquerda.
                numeros[j + 1] = auxiliar; // Move o maior numero para a direita.
            }
        }
    }
}

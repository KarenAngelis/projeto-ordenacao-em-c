#include "ordenacao.h"

// Conta quantas vezes cada número aparece e escreve essa quantidade em ordem crescente.

void countingSort(int numeros[], int n, int contagens[]) { // Recebe os numeros e os contadores reservados antes da medicao.
    if (n < 2) { // Reconhece um vetor vazio ou unitario.
        return; // Termina porque o vetor ja esta ordenado.
    }
    for (int valor = 0; valor <= VALOR_MAXIMO; valor++) { // Percorre todos os valores que podem aparecer no vetor.
        contagens[valor] = 0; // Zera a frequencia; esta inicializacao faz parte do tempo de ordenacao.
    }
    for (int i = 0; i < n; i++) { // Visita cada numero do vetor original.
        contagens[numeros[i]]++; // Registra mais uma ocorrencia desse numero.
    }
    int destino = 0; // Marca a proxima posicao que recebera um numero ordenado.
    for (int valor = 0; valor <= VALOR_MAXIMO; valor++) { // Percorre os valores possiveis em ordem crescente.
        for (int vezes = 0; vezes < contagens[valor]; vezes++) { // Repete o numero de acordo com sua frequencia.
            numeros[destino] = valor; // Escreve uma ocorrencia no vetor de saida.
            destino++; // Avanca para a proxima posicao de saida.
        }
    }
}

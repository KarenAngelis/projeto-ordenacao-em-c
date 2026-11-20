#include "ordenacao.h"

// Divide o vetor em duas metades. Depois de ordenar cada metade, intercala as duas.

static void intercalar(int numeros[], int auxiliar[], int inicio, int meio, int fim) { // Junta duas metades que ja estao ordenadas.
    int esquerda = inicio; // Aponta para o proximo numero disponivel da metade esquerda.
    int direita = meio + 1; // Aponta para o proximo numero disponivel da metade direita.
    int destino = inicio; // Aponta para a proxima posicao de escrita no auxiliar.
    while (esquerda <= meio && direita <= fim) { // Compara as metades enquanto ambas possuem numeros.
        if (numeros[esquerda] <= numeros[direita]) { // Escolhe a esquerda no empate para manter a estabilidade.
            auxiliar[destino++] = numeros[esquerda++]; // Copia da esquerda e avanca os dois indices.
        } else { // Trata o caso em que o proximo numero da direita e menor.
            auxiliar[destino++] = numeros[direita++]; // Copia da direita e avanca os dois indices.
        }
    }
    while (esquerda <= meio) { // Verifica se sobraram numeros na metade esquerda.
        auxiliar[destino++] = numeros[esquerda++]; // Copia o proximo numero restante da esquerda.
    }
    while (direita <= fim) { // Verifica se sobraram numeros na metade direita.
        auxiliar[destino++] = numeros[direita++]; // Copia o proximo numero restante da direita.
    }
    for (int i = inicio; i <= fim; i++) { // Percorre o intervalo intercalado.
        numeros[i] = auxiliar[i]; // Devolve o numero ordenado ao vetor original durante a medicao.
    }
}

void mergeSort(int numeros[], int auxiliar[], int inicio, int fim) { // Recebe o intervalo incluindo suas duas extremidades.
    if (inicio < fim) { // Continua apenas se o intervalo possui pelo menos dois numeros.
        int meio = inicio + (fim - inicio) / 2; // Calcula o indice que divide o intervalo ao meio.
        mergeSort(numeros, auxiliar, inicio, meio); // Ordena a metade esquerda por recursao.
        mergeSort(numeros, auxiliar, meio + 1, fim); // Ordena a metade direita por recursao.
        intercalar(numeros, auxiliar, inicio, meio, fim); // Junta as duas metades em ordem crescente.
    }
}

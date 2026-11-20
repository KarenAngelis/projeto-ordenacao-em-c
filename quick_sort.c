#include "ordenacao.h"

void quickSort(int numeros[], int inicio, int fim) {
    while (inicio < fim) {
        int esquerda = inicio;
        int direita = fim;
        int pivo = numeros[inicio + (fim - inicio) / 2];

        // Os menores ficam à esquerda do pivô e os maiores à direita.
        while (esquerda <= direita) {
            while (numeros[esquerda] < pivo) esquerda++;
            while (numeros[direita] > pivo) direita--;

            if (esquerda <= direita) {
                int temp = numeros[esquerda];
                numeros[esquerda] = numeros[direita];
                numeros[direita] = temp;
                esquerda++;
                direita--; 
            }
        }

        // A chamada recursiva cuida da parte menor. O laço continua na maior.
        if (direita - inicio < fim - esquerda) {
            quickSort(numeros, inicio, direita);
            inicio = esquerda;
        } else {
            quickSort(numeros, esquerda, fim); 
            fim = direita;
        }
    }
}

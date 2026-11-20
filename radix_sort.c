#include "ordenacao.h"

// Ordena uma casa decimal por vez. Em cada passagem, conserva a ordem dos empates.

void radixSort(int numeros[], int n, int auxiliar[]) { // Recebe o vetor e um auxiliar reservado antes da medicao.
    if (n < 2) { // Reconhece um vetor que ja esta ordenado.
        return; // Termina sem acessar uma primeira posicao inexistente.
    }
    int maior = numeros[0]; // Assume inicialmente que o primeiro numero e o maior.
    for (int i = 1; i < n; i++) { // Procura o maior numero nas demais posicoes.
        if (numeros[i] > maior) { // Identifica um numero maior que o candidato atual.
            maior = numeros[i]; // Atualiza o maior valor encontrado.
        }
    }
    for (int casa = 1; maior / casa > 0; casa *= 10) { // Processa unidades, dezenas e seguintes; o limite de um milhao evita estouro.
        int contagens[10] = {0}; // Zera um contador para cada digito decimal.
        for (int i = 0; i < n; i++) { // Examina o digito atual de todos os numeros.
            int digito = (numeros[i] / casa) % 10; // Extrai o digito da casa decimal escolhida.
            contagens[digito]++; // Conta mais uma ocorrencia desse digito.
        }
        for (int digito = 1; digito < 10; digito++) { // Visita os contadores a partir do segundo digito.
            contagens[digito] += contagens[digito - 1]; // Calcula os limites acumulados de escrita de cada digito.
        }
        for (int i = n - 1; i >= 0; i--) { // Percorre de tras para frente para preservar a ordem nos empates.
            int digito = (numeros[i] / casa) % 10; // Extrai o digito do numero que sera distribuido.
            contagens[digito]--; // Reserva a ultima posicao ainda livre para esse digito.
            auxiliar[contagens[digito]] = numeros[i]; // Escreve o numero nessa posicao de forma estavel.
        }
        for (int i = 0; i < n; i++) { // Atualiza o vetor antes de passar para a proxima casa decimal.
            numeros[i] = auxiliar[i]; // Copia o resultado da passagem; esta copia interna faz parte da ordenacao.
        }
    }
}

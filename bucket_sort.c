#include "ordenacao.h"

// Separa os números por faixa, ordena cada balde por inserção e reúne os baldes em ordem.

static int escolherBalde(int numero, int n) { // Descobre a qual de N faixas o numero pertence.
    return (int)((long long)numero * n / (VALOR_MAXIMO + 1LL)); // Usa long long para multiplicar sem estouro e manter o indice entre zero e N menos um.
}

void bucketSort(int numeros[], int n, int auxiliar[], int contagens[], int posicoes[]) { // Recebe vetores reservados antes da medicao.
    if (n < 2) { // Reconhece entradas que ja estao ordenadas.
        return; // Termina sem preparar baldes desnecessarios.
    }
    for (int balde = 0; balde < n; balde++) { // Percorre os N baldes, um para cada faixa da distribuicao.
        contagens[balde] = 0; // Zera o tamanho desse balde dentro do tempo medido.
    }
    for (int i = 0; i < n; i++) { // Visita os numeros para descobrir o tamanho de cada balde.
        int balde = escolherBalde(numeros[i], n); // Identifica a faixa que recebera o numero atual.
        contagens[balde]++; // Registra mais um numero nessa faixa.
    }
    int acumulado = 0; // Conta quantas posicoes ja foram reservadas aos baldes anteriores.
    for (int balde = 0; balde < n; balde++) { // Define um trecho continuo do auxiliar para cada balde.
        posicoes[balde] = acumulado; // Guarda a primeira posicao livre do balde atual.
        acumulado += contagens[balde]; // Avanca pelo espaco ocupado por esse balde.
    }
    for (int i = 0; i < n; i++) { // Distribui os numeros pelos trechos reservados.
        int balde = escolherBalde(numeros[i], n); // Identifica o destino do numero atual.
        auxiliar[posicoes[balde]] = numeros[i]; // Escreve o numero na proxima posicao livre do balde.
        posicoes[balde]++; // Avanca o endereco de escrita desse balde.
    }
    int inicio = 0; // Comeca pelo primeiro balde do auxiliar.
    for (int balde = 0; balde < n; balde++) { // Visita os baldes em ordem crescente de faixa.
        int fim = inicio + contagens[balde]; // Calcula o limite exclusivo do trecho ocupado pelo balde.
        for (int i = inicio + 1; i < fim; i++) { // Aplica Insertion Sort somente dentro do balde atual.
            int chave = auxiliar[i]; // Preserva o numero que sera inserido no lugar correto.
            int j = i - 1; // Comeca a procurar a posicao imediatamente a esquerda.
            while (j >= inicio && auxiliar[j] > chave) { // Desloca somente os numeros maiores desse mesmo balde.
                auxiliar[j + 1] = auxiliar[j]; // Move o numero maior uma posicao para a direita.
                j--; // Continua a busca para a esquerda.
            }
            auxiliar[j + 1] = chave; // Insere a chave no espaco livre do balde.
        }
        inicio = fim; // Avanca ao primeiro numero do proximo balde.
    }
    for (int i = 0; i < n; i++) { // Percorre os baldes ja concatenados no auxiliar.
        numeros[i] = auxiliar[i]; // Devolve o resultado ao vetor original durante a medicao.
    }
}

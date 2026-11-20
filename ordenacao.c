#include "ordenacao.h" // Importa as declaracoes dos nove algoritmos para integra-los ao experimento.

const char *const NOMES[TOTAL_ALGORITMOS] = { // Guarda os nomes na mesma ordem usada pelo seletor abaixo.
    "Bubble Sort", "Selection Sort", "Insertion Sort", // Identifica os tres algoritmos simples de comparacao.
    "Merge Sort", "Quick Sort", "Heap Sort", // Identifica os tres algoritmos mais eficientes de comparacao.
    "Counting Sort", "Radix Sort", "Bucket Sort" // Identifica os tres algoritmos de distribuicao exigidos.
}; // Encerra a lista de nomes.

const char *const CATEGORIAS[TOTAL_ALGORITMOS] = { // Guarda a classificacao exibida nos arquivos de resultados.
    "Comparacao O(n^2)", "Comparacao O(n^2)", "Comparacao O(n^2)", // Classifica Bubble, Selection e Insertion pelo pior caso.
    "Comparacao O(n log n)", "Comparacao O(n log n) medio", "Comparacao O(n log n)", // Registra o caso medio do Quick; seu pior caso e O(n^2).
    "Distribuicao O(n+K)", "Distribuicao O(d*(n+10))", "Distribuicao O(n) esperado" // Bucket depende da distribuicao e pode levar O(n^2).
}; // Encerra a lista de categorias.

void ordenar(int algoritmo, int numeros[], int n, int auxiliar[], int contagens[], int posicoes[]) { // Escolhe uma ordenacao sem misturar sua implementacao com as demais.
    switch (algoritmo) { // Seleciona a funcao correspondente ao numero recebido pelo experimento.
        case 0: bubbleSort(numeros, n); break; // Executa a funcao de bubble_sort.c.
        case 1: selectionSort(numeros, n); break; // Executa a funcao de selection_sort.c.
        case 2: insertionSort(numeros, n); break; // Executa a funcao de insertion_sort.c.
        case 3: mergeSort(numeros, auxiliar, 0, n - 1); break; // Ordena o intervalo completo usando o auxiliar ja reservado.
        case 4: quickSort(numeros, 0, n - 1); break; // Ordena o intervalo completo usando um pivo.
        case 5: heapSort(numeros, n); break; // Executa a ordenacao com heap no proprio vetor.
        case 6: countingSort(numeros, n, contagens); break; // Usa os contadores reservados antes da medicao.
        case 7: radixSort(numeros, n, auxiliar); break; // Usa o auxiliar reservado antes da medicao.
        case 8: bucketSort(numeros, n, auxiliar, contagens, posicoes); break; // Usa os vetores previamente reservados para os baldes.
    } // Encerra a escolha entre os nove algoritmos.
} // Encerra o ponto comum de entrada das ordenacoes.

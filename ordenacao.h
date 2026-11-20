#ifndef ORDENACAO_H // Impede incluir este cabecalho duas vezes no mesmo arquivo compilado.
#define ORDENACAO_H // Marca o cabecalho como ja incluido.
#define VALOR_MAXIMO 1000000 // Define o maior valor aceito nos vetores: um milhao.
#define TOTAL_ALGORITMOS 9 // Define quantos algoritmos participam do projeto.
void bubbleSort(int numeros[], int n); // Declara a funcao implementada em bubble_sort.c.
void selectionSort(int numeros[], int n); // Declara a funcao implementada em selection_sort.c.
void insertionSort(int numeros[], int n); // Declara a funcao implementada em insertion_sort.c.
void mergeSort(int numeros[], int auxiliar[], int inicio, int fim); // Declara a funcao que ordena um intervalo por intercalacao.
void quickSort(int numeros[], int inicio, int fim); // Declara a funcao que divide um intervalo usando um pivo.
void heapSort(int numeros[], int n); // Declara a funcao implementada em heap_sort.c.
void countingSort(int numeros[], int n, int contagens[]); // Recebe contadores reservados antes da medicao.
void radixSort(int numeros[], int n, int auxiliar[]); // Recebe um auxiliar reservado antes da medicao.
void bucketSort(int numeros[], int n, int auxiliar[], int contagens[], int posicoes[]); // Recebe os vetores que representam os baldes.
extern const char *const NOMES[TOTAL_ALGORITMOS]; // Permite consultar os nomes definidos em ordenacao.c.
extern const char *const CATEGORIAS[TOTAL_ALGORITMOS]; // Permite consultar as categorias definidas em ordenacao.c.
void ordenar(int algoritmo, int numeros[], int n, int auxiliar[], int contagens[], int posicoes[]); // Escolhe qual algoritmo executar no experimento.
#endif // Encerra a protecao contra inclusoes repetidas.

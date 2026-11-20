#include "ordenacao.h" // Importa as declaracoes dos algoritmos, sem repetir suas implementacoes.
#include <stdint.h> // Disponibiliza uint32_t para gerar a mesma sequencia em diferentes execucoes.
#include <stdio.h> // Disponibiliza printf, fopen e fprintf para mostrar e salvar resultados.
#include <stdlib.h> // Disponibiliza malloc, free, qsort e os codigos de encerramento.
#include <string.h> // Disponibiliza memcpy, memcmp e strcmp para copias e verificacoes.
#ifdef _WIN32 // Escolhe um relogio de alta resolucao quando estamos no Windows.
#define WIN32_LEAN_AND_MEAN // Evita carregar declaracoes desnecessarias da biblioteca do Windows.
#include <windows.h> // Disponibiliza o contador de desempenho usado somente na medicao.
static LARGE_INTEGER frequencia; // Guarda quantos incrementos do contador equivalem a um segundo.
#else // Prepara uma alternativa em C padrao para outros sistemas.
#include <time.h> // Disponibiliza clock para medir tempo de CPU fora do Windows.
#endif // Encerra a selecao das bibliotecas do cronometro.
#define TOTAL_TAMANHOS 5 // Define a quantidade minima de tamanhos exigida pelo trabalho.
#define REPETICOES 3 // Define as tres ordenacoes usadas para calcular cada media.
static const int TAMANHOS[TOTAL_TAMANHOS] = {100, 1000, 10000, 25000, 50000}; // Inclui entradas pequenas e dezenas de milhares de numeros.

static double agoraMs(void) { // Consulta o relogio e devolve sua leitura em milissegundos.
#ifdef _WIN32 // Usa o contador de desempenho de alta resolucao do Windows.
    LARGE_INTEGER leitura; // Reserva espaco para receber o contador atual.
    if (!QueryPerformanceCounter(&leitura)) { // Detecta uma consulta de tempo sem resultado valido.
        fprintf(stderr, "Falha ao consultar o relogio.\n"); // Explica por que a medicao nao pode continuar.
        exit(EXIT_FAILURE); // Impede que uma falha do contador seja salva como um tempo real.
    } // Encerra o tratamento da falha do contador.
    return (double)leitura.QuadPart * 1000.0 / (double)frequencia.QuadPart; // Converte a contagem para milissegundos.
#else // Usa o relogio padrao nos outros sistemas operacionais.
    clock_t leitura = clock(); // Consulta o tempo de CPU consumido pelo programa.
    if (leitura == (clock_t)-1) { // Reconhece uma consulta invalida do relogio padrao.
        fprintf(stderr, "Relogio indisponivel.\n"); // Explica por que nao ha uma medicao confiavel.
        exit(EXIT_FAILURE); // Encerra sem registrar um resultado falso.
    } // Encerra o tratamento da falha do relogio padrao.
    return (double)leitura * 1000.0 / CLOCKS_PER_SEC; // Converte o tempo de CPU para milissegundos.
#endif // Encerra a escolha da implementacao do cronometro.
} // Encerra a consulta do tempo atual.

static int sortearNumero(uint32_t *estado) { // Gera numeros reproduziveis entre zero e um milhao.
    const uint32_t faixa = (uint32_t)VALOR_MAXIMO + 1; // Conta quantos valores diferentes sao permitidos.
    const uint32_t limite = UINT32_MAX - UINT32_MAX % faixa; // Define uma faixa de sorteio divisivel sem sobra pelo numero de valores.
    do { // Sorteia novamente se o resultado cair na sobra que criaria vies pelo modulo.
        *estado = *estado * UINT32_C(1664525) + UINT32_C(1013904223); // Atualiza um gerador congruencial de 32 bits com periodo completo.
    } while (*estado >= limite); // Rejeita a sobra da divisao para equilibrar as frequencias dos valores.
    return (int)(*estado % faixa); // Reduz o candidato aceito para o intervalo de zero a VALOR_MAXIMO.
} // Encerra a geracao de um numero de entrada.

static int compararInteiros(const void *a, const void *b) { // Fornece a comparacao exigida por qsort, usado somente para validar.
    int primeiro = *(const int *)a; // Le o primeiro numero a partir do endereco recebido.
    int segundo = *(const int *)b; // Le o segundo numero a partir do endereco recebido.
    return (primeiro > segundo) - (primeiro < segundo); // Retorna sinal positivo, negativo ou zero sem risco de estouro por subtracao.
} // Encerra a comparacao da ordenacao de referencia.

static int testar(int base[], int copia[], int esperado[], int auxiliar[], int contagens[], int posicoes[]) { // Confere todas as ordenacoes em entradas variadas.
    const int tamanhos[] = {0, 1, 2, 3, 10, 100, 1000}; // Inclui vazio, unitario e varios tamanhos nao triviais.
    uint32_t estado = UINT32_C(2026); // Fixa a sequencia pseudoaleatoria dos testes.
    int aprovados = 0; // Conta quantas combinacoes foram realmente aprovadas.
    for (int t = 0; t < 7; t++) { // Percorre os sete tamanhos de teste.
        int n = tamanhos[t]; // Obtem o tamanho da entrada atual.
        for (int padrao = 0; padrao < 8; padrao++) { // Verifica oito formatos diferentes para cada tamanho.
            for (int i = 0; i < n; i++) { // Preenche todas as posicoes da entrada atual.
                switch (padrao) { // Escolhe o formato dos numeros para este teste.
                    case 0: base[i] = sortearNumero(&estado); break; // Testa valores pseudoaleatorios em toda a faixa.
                    case 1: base[i] = i; break; // Testa uma entrada que ja esta em ordem crescente.
                    case 2: base[i] = n - i; break; // Testa uma entrada em ordem decrescente.
                    case 3: base[i] = 42; break; // Testa uma entrada com todos os numeros iguais.
                    case 4: base[i] = sortearNumero(&estado) % 5; break; // Testa muitos numeros repetidos.
                    case 5: base[i] = i % 2 == 0 ? 0 : VALOR_MAXIMO; break; // Testa os valores minimo e maximo permitidos.
                    default: base[i] = i; break; // Prepara uma entrada crescente para criar o caso quase ordenado.
                } // Encerra a escolha do valor dessa posicao.
            } // Encerra o preenchimento da entrada de teste.
            if (padrao == 6 && n > 1) { // Identifica o caso quase ordenado com pelo menos dois numeros.
                int temporario = base[0]; // Preserva o primeiro numero.
                base[0] = base[n - 1]; // Move o ultimo numero para o inicio.
                base[n - 1] = temporario; // Move o primeiro numero para o final.
            } // Encerra a criacao da entrada quase ordenada.
            size_t bytes = (size_t)n * sizeof(int); // Calcula a quantidade de bytes que devemos copiar e comparar.
            if (padrao == 7) { // Monta uma permutacao desfavoravel ao Quick Sort com pivo central.
                for (int i = 0; i < n; i++) { // Simula uma sequencia de particoes muito desequilibradas.
                    int meio = i + (n - 1 - i) / 2; // Localiza o pivo central do trecho ainda nao processado.
                    esperado[base[meio]] = i; // Atribui o menor valor restante ao elemento escolhido como pivo.
                    int temporario = base[i]; // Preserva o identificador da primeira posicao restante.
                    base[i] = base[meio]; // Simula o deslocamento do pivo para o inicio desse trecho.
                    base[meio] = temporario; // Atualiza a posicao do identificador que foi deslocado.
                } // Encerra a construcao da permutacao desfavoravel.
                memcpy(base, esperado, bytes); // Usa a permutacao construida como entrada do teste.
            } // Encerra a preparacao do caso de particoes desequilibradas.
            memcpy(esperado, base, bytes); // Copia a entrada para produzir a resposta de referencia.
            qsort(esperado, (size_t)n, sizeof(int), compararInteiros); // Ordena a referencia com a biblioteca padrao do C.
            for (int algoritmo = 0; algoritmo < TOTAL_ALGORITMOS; algoritmo++) { // Testa os nove algoritmos com a mesma entrada.
                memcpy(copia, base, bytes); // Restaura os numeros originais antes de cada algoritmo.
                copia[n] = -12345; // Coloca uma sentinela na primeira posicao que esta fora do vetor de teste.
                ordenar(algoritmo, copia, n, auxiliar, contagens, posicoes); // Executa a implementacao que esta sendo testada.
                if (memcmp(copia, esperado, bytes) != 0 || copia[n] != -12345) { // Confere a resposta e detecta escrita imediatamente alem do vetor.
                    fprintf(stderr, "Falha: %s, N=%d, padrao=%d.\n", NOMES[algoritmo], n, padrao); // Identifica um caso reproduzivel em que houve erro.
                    return EXIT_FAILURE; // Interrompe a bateria assim que um resultado incorreto aparece.
                } // Encerra a verificacao da resposta do algoritmo.
                aprovados++; // Registra que esta combinacao passou pela comparacao completa.
            } // Encerra os algoritmos para essa entrada.
        } // Encerra os formatos de entrada desse tamanho.
    } // Encerra todos os tamanhos de teste.
    printf("Testes aprovados: %d combinacoes.\n", aprovados); // Mostra o total real de combinacoes aprovadas.
    return EXIT_SUCCESS; // Informa ao terminal que todos os testes passaram.
} // Encerra os testes de corretude.

int main(int argc, char *argv[]) { // Inicia o programa e recebe os argumentos digitados no terminal.
    int somenteTestes = argc == 2 && strcmp(argv[1], "--testar") == 0; // Reconhece o comando que roda apenas os testes.
    if (argc != 1 && !somenteTestes) { // Recusa argumentos diferentes do modo de teste documentado.
        fprintf(stderr, "Uso: main.exe [--testar]\n"); // Mostra as duas formas validas de executar o programa.
        return EXIT_FAILURE; // Informa que o comando recebido e invalido.
    } // Encerra a verificacao dos argumentos.
#ifdef _WIN32 // Inicializa o contador especifico do Windows antes das medicoes.
    if (!QueryPerformanceFrequency(&frequencia) || frequencia.QuadPart <= 0) { // Confirma que existe um contador de alta resolucao valido.
        fprintf(stderr, "Relogio indisponivel.\n"); // Explica por que nao podemos realizar o experimento.
        return EXIT_FAILURE; // Encerra sem produzir tempos ficticios.
    } // Encerra a verificacao inicial do relogio.
#endif // Encerra a inicializacao especifica do Windows.
    int resultado = EXIT_FAILURE; // Assume falha ate terminar a validacao e a gravacao dos resultados.
    size_t capacidade = (size_t)TAMANHOS[TOTAL_TAMANHOS - 1]; // Obtem espaco suficiente para o maior tamanho do experimento.
    int *base = malloc(capacidade * sizeof(int)); // Reserva o vetor base antes de iniciar qualquer medicao.
    int *copia = malloc(capacidade * sizeof(int)); // Reserva o vetor que sera restaurado antes de cada execucao.
    int *esperado = malloc(capacidade * sizeof(int)); // Reserva a resposta de referencia usada somente na validacao.
    int *auxiliar = malloc(capacidade * sizeof(int)); // Reserva apoio para Merge, Radix e Bucket fora da medicao.
    int *contagens = malloc(((size_t)VALOR_MAXIMO + 1) * sizeof(int)); // Reserva os contadores sem zerar antecipadamente o trabalho dos algoritmos.
    int *posicoes = malloc(capacidade * sizeof(int)); // Reserva o controle das posicoes dos baldes fora da medicao.
    FILE *medidas = NULL, *tabela = NULL; // Comeca sem arquivos abertos para permitir limpeza segura em qualquer saida.
    double medias[TOTAL_ALGORITMOS][TOTAL_TAMANHOS] = {{0}}; // Guarda as medias que formarao a tabela comparativa.
    uint32_t estado = UINT32_C(2026); // Fixa a semente para repetir os mesmos dados em outra execucao.
    if (!base || !copia || !esperado || !auxiliar || !contagens || !posicoes) { // Verifica todas as reservas de memoria antes de usar os vetores.
        fprintf(stderr, "Memoria insuficiente para iniciar o programa.\n"); // Explica a falha de preparacao.
        goto finalizar; // Libera o que foi reservado antes de encerrar.
    } // Encerra a verificacao da memoria principal.
    if (somenteTestes) { // Verifica se o usuario solicitou apenas a bateria de corretude.
        resultado = testar(base, copia, esperado, auxiliar, contagens, posicoes); // Executa os testes sem criar arquivos de tempos.
        goto finalizar; // Libera a memoria e encerra com o resultado dos testes.
    } // Encerra o tratamento do modo de testes.
    medidas = fopen("tempos.csv", "w"); // Abre o CSV que recebera as 135 medicoes individuais.
    if (medidas == NULL) { // Verifica se conseguimos criar o arquivo no diretorio atual.
        perror("Nao foi possivel criar tempos.csv"); // Mostra o motivo da falha fornecido pelo sistema.
        goto finalizar; // Libera a memoria antes de sair com erro.
    } // Encerra a verificacao da abertura do CSV individual.
    fprintf(medidas, "categoria,algoritmo,n,repeticao,tempo_ms\n"); // Identifica as colunas e a unidade de tempo.
    puts("9 algoritmos | 5 tamanhos | 3 repeticoes | seed=2026 | tempo em ms"); // Apresenta as condicoes fixas do experimento.
    for (int t = 0; t < TOTAL_TAMANHOS; t++) { // Percorre os cinco tamanhos escolhidos.
        int n = TAMANHOS[t]; // Obtem a quantidade de numeros desta escala.
        size_t bytes = (size_t)n * sizeof(int); // Calcula quantos bytes pertencem ao vetor atual.
        for (int i = 0; i < n; i++) { // Gera exatamente um vetor base para este tamanho.
            base[i] = sortearNumero(&estado); // Sorteia um valor sem incluir geracao no tempo de ordenacao.
        } // Encerra a geracao dos dados compartilhados.
        memcpy(esperado, base, bytes); // Copia os dados para produzir uma referencia de resposta.
        qsort(esperado, (size_t)n, sizeof(int), compararInteiros); // Ordena a referencia fora do trecho cronometrado.
        for (int algoritmo = 0; algoritmo < TOTAL_ALGORITMOS; algoritmo++) { // Submete todos os algoritmos ao mesmo vetor base.
            double soma = 0.0; // Zera a soma dos tempos desta combinacao de algoritmo e tamanho.
            for (int repeticao = 1; repeticao <= REPETICOES; repeticao++) { // Mede tres execucoes independentes da mesma entrada.
                memcpy(copia, base, bytes); // Restaura a entrada antes do cronometro, inclusive entre repeticoes.
                double inicio = agoraMs(); // Marca o inicio imediatamente antes da ordenacao.
                ordenar(algoritmo, copia, n, auxiliar, contagens, posicoes); // Mede apenas a chamada da rotina de ordenacao.
                double duracao = agoraMs() - inicio; // Interrompe o tempo antes de validar ou escrever resultados.
                if (memcmp(copia, esperado, bytes) != 0) { // Confere a resposta inteira fora da medicao.
                    fprintf(stderr, "Ordenacao incorreta: %s, N=%d.\n", NOMES[algoritmo], n); // Identifica a combinacao que falhou.
                    goto finalizar; // Impede que uma ordenacao incorreta seja anunciada como resultado valido.
                } // Encerra a validacao dessa execucao.
                soma += duracao; // Acumula o tempo medido para calcular a media aritmetica.
                fprintf(medidas, "%s,%s,%d,%d,%.9f\n", CATEGORIAS[algoritmo], NOMES[algoritmo], n, repeticao, duracao); // Salva a medicao individual depois do cronometro.
            } // Encerra as tres repeticoes desta combinacao.
            medias[algoritmo][t] = soma / REPETICOES; // Calcula a media aritmetica exigida pelo trabalho.
            printf("%-15s | N=%6d | media=%12.6f ms\n", NOMES[algoritmo], n, medias[algoritmo][t]); // Exibe o progresso com unidade de tempo.
            fflush(stdout); // Mostra o progresso imediatamente mesmo quando a saida e redirecionada.
        } // Encerra os nove algoritmos desse tamanho.
    } // Encerra os cinco tamanhos do experimento.
    tabela = fopen("tabela_tempos.csv", "w"); // Cria a tabela resumida depois de concluir as medicoes.
    if (tabela == NULL) { // Verifica se o arquivo de medias pode ser criado.
        perror("Nao foi possivel criar tabela_tempos.csv"); // Exibe o motivo da falha de abertura.
        goto finalizar; // Libera os recursos antes de encerrar.
    } // Encerra a verificacao da abertura da tabela.
    fprintf(tabela, "categoria,algoritmo"); // Inicia o cabecalho pelas colunas descritivas.
    for (int t = 0; t < TOTAL_TAMANHOS; t++) { // Escreve uma coluna para cada tamanho de entrada.
        fprintf(tabela, ",N=%d (ms)", TAMANHOS[t]); // Identifica o tamanho e explicita a unidade de tempo.
    } // Encerra os nomes das colunas numericas.
    fprintf(tabela, "\n"); // Termina a linha do cabecalho.
    for (int algoritmo = 0; algoritmo < TOTAL_ALGORITMOS; algoritmo++) { // Escreve uma linha de medias para cada algoritmo.
        fprintf(tabela, "%s,%s", CATEGORIAS[algoritmo], NOMES[algoritmo]); // Registra a categoria e o nome da ordenacao.
        for (int t = 0; t < TOTAL_TAMANHOS; t++) { // Percorre as cinco medias desse algoritmo.
            fprintf(tabela, ",%.9f", medias[algoritmo][t]); // Grava a media sem arredondar precocemente os valores pequenos.
        } // Encerra as medias desse algoritmo.
        fprintf(tabela, "\n"); // Termina a linha da tabela comparativa.
    } // Encerra a tabela dos nove algoritmos.
    resultado = EXIT_SUCCESS; // Marca sucesso sujeito a verificacao final de fechamento dos arquivos.
finalizar: // Reune a limpeza dos recursos nos caminhos de sucesso e de falha.
    free(base); // Libera a entrada original; free tambem aceita NULL.
    free(copia); // Libera o vetor restaurado antes de cada execucao.
    free(esperado); // Libera a resposta de referencia.
    free(auxiliar); // Libera o vetor auxiliar das ordenacoes.
    free(contagens); // Libera os contadores de valores e de baldes.
    free(posicoes); // Libera o controle das posicoes dos baldes.
    if (medidas != NULL && fclose(medidas) != 0) { // Fecha o CSV individual e detecta falhas de escrita pendentes.
        perror("Falha ao concluir tempos.csv"); // Informa que os resultados podem nao ter sido gravados integralmente.
        resultado = EXIT_FAILURE; // Impede um encerramento bem-sucedido depois de falha no disco.
    } // Encerra o fechamento das medicoes individuais.
    if (tabela != NULL && fclose(tabela) != 0) { // Fecha a tabela de medias e verifica a conclusao da gravacao.
        perror("Falha ao concluir tabela_tempos.csv"); // Explica a falha de escrita da tabela.
        resultado = EXIT_FAILURE; // Registra que o resultado nao foi salvo completamente.
    } // Encerra o fechamento da tabela comparativa.
    if (resultado == EXIT_SUCCESS && !somenteTestes) { // Verifica se o experimento completo terminou sem erros.
        puts("Concluido: tempos.csv e tabela_tempos.csv foram gravados na pasta atual."); // Indica os arquivos com os resultados reais.
    } // Encerra a mensagem final do experimento.
    return resultado; // Entrega ao terminal o codigo de sucesso ou falha.
} // Encerra o programa principal.

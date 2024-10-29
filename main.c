#include "include/grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <float.h> 

// Declaração das funções auxiliares
TipoRepresentacao obterTipoRepresentacao(const char *arg);
char *gerarNomeBaseArquivo(const char *nomeArquivoEntrada, const char *tipoRepresentacao);
Grafo *carregarGrafo(const char *nomeArquivoEntrada, TipoRepresentacao tipo);
void escreverEstatisticasGrafo(Grafo *grafo, const char *baseNomeArquivo);
void executarDFS(Grafo *grafo, const char *baseNomeArquivo, TipoRepresentacao tipo);
void executarBFS(Grafo *grafo, const char *baseNomeArquivo, TipoRepresentacao tipo);
void descobrirComponentesConexas(Grafo *grafo, const char *baseNomeArquivo);
void calcularDistanciaVerticesInterativo(Grafo *grafo);
void executarEstudoCasoBFS(Grafo *grafo);
void executarEstudoCasoDFS(Grafo *grafo);
void executarEstudoCasoDijkstraVetor(Grafo *grafo, const char *baseNomeArquivo);


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <nome do arquivo> <tipo de representação (matriz ou lista)>\n", argv[0]);
        return 1;
    }

    // Obtém o tipo de representação
    TipoRepresentacao tipo = obterTipoRepresentacao(argv[2]);
    if (tipo == -1) {
        printf("Tipo de representação inválido. Use 'matriz' ou 'lista'.\n");
        return 1;
    }

    // Carrega o grafo a partir do arquivo
    Grafo *grafo = carregarGrafo(argv[1], tipo);
    if (!grafo) {
        printf("Erro ao carregar o grafo.\n");
        return 1;
    }

    int pid = getpid();
    printf("Grafo carregado. Pressione Enter para continuar... (PID: %d)", pid);
    getchar(); 

    // Gera o nome base do arquivo de saída
    char *baseNomeArquivo = gerarNomeBaseArquivo(argv[1], argv[2]);
    if (!baseNomeArquivo) {
        printf("Erro ao gerar o nome base do arquivo.\n");
        liberarGrafo(grafo);
        return 1;
    }

    // Menu de opções
    int opcao;
    do {
        printf("\nSelecione uma opção:\n");
        printf("1. Obter informações do grafo\n");
        printf("2. Executar BFS\n");
        printf("3. Executar DFS\n");
        printf("4. Gerar componentes\n");
        printf("5. Calcular distância\n");
        printf("6. Estudo de caso 2 (100 BFS)\n");  
        printf("7. Estudo de caso 3 (100 DFS)\n");  
        printf("8. Calcular distâncias e caminhos mínimos (Dijkstra com vetor)\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Obter informações do grafo
                escreverEstatisticasGrafo(grafo, baseNomeArquivo);
                break;
            case 2:
                // Executar BFS
                executarBFS(grafo, baseNomeArquivo, tipo);
                break;
            case 3:
                // Executar DFS
                executarDFS(grafo, baseNomeArquivo, tipo);
                break;
            case 4:
                // Gerar componentes
                descobrirComponentesConexas(grafo, baseNomeArquivo);
                break;
            case 5:
                // Calcular distância
                calcularDistanciaVerticesInterativo(grafo);
                break;
            case 6:
                // Estudo de caso 2: 100 BFS
                executarEstudoCasoBFS(grafo);
                break;
            case 7:
                // Estudo de caso 3: 100 DFS
                executarEstudoCasoDFS(grafo);
                break;
            case 8:
                executarEstudoCasoDijkstraVetor(grafo, baseNomeArquivo);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    // Libera recursos
    liberarGrafo(grafo);
    free(baseNomeArquivo);

    return 0;
}

// Implementações das funções auxiliares
TipoRepresentacao obterTipoRepresentacao(const char *arg) {
    if (strcmp(arg, "matriz") == 0) {
        return MATRIZ_ADJACENCIA;
    } else if (strcmp(arg, "lista") == 0) {
        return LISTA_ADJACENCIA;
    } else {
        return -1; // Tipo inválido
    }
}

char *gerarNomeBaseArquivo(const char *nomeArquivoEntrada, const char *tipoRepresentacao) {
    // Remove a extensão do nome do arquivo de entrada
    char baseNomeArquivo[256];
    strncpy(baseNomeArquivo, nomeArquivoEntrada, sizeof(baseNomeArquivo) - 1);
    baseNomeArquivo[sizeof(baseNomeArquivo) - 1] = '\0'; // Garante terminação nula

    // Remove a extensão
    char *ponto = strrchr(baseNomeArquivo, '.');
    if (ponto != NULL) {
        *ponto = '\0'; // Remove a extensão
    }

    // Calcula o tamanho necessário para o resultado
    size_t tamanhoBase = strlen(baseNomeArquivo);
    size_t tamanhoTipo = strlen(tipoRepresentacao);
    size_t tamanhoTotal = tamanhoBase + 1 + tamanhoTipo + 1; // +1 para '-', +1 para '\0'

    // Aloca memória para o resultado
    char *resultado = (char *)malloc(tamanhoTotal * sizeof(char));
    if (!resultado) {
        return NULL;
    }

    // Constrói o nome base do arquivo
    snprintf(resultado, tamanhoTotal, "%s-%s", baseNomeArquivo, tipoRepresentacao);

    return resultado;
}

Grafo *carregarGrafo(const char *nomeArquivoEntrada, TipoRepresentacao tipo) {
    FILE *arquivo = fopen(nomeArquivoEntrada, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivoEntrada);
        return NULL;
    }

    int numVertices;
    if (fscanf(arquivo, "%d", &numVertices) != 1) {
        printf("Erro ao ler o número de vértices.\n");
        fclose(arquivo);
        return NULL;
    }
    fclose(arquivo);

    Grafo *grafo = criarGrafo(numVertices, tipo);
    if (grafo) {
        lerArestas(grafo, nomeArquivoEntrada);
    }
    return grafo;
}

void escreverEstatisticasGrafo(Grafo *grafo, const char *baseNomeArquivo) {
    // Gera o nome do arquivo de saída
    char nomeArquivoSaida[256];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s-infos.txt", baseNomeArquivo);

    FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (!arquivoSaida) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivoSaida);
        return;
    }

    EstatisticasGrafo *stats = calcularEstatisticasGrafo(grafo);
    fprintf(arquivoSaida, "Número de arestas: %d\n", stats->numArestas);
    fprintf(arquivoSaida, "Grau mínimo: %d\n", stats->grauMinimo);
    fprintf(arquivoSaida, "Grau máximo: %d\n", stats->grauMaximo);
    fprintf(arquivoSaida, "Grau médio: %.2f\n", stats->grauMedio);
    fprintf(arquivoSaida, "Mediana do grau: %d\n", stats->medianaGrau);
    free(stats);

    fclose(arquivoSaida);

    printf("Informações do grafo escritas no arquivo %s.\n", nomeArquivoSaida);
}

void executarDFS(Grafo *grafo, const char *baseNomeArquivo, TipoRepresentacao tipo) {
    // Gera o nome do arquivo de saída
    char nomeArquivoSaida[256];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s-DFS.txt", baseNomeArquivo);

    FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (!arquivoSaida) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivoSaida);
        return;
    }

    int numVertices = grafo->numVertices;
    int *visitadosDFS = (int *)calloc(numVertices, sizeof(int));
    int *paisDFS = (int *)malloc(numVertices * sizeof(int));
    int *niveisDFS = (int *)malloc(numVertices * sizeof(int));
    Grafo *arvoreDFS = criarGrafoVazio(numVertices, tipo);

    int verticeInicial;
    printf("Vértice inicial: ");
    scanf("%d", &verticeInicial);

    fprintf(arquivoSaida, "Executando DFS com pilha a partir do vértice 1:\n");
    dfsComPilhaArvore(grafo, verticeInicial-1, visitadosDFS, arvoreDFS, paisDFS, niveisDFS, arquivoSaida);

    // Libera a memória alocada para DFS
    free(visitadosDFS);
    free(paisDFS);
    free(niveisDFS);
    liberarGrafo(arvoreDFS);

    fclose(arquivoSaida);

    printf("Resultado da DFS escrito no arquivo %s.\n", nomeArquivoSaida);
}

void executarBFS(Grafo *grafo, const char *baseNomeArquivo, TipoRepresentacao tipo) {
    // Gera o nome do arquivo de saída
    char nomeArquivoSaida[256];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s-BFS.txt", baseNomeArquivo);

    FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (!arquivoSaida) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivoSaida);
        return;
    }

    int numVertices = grafo->numVertices;
    int *visitadosBFS = (int *)calloc(numVertices, sizeof(int));
    int *paisBFS = (int *)malloc(numVertices * sizeof(int));
    int *niveisBFS = (int *)malloc(numVertices * sizeof(int));
    Grafo *arvoreBFS = criarGrafoVazio(numVertices, tipo);

    int verticeInicial;
    printf("Vértice inicial: ");
    scanf("%d", &verticeInicial);

    fprintf(arquivoSaida, "Executando BFS a partir do vértice %d:\n", verticeInicial);
    bfsComFilaArvore(grafo, verticeInicial-1, visitadosBFS, arvoreBFS, paisBFS, niveisBFS, arquivoSaida);

    // Libera a memória alocada para BFS
    free(visitadosBFS);
    free(paisBFS);
    free(niveisBFS);
    liberarGrafo(arvoreBFS);

    fclose(arquivoSaida);

    printf("Resultado da BFS escrita no arquivo %s.\n", nomeArquivoSaida);
}

void descobrirComponentesConexas(Grafo *grafo, const char *baseNomeArquivo) {
    // Gera o nome do arquivo de saída
    char nomeArquivoSaida[256];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s-componentes.txt", baseNomeArquivo);

    FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (!arquivoSaida) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivoSaida);
        return;
    }

    int numVertices = grafo->numVertices;
    int *componentes = (int *)calloc(numVertices, sizeof(int));
    int numComponentes = 0;
    descobrirComponentes(grafo, componentes, &numComponentes);

    // Criar listas para armazenar os vértices de cada componente
    int **listaComponentes = (int **)malloc((numComponentes + 1) * sizeof(int *));
    int *tamanhoComponentes = (int *)calloc((numComponentes + 1), sizeof(int));

    // Contar quantos vértices há em cada componente
    for (int i = 0; i < numVertices; i++) {
        int compID = componentes[i];
        tamanhoComponentes[compID]++;
    }

    // Alocar espaço para armazenar os vértices de cada componente
    for (int i = 1; i <= numComponentes; i++) {
        listaComponentes[i] = (int *)malloc(tamanhoComponentes[i] * sizeof(int));
        tamanhoComponentes[i] = 0; // Reiniciar para usar como índice ao adicionar os vértices
    }

    // Preencher as listas com os vértices de cada componente
    for (int i = 0; i < numVertices; i++) {
        int compID = componentes[i];
        listaComponentes[compID][tamanhoComponentes[compID]] = i;
        tamanhoComponentes[compID]++;
    }

    // Escrever as componentes no arquivo de saída
    fprintf(arquivoSaida, "Número de componentes conexas: %d\n", numComponentes);
    for (int i = 1; i <= numComponentes; i++) {
        fprintf(arquivoSaida, "\nComponente %d: ", i);
        for (int j = 0; j < tamanhoComponentes[i]; j++) {
            fprintf(arquivoSaida, "%d ", listaComponentes[i][j] + 1); // +1 para notação 1-based
        }
        fprintf(arquivoSaida, "\n");
    }

    // Liberar a memória alocada para componentes
    for (int i = 1; i <= numComponentes; i++) {
        free(listaComponentes[i]);
    }
    free(listaComponentes);
    free(tamanhoComponentes);
    free(componentes);

    fclose(arquivoSaida);

    printf("Componentes conexas escritas no arquivo %s.\n", nomeArquivoSaida);
}

void calcularDistanciaVerticesInterativo(Grafo *grafo) {
    int origem, destino;
    printf("Digite o número do vértice de origem: ");
    scanf("%d", &origem);
    printf("Digite o número do vértice de destino: ");
    scanf("%d", &destino);

    // Ajusta os índices para começar em 0
    origem -= 1;
    destino -= 1;

    if (origem < 0 || origem >= grafo->numVertices || destino < 0 || destino >= grafo->numVertices) {
        printf("Vértices inválidos. Por favor, insira valores entre 1 e %d.\n", grafo->numVertices);
        return;
    }

    int distancia = calcularDistancia(grafo, origem, destino);

    if (distancia != -1) {
        printf("A distância entre os vértices %d e %d é: %d\n", origem + 1, destino + 1, distancia);
    } else {
        printf("Não existe caminho entre os vértices %d e %d.\n", origem + 1, destino + 1);
    }
}

void executarEstudoCasoBFS(Grafo *grafo) {
    int numVertices = grafo->numVertices;
    int numExecucoes = 100;
    double tempoTotal = 0.0;

    // Semente para números aleatórios
    srand(time(NULL));

    for (int i = 0; i < numExecucoes; i++) {
        int verticeInicial = rand() % numVertices;

        // Inicializa arrays necessários
        int *visitadosBFS = (int *)calloc(numVertices, sizeof(int));
        int *paisBFS = (int *)malloc(numVertices * sizeof(int));
        int *niveisBFS = (int *)malloc(numVertices * sizeof(int));

        // Medir o tempo de execução
        clock_t inicio = clock();

        // Executa BFS sem escrever em arquivo
        bfsComFilaArvore(grafo, verticeInicial, visitadosBFS, NULL, paisBFS, niveisBFS, NULL);

        clock_t fim = clock();
        double tempoExecucao = (((double)(fim - inicio)) / CLOCKS_PER_SEC) * 1000;
        tempoTotal += tempoExecucao;

        // Libera a memória alocada para BFS
        free(visitadosBFS);
        free(paisBFS);
        free(niveisBFS);
    }

    double tempoMedio = tempoTotal / numExecucoes;
    printf("Tempo médio de execução do BFS: %.6f ms\n", tempoMedio);
}

void executarEstudoCasoDFS(Grafo *grafo) {
    int numVertices = grafo->numVertices;
    int numExecucoes = 100;
    double tempoTotal = 0.0;

    // Semente para números aleatórios
    srand(time(NULL));

    for (int i = 0; i < numExecucoes; i++) {
        int verticeInicial = rand() % numVertices;

        // Inicializa arrays necessários
        int *visitadosDFS = (int *)calloc(numVertices, sizeof(int));
        int *paisDFS = (int *)malloc(numVertices * sizeof(int));
        int *niveisDFS = (int *)malloc(numVertices * sizeof(int));

        // Inicializa os arrays paisDFS e niveisDFS
        for (int j = 0; j < numVertices; j++) {
            paisDFS[j] = -1;
            niveisDFS[j] = -1;
        }

        // Medir o tempo de execução
        clock_t inicio = clock();

        // Executa DFS sem escrever em arquivo
        dfsComPilhaArvore(grafo, verticeInicial, visitadosDFS, NULL, paisDFS, niveisDFS, NULL);

        clock_t fim = clock();
        double tempoExecucao = (((double)(fim - inicio)) / CLOCKS_PER_SEC) * 1000;
        tempoTotal += tempoExecucao;

        // Libera a memória alocada para DFS
        free(visitadosDFS);
        free(paisDFS);
        free(niveisDFS);
    }

    double tempoMedio = tempoTotal / numExecucoes;
    printf("Tempo médio de execução do DFS: %.6f ms\n", tempoMedio);
}

int possuiPesosNegativos(Grafo *grafo) {
    if (grafo->tipo == MATRIZ_ADJACENCIA) { // Verificação para a matriz de adjacência
        for (int i = 0; i < grafo->numVertices; i++) {
            for (int j = 0; j < grafo->numVertices; j++) {
                if (grafo->grafoMatriz->matriz[i][j] < 0) {  // Verifica se existe uma aresta com peso negativo (diferente de 0 e negativo)
                    return 1; // Retorna 1 se tiver peso negativo
                }
            }
        }
    } else if (grafo->tipo == LISTA_ADJACENCIA) { // Verificação para a lista de adjacência
        for (int i = 0; i < grafo->numVertices; i++) {
            No *atual = grafo->grafoLista->listaAdj[i];
            while (atual != NULL) {
                if (atual->peso < 0) {  // Verifica se existe uma aresta com peso negativo
                    return 1; // Retorna 1 se tiver peso negativo
                } 
                atual = atual->prox;
            }
        }
    }
    return 0; // Retorna 0 se não tiver pesos negativos
}

void executarEstudoCasoDijkstraVetor(Grafo *grafo, const char *baseNomeArquivo) {
    int origem = 9; // Índice 9 corresponde ao vértice 10 (notação 1-based)
    int destinos[] = {19, 29, 39, 49, 59}; // Índices para os vértices 20, 30, 40, 50, 60
    int numDestinos = sizeof(destinos) / sizeof(destinos[0]);

    // Gera o nome do arquivo de saída
    char nomeArquivoSaida[256];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s-estudoCaso3_1.txt", baseNomeArquivo);

    // Abre o arquivo de saída para escrever os resultados
    FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
    if (!arquivoSaida) {
        printf("Erro ao criar o arquivo %s.\n", nomeArquivoSaida);
        return;
    }

    fprintf(arquivoSaida, "Estudo de Caso 3.1: Distâncias e caminhos mínimos a partir do vértice 10\n\n");
    fprintf(arquivoSaida, "Destino\tDistância\tCaminho Mínimo\n");
    fprintf(arquivoSaida, "-------\t---------\t--------------\n");

    // Inicializa arrays necessários para o Dijkstra
    int numVertices = grafo->numVertices;
    double *distancia = (double *)malloc(numVertices * sizeof(double));
    int *visitados = (int *)calloc(numVertices, sizeof(int));
    int *pais = (int *)malloc(numVertices * sizeof(int));

    if (!distancia || !visitados || !pais) {
        printf("Erro ao alocar memória.\n");
        free(distancia);
        free(visitados);
        free(pais);
        fclose(arquivoSaida);
        return;
    }

    // Inicializa o array de distâncias com infinito e os pais com -1
    for (int i = 0; i < numVertices; i++) {
        distancia[i] = DBL_MAX;
        pais[i] = -1;
    }

    distancia[origem] = 0.0;

    // Implementação do algoritmo de Dijkstra
    for (int i = 0; i < numVertices - 1; i++) {
        double minDistancia = DBL_MAX;
        int u = -1;

        for (int v = 0; v < numVertices; v++) {
            if (!visitados[v] && distancia[v] < minDistancia) {
                minDistancia = distancia[v];
                u = v;
            }
        }

        if (u == -1) break; // Todos os vértices acessíveis foram processados

        visitados[u] = 1;

        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int v = 0; v < numVertices; v++) {
                if (grafo->grafoMatriz->matriz[u][v] > 0 && !visitados[v]) {
                    double peso = grafo->grafoMatriz->matriz[u][v];
                    if (distancia[u] + peso < distancia[v]) {
                        distancia[v] = distancia[u] + peso;
                        pais[v] = u;
                    }
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *adjacente = grafo->grafoLista->listaAdj[u];
            while (adjacente != NULL) {
                int v = adjacente->vertice;
                double peso = adjacente->peso;
                if (!visitados[v] && distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
                    pais[v] = u;
                }
                adjacente = adjacente->prox;
            }
        }
    }

    // Para cada destino, imprimir a distância e o caminho mínimo
    for (int i = 0; i < numDestinos; i++) {
        int destino = destinos[i];

        if (distancia[destino] == DBL_MAX) {
            fprintf(arquivoSaida, "%d\tInfinito\tInacessível\n", destino + 1);
        } else {
            // Reconstrói o caminho mínimo
            fprintf(arquivoSaida, "%d\t%.2f\t\t", destino + 1, distancia[destino]);
            int caminho[numVertices];
            int tamanhoCaminho = 0;
            int atual = destino;

            while (atual != -1) {
                caminho[tamanhoCaminho++] = atual;
                atual = pais[atual];
            }

            // Imprime o caminho na ordem correta
            for (int j = tamanhoCaminho - 1; j >= 0; j--) {
                fprintf(arquivoSaida, "%d", caminho[j] + 1);
                if (j > 0) {
                    fprintf(arquivoSaida, " -> ");
                }
            }
            fprintf(arquivoSaida, "\n");
        }
    }

    free(distancia);
    free(visitados);
    free(pais);
    fclose(arquivoSaida);

    printf("Resultado do estudo de caso 3.1 escrito no arquivo %s.\n", nomeArquivoSaida);
}

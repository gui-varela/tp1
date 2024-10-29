#include "../include/grafo.h"
#include <stdlib.h>
#include <stdio.h>

Grafo *criarGrafo(int numVertices, TipoRepresentacao tipo) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    grafo->tipo = tipo;

    if (tipo == MATRIZ_ADJACENCIA) {
        grafo->grafoMatriz = criarGrafoMatriz(numVertices);
        grafo->grafoLista = NULL;
    } else if (tipo == LISTA_ADJACENCIA) {
        grafo->grafoLista = criarGrafoLista(numVertices);
        grafo->grafoMatriz = NULL;
    }
    return grafo;
}

// Função para ler as arestas do arquivo e preencher o grafo
void lerArestas(Grafo *grafo, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int numVertices;
    fscanf(arquivo, "%d", &numVertices);  // Ignora o número de vértices, já foi lido

    int u, v;
    double peso;
    while (fscanf(arquivo, "%d %d %lf", &u, &v, &peso) == 3) { // garantir a leitura dos 3 itens em cada linha (par de vertices e o peso da aresta)
        // Verifica se os índices dos vértices são válidos
        if (u <= 0 || u > grafo->numVertices || v <= 0 || v > grafo->numVertices) {
            printf("Vértice inválido: %d ou %d\n", u, v);
            continue; // Ignora esta aresta inválida
        }

        // Adiciona aresta na matriz de adjacência ou lista de adjacência
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            adicionarArestaMatriz(grafo->grafoMatriz, u - 1, v - 1, peso);
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            adicionarArestaLista(grafo->grafoLista, u - 1, v - 1, peso);
        }
    }

    fclose(arquivo);
}


void liberarGrafo(Grafo *grafo) {
    if (grafo->tipo == MATRIZ_ADJACENCIA) {
        liberarGrafoMatriz(grafo->grafoMatriz);
    } else if (grafo->tipo == LISTA_ADJACENCIA) {
        liberarGrafoLista(grafo->grafoLista);
    }
    free(grafo);
}

// Função auxiliar para calcular estatísticas dos graus
EstatisticasGrafo *calcularEstatisticasGrafo(Grafo *grafo) {
    EstatisticasGrafo *stats = (EstatisticasGrafo *)malloc(sizeof(EstatisticasGrafo));
    int *graus = (int *)malloc(grafo->numVertices * sizeof(int));
    int totalGrau = 0;

    // Inicializar valores
    stats->grauMinimo = grafo->numVertices;
    stats->grauMaximo = 0;
    stats->numArestas = 0;

    // Calcular o grau de cada vértice
    for (int i = 0; i < grafo->numVertices; i++) {
        int grau = 0;
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                grau += grafo->grafoMatriz->matriz[i][j];
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[i];
            while (atual != NULL) {
                grau++;
                atual = atual->prox;
            }
        }

        graus[i] = grau;
        stats->numArestas += grau;

        // Atualizar grau mínimo e máximo
        if (grau < stats->grauMinimo) stats->grauMinimo = grau;
        if (grau > stats->grauMaximo) stats->grauMaximo = grau;
        totalGrau += grau;
    }

    // Calcular grau médio
    stats->grauMedio = (float)totalGrau / grafo->numVertices;

    // Calcular mediana
    qsort(graus, grafo->numVertices, sizeof(int), compararInteiros);
    if (grafo->numVertices % 2 == 0) {
        stats->medianaGrau = (graus[grafo->numVertices / 2 - 1] + graus[grafo->numVertices / 2]) / 2;
    } else {
        stats->medianaGrau = graus[grafo->numVertices / 2];
    }

    // Dividir por 2 pois cada aresta foi contada duas vezes
    stats->numArestas /= 2;
    stats->numVertices = grafo->numVertices;

    // Limpar
    free(graus);

    return stats;
}

// Função auxiliar para comparação
int compararInteiros(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Cria uma pilha com capacidade específica
Pilha *criarPilha(int capacidade) {
    Pilha *pilha = (Pilha *)malloc(sizeof(Pilha));
    if (!pilha) {
        fprintf(stderr, "Erro ao alocar memória para a pilha\n");
        exit(EXIT_FAILURE);
    }
    pilha->dados = (int *)malloc(capacidade * sizeof(int));
    if (!pilha->dados) {
        fprintf(stderr, "Erro ao alocar memória para os dados da pilha\n");
        exit(EXIT_FAILURE);
    }
    pilha->topo = -1;
    pilha->capacidade = capacidade;
    return pilha;
}

// Empilha um valor na pilha
void push(Pilha *pilha, int valor) {
    if (pilha->topo < pilha->capacidade - 1) {
        pilha->dados[++(pilha->topo)] = valor;
    } else {
        // Pilha cheia - redimensionar ou tratar o erro
        fprintf(stderr, "Erro: Pilha cheia ao tentar empilhar %d\n", valor);
        exit(EXIT_FAILURE);
    }
}

// Desempilha um valor da pilha
int pop(Pilha *pilha) {
    if (pilha->topo >= 0) {
        return pilha->dados[(pilha->topo)--];
    } else {
        // Tratamento de erro: pilha vazia
        fprintf(stderr, "Erro: Tentativa de desempilhar de uma pilha vazia\n");
        exit(EXIT_FAILURE);
    }
}

// Verifica se a pilha está vazia
int estaVaziaPilha(Pilha *pilha) {
    return pilha->topo == -1;
}

// Libera a memória da pilha
void liberarPilha(Pilha *pilha) {
    free(pilha->dados);
    free(pilha);
}

// Função para criar um grafo vazio (usado para a árvore DFS)
Grafo *criarGrafoVazio(int numVertices, TipoRepresentacao tipo) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    grafo->tipo = tipo;

    if (tipo == MATRIZ_ADJACENCIA) {
        grafo->grafoMatriz = criarGrafoMatriz(numVertices);
        grafo->grafoLista = NULL;
    } else if (tipo == LISTA_ADJACENCIA) {
        grafo->grafoLista = criarGrafoLista(numVertices);
        grafo->grafoMatriz = NULL;
    }
    return grafo;
}

// Função para adicionar uma aresta ao grafo (usada para construir a árvore DFS)
void adicionarArestaGrafo(Grafo *grafo, int u, int v, double peso) {
    if (grafo->tipo == MATRIZ_ADJACENCIA) {
        adicionarArestaMatriz(grafo->grafoMatriz, u, v, peso);
    } else if (grafo->tipo == LISTA_ADJACENCIA) {
        adicionarArestaLista(grafo->grafoLista, u, v, peso);
    }
    grafo->numArestas++;
}

// Função principal para DFS com pilha que constrói a árvore DFS
void dfsComPilhaArvore(Grafo *grafo, int verticeInicial, int *visitados,
                       Grafo *arvoreDFS, int *pais, int *niveis, FILE *arquivoSaida) {
    Pilha *pilha = criarPilha(grafo->numVertices);
    push(pilha, verticeInicial);
    visitados[verticeInicial] = 1;  // Marca como visitado no momento em que é empilhado

    pais[verticeInicial] = verticeInicial;
    niveis[verticeInicial] = 0;

    while (!estaVaziaPilha(pilha)) {
        int verticeAtual = pop(pilha);

        // Escreve no arquivo de saída se ele não for NULL
        if (arquivoSaida != NULL) {
            fprintf(arquivoSaida, "Vértice: %d, Pai: %d, Nível: %d\n",
                    verticeAtual + 1, pais[verticeAtual] + 1, niveis[verticeAtual]);
        }

        // Adiciona aresta ao grafo árvore, se não for o vértice inicial e arvoreDFS não for NULL
        if (verticeAtual != verticeInicial && arvoreDFS != NULL) {
            adicionarArestaGrafo(arvoreDFS, verticeAtual, pais[verticeAtual], 1.0); //adicionei peso 1 pq na dfs nao importa o peso
        }

        // Explora os vértices adjacentes
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = grafo->numVertices - 1; j >= 0; j--) {
                if (grafo->grafoMatriz->matriz[verticeAtual][j] == 1 && !visitados[j]) {
                    push(pilha, j);
                    visitados[j] = 1;
                    pais[j] = verticeAtual;
                    niveis[j] = niveis[verticeAtual] + 1;
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[verticeAtual];
            while (atual != NULL) {
                int v = atual->vertice;
                if (!visitados[v]) {
                    push(pilha, v);
                    visitados[v] = 1;  // Marca como visitado ao empilhar
                    pais[v] = verticeAtual;
                    niveis[v] = niveis[verticeAtual] + 1;
                }
                atual = atual->prox;
            }
        }
    }

    liberarPilha(pilha);
}

// Função para criar uma fila com capacidade específica
Fila *criarFila(int capacidade) {
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->dados = (int *)malloc(capacidade * sizeof(int));
    fila->frente = -1;  // Inicialize com -1 para indicar que a fila está vazia
    fila->tras = -1;    // Inicialize com -1 para indicar que a fila está vazia
    fila->capacidade = capacidade;
    return fila;
}

// Enfileira um valor na fila
void enqueue(Fila *fila, int valor) {
    // Verifica se a fila está cheia
    if ((fila->tras + 1) % fila->capacidade == fila->frente) {
        printf("Erro: Fila cheia ao tentar enfileirar %d\n", valor + 1);
        return;
    }

    if (fila->frente == -1) {
        // Caso especial para o primeiro elemento
        fila->frente = 0;
    }

    fila->tras = (fila->tras + 1) % fila->capacidade;
    fila->dados[fila->tras] = valor;
}

// Desenfileira um valor da fila
int dequeue(Fila *fila) {
    if (fila->frente == -1) {
        // Fila vazia
        printf("Erro: Fila vazia ao tentar desenfileirar\n");
        return -1;
    }

    int valor = fila->dados[fila->frente];
    if (fila->frente == fila->tras) {
        // Fila ficará vazia após esta operação
        fila->frente = -1;
        fila->tras = -1;
    } else {
        fila->frente = (fila->frente + 1) % fila->capacidade;
    }

    return valor;
}

// Verifica se a fila está vazia
int estaVaziaFila(Fila *fila) {
    int vazia = (fila->frente == -1 && fila->tras == -1);
    return vazia;
}

// Libera a memória da fila
void liberarFila(Fila *fila) {
    free(fila->dados);
    free(fila);
}

// Função principal para BFS
void bfsComFilaArvore(Grafo *grafo, int verticeInicial, int *visitados, Grafo *arvoreBFS, int *pais, int *niveis, FILE *arquivoSaida) {
    Fila *fila = criarFila(grafo->numVertices);
    enqueue(fila, verticeInicial);
    visitados[verticeInicial] = 1;
    pais[verticeInicial] = verticeInicial; // O pai do vértice inicial é ele mesmo
    niveis[verticeInicial] = 0;

    while (!estaVaziaFila(fila)) {
        int verticeAtual = dequeue(fila);

        // Escreve no arquivo de saída se ele não for NULL
        if (arquivoSaida != NULL) {
            fprintf(arquivoSaida, "Vértice: %d, Pai: %d, Nível: %d\n", verticeAtual + 1, pais[verticeAtual] + 1, niveis[verticeAtual]);
        }

        // Adiciona aresta à árvore BFS se não for o vértice inicial e se arvoreBFS não for NULL
        if (verticeAtual != verticeInicial && arvoreBFS != NULL) {
            adicionarArestaGrafo(arvoreBFS, verticeAtual, pais[verticeAtual], 1.0); //adicionei peso 1 pq na bfs nao importa o peso
        }

        // Explora os vértices adjacentes
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                if (grafo->grafoMatriz->matriz[verticeAtual][j] == 1 && !visitados[j]) {
                    visitados[j] = 1;  // Marca como visitado
                    enqueue(fila, j);   // Enfileira o vértice adjacente
                    pais[j] = verticeAtual;
                    niveis[j] = niveis[verticeAtual] + 1;
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[verticeAtual];
            while (atual != NULL) {
                int v = atual->vertice;
                if (!visitados[v]) {
                    visitados[v] = 1;  // Marca como visitado
                    enqueue(fila, v);   // Enfileira o vértice adjacente
                    pais[v] = verticeAtual;
                    niveis[v] = niveis[verticeAtual] + 1;
                }
                atual = atual->prox;
            }
        }
    }

    liberarFila(fila);
}

void descobrirComponentes(Grafo *grafo, int *componentes, int *numComponentes) {
    int *visitados = (int *)calloc(grafo->numVertices, sizeof(int));
    int *pais = (int *)malloc(grafo->numVertices * sizeof(int));
    int *niveis = (int *)malloc(grafo->numVertices * sizeof(int));
    *numComponentes = 0;

    for (int v = 0; v < grafo->numVertices; v++) {
        if (!visitados[v]) {
            (*numComponentes)++;
            // Inicializa os arrays 'pais' e 'niveis' para esta BFS
            for (int i = 0; i < grafo->numVertices; i++) {
                pais[i] = -1;
                niveis[i] = -1;
            }

            // Chama a BFS a partir do vértice 'v' sem gerar saída e sem construir a árvore BFS
            bfsComFilaArvore(grafo, v, visitados, NULL, pais, niveis, NULL);

            // Após a BFS, todos os vértices visitados pertencem à mesma componente
            for (int u = 0; u < grafo->numVertices; u++) {
                if (visitados[u] && componentes[u] == 0 && (pais[u] != -1 || u == v)) {
                    componentes[u] = *numComponentes;
                }
            }
        }
    }

    free(visitados);
    free(pais);
    free(niveis);
}

// Função para calcular a distância mínima entre dois vértices usando BFS
int calcularDistancia(Grafo *grafo, int origem, int destino) {
    if (origem < 0 || origem >= grafo->numVertices || destino < 0 || destino >= grafo->numVertices) {
        printf("Vértices inválidos.\n");
        return -1;
    }

    int *visitados = (int *)calloc(grafo->numVertices, sizeof(int));
    int *distancias = (int *)malloc(grafo->numVertices * sizeof(int));
    for (int i = 0; i < grafo->numVertices; i++) {
        distancias[i] = -1; // Inicializa todas as distâncias como -1 (não alcançado)
    }

    Fila *fila = criarFila(grafo->numVertices);
    enqueue(fila, origem);
    visitados[origem] = 1;
    distancias[origem] = 0;

    while (!estaVaziaFila(fila)) {
        int verticeAtual = dequeue(fila);

        // Se encontramos o destino, podemos retornar a distância
        if (verticeAtual == destino) {
            int distancia = distancias[destino];
            liberarFila(fila);
            free(visitados);
            free(distancias);
            return distancia;
        }

        // Explora os vértices adjacentes
        if (grafo->tipo == MATRIZ_ADJACENCIA) {
            for (int j = 0; j < grafo->numVertices; j++) {
                if (grafo->grafoMatriz->matriz[verticeAtual][j] == 1 && !visitados[j]) {
                    visitados[j] = 1;
                    distancias[j] = distancias[verticeAtual] + 1;
                    enqueue(fila, j);
                }
            }
        } else if (grafo->tipo == LISTA_ADJACENCIA) {
            No *atual = grafo->grafoLista->listaAdj[verticeAtual];
            while (atual != NULL) {
                int v = atual->vertice;
                if (!visitados[v]) {
                    visitados[v] = 1;
                    distancias[v] = distancias[verticeAtual] + 1;
                    enqueue(fila, v);
                }
                atual = atual->prox;
            }
        }
    }

    // Se o destino não foi alcançado, os vértices não estão conectados
    liberarFila(fila);
    free(visitados);
    free(distancias);
    return -1; // Indica que não há caminho entre origem e destino
}

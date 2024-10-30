#ifndef GRAFO_H
#define GRAFO_H

#include "grafo_matriz.h"
#include "grafo_lista.h"
#include <stdio.h>

typedef enum { MATRIZ_ADJACENCIA, LISTA_ADJACENCIA } TipoRepresentacao;

typedef struct {
    TipoRepresentacao tipo;
    int numVertices;
    int numArestas;
    GrafoMatriz *grafoMatriz;
    GrafoLista *grafoLista;
} Grafo;

typedef struct {
    int numVertices;
    int numArestas;
    int grauMinimo;
    int grauMaximo;
    float grauMedio;
    int medianaGrau;
} EstatisticasGrafo;

typedef struct {
    int *dados;
    int topo;
    int capacidade;
} Pilha;

typedef struct {
    int *dados;
    int frente;
    int tras;
    int capacidade;
} Fila;

typedef struct {
    int vertice;
    double distancia;
} HeapNode;

typedef struct {
    int capacidade;
    int tamanho;
    int *posicoes;
    HeapNode **array;
} MinHeap;


// Funções principais do grafo
Grafo *criarGrafo(int numVertices, TipoRepresentacao tipo);
Grafo *criarGrafoVazio(int numVertices, TipoRepresentacao tipo);
void liberarGrafo(Grafo *grafo);
EstatisticasGrafo *calcularEstatisticasGrafo(Grafo *grafo);
void lerArestas(Grafo *grafo, const char *nomeArquivo);
void adicionarArestaGrafo(Grafo *grafo, int u, int v, double peso);

// Funções auxiliares
int compararInteiros(const void *p, const void *arg);

// Funções para manipulação da pilha
Pilha *criarPilha(int capacidade);
void push(Pilha *pilha, int valor);
int pop(Pilha *pilha);
int estaVaziaPilha(Pilha *pilha);
void liberarPilha(Pilha *pilha);

// Funções para manipulação da fila
Fila *criarFila(int capacidade);
void enqueue(Fila *fila, int valor);
int dequeue(Fila *fila);
int estaVaziaFila(Fila *fila);
void liberarFila(Fila *fila);

// Funções DFS e BFS
void dfsComPilhaArvore(Grafo *grafo, int verticeInicial, int *visitados, Grafo *arvoreDFS, int *pais, int *niveis, FILE *arquivoSaida);
void bfsComFilaArvore(Grafo *grafo, int verticeInicial, int *visitados, Grafo *arvoreBFS, int *pais, int *niveis, FILE *arquivoSaida);
int calcularDistancia(Grafo *grafo, int origem, int destino);
void descobrirComponentes(Grafo *grafo, int *componentes, int *numComponentes);

// trab2
int possuiPesosNegativos(Grafo *grafo);
void dijkstraVetor(Grafo *grafo, int origem, int imprimir, double *distancia, int *pais);

// Protótipos das funções do Heap
MinHeap* criarMinHeap(int capacidade);
void trocarHeapNode(HeapNode** a, HeapNode** b);
void minHeapify(MinHeap* minHeap, int idx);
HeapNode* extrairMinimo(MinHeap* minHeap);
void diminuirChave(MinHeap* minHeap, int vertice, double distancia);
int estaNoHeap(MinHeap *minHeap, int vertice);
int estaVazioHeap(MinHeap *minHeap);
void liberarMinHeap(MinHeap *minHeap);

// Protótipo da função Dijkstra com Heap
void dijkstraHeap(Grafo *grafo, int origem, int imprimir, double *distancia, int *pais);


#endif // GRAFO_H

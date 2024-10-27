#ifndef GRAFO_H
#define GRAFO_H

#include "grafo_matriz.h"
#include "grafo_lista.h"
#include <stdio.h>  // Necessary for FILE*

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

// Structures for Stack and Queue
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

// Main graph functions
Grafo *criarGrafo(int numVertices, TipoRepresentacao tipo);
Grafo *criarGrafoVazio(int numVertices, TipoRepresentacao tipo);
void liberarGrafo(Grafo *grafo);
EstatisticasGrafo *calcularEstatisticasGrafo(Grafo *grafo);
void lerArestas(Grafo *grafo, const char *nomeArquivo);
void adicionarArestaGrafo(Grafo *grafo, int u, int v);

// Auxiliary functions
int compararInteiros(const void *p, const void *arg);

// Stack manipulation functions
Pilha *criarPilha(int capacidade);
void push(Pilha *pilha, int valor);
int pop(Pilha *pilha);
int estaVaziaPilha(Pilha *pilha);
void liberarPilha(Pilha *pilha);

// Queue manipulation functions
Fila *criarFila(int capacidade);
void enqueue(Fila *fila, int valor);
int dequeue(Fila *fila);
int estaVaziaFila(Fila *fila);
void liberarFila(Fila *fila);

// DFS and BFS functions
void dfsComPilhaArvore(Grafo *grafo, int verticeInicial, int *visitados, Grafo *arvoreDFS, int *pais, int *niveis, FILE *arquivoSaida);
void bfsComFilaArvore(Grafo *grafo, int verticeInicial, int *visitados, Grafo *arvoreBFS, int *pais, int *niveis, FILE *arquivoSaida); // New function

#endif // GRAFO_H

#ifndef GRAFO_H
#define GRAFO_H

#include "grafo_matriz.h"
#include "grafo_lista.h"
#include <stdio.h>  // Necessário para FILE*

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

// Estruturas da Pilha e Fila
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

// Nova função para calcular a distância entre dois vértices
int calcularDistancia(Grafo *grafo, int origem, int destino);

// **Nova função para descobrir as componentes conexas**
void descobrirComponentes(Grafo *grafo, int *componentes, int *numComponentes);

#endif // GRAFO_H

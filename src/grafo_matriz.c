#include "../include/grafo_matriz.h"
#include <stdlib.h>

GrafoMatriz *criarGrafoMatriz(int numVertices) {
    GrafoMatriz *grafo = (GrafoMatriz *)malloc(sizeof(GrafoMatriz));
    grafo->numVertices = numVertices;
    grafo->matriz = (int **)malloc(numVertices * sizeof(int *));
    for (int i = 0; i < numVertices; i++) {
        grafo->matriz[i] = (int *)calloc(numVertices, sizeof(int));
    }
    return grafo;
}

void liberarGrafoMatriz(GrafoMatriz *grafoMatriz) {
    for (int i = 0; i < grafoMatriz->numVertices; i++) {
        free(grafoMatriz->matriz[i]);
    }
    free(grafoMatriz->matriz);
    free(grafoMatriz);
}

// Função para adicionar uma aresta na matriz de adjacência
void adicionarArestaMatriz(GrafoMatriz *grafoMatriz, int u, int v) {
    grafoMatriz->matriz[u][v] = 1;
    grafoMatriz->matriz[v][u] = 1;
}
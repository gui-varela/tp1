#include "../include/grafo_matriz.h"
#include <stdlib.h>

GrafoMatriz *criarGrafoMatriz(int numVertices) {
    GrafoMatriz *grafo = (GrafoMatriz *)malloc(sizeof(GrafoMatriz));
    grafo->numVertices = numVertices;
    grafo->matriz = (double **)malloc(numVertices * sizeof(double *));
    for (int i = 0; i < numVertices; i++) {
        grafo->matriz[i] = (double *)calloc(numVertices, sizeof(double));
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
void adicionarArestaMatriz(GrafoMatriz *grafoMatriz, int u, int v, double peso) {
    grafoMatriz->matriz[u][v] = peso;
    grafoMatriz->matriz[v][u] = peso;
}
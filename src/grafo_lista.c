#include "../include/grafo_lista.h"
#include <stdlib.h>

GrafoLista *criarGrafoLista(int numVertices) {
    GrafoLista *grafo = (GrafoLista *)malloc(sizeof(GrafoLista));
    grafo->numVertices = numVertices;
    grafo->listaAdj = (No **)malloc(numVertices * sizeof(No *));
    for (int i = 0; i < numVertices; i++) {
        grafo->listaAdj[i] = NULL;
    }
    return grafo;
}

void liberarGrafoLista(GrafoLista *grafoLista) {
    for (int i = 0; i < grafoLista->numVertices; i++) {
        No *atual = grafoLista->listaAdj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafoLista->listaAdj);
    free(grafoLista);
}

// Função para adicionar uma aresta na lista de adjacência
void adicionarArestaLista(GrafoLista *grafoLista, int u, int v) {
    No *novoNoU = (No *)malloc(sizeof(No));
    novoNoU->vertice = v;
    novoNoU->prox = grafoLista->listaAdj[u];
    grafoLista->listaAdj[u] = novoNoU;

    No *novoNoV = (No *)malloc(sizeof(No));
    novoNoV->vertice = u;
    novoNoV->prox = grafoLista->listaAdj[v];
    grafoLista->listaAdj[v] = novoNoV;
}
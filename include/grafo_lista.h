#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

typedef struct No {
    int vertice;
    double peso; // novo campo pro peso
    struct No *prox;
} No;

typedef struct {
    No **listaAdj;
    int numVertices;
} GrafoLista;

// Funções para manipulação do grafo com lista de adjacência
GrafoLista *criarGrafoLista(int numVertices);
void liberarGrafoLista(GrafoLista *grafoLista);
void adicionarArestaLista(GrafoLista *grafoLista, int u, int v, double peso); 

#endif // GRAFO_LISTA_H

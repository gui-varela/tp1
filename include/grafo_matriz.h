#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

typedef struct {
    double **matriz; //alterei de inteiro pra double pra armazenar os pesos do tipo double
    int numVertices;
} GrafoMatriz;

// Funções para manipulação do grafo com matriz de adjacência
GrafoMatriz *criarGrafoMatriz(int numVertices);
void liberarGrafoMatriz(GrafoMatriz *grafoMatriz);
void adicionarArestaMatriz(GrafoMatriz *grafoMatriz, int u, int v, double peso);

#endif // GRAFO_MATRIZ_H

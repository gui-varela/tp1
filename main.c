#include "include/grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <nome do arquivo> <tipo de representação (matriz ou lista)>\n", argv[0]);
        return 1;
    }

    // Determine the representation type
    TipoRepresentacao tipo;
    if (strcmp(argv[2], "matriz") == 0) {
        tipo = MATRIZ_ADJACENCIA;
    } else if (strcmp(argv[2], "lista") == 0) {
        tipo = LISTA_ADJACENCIA;
    } else {
        printf("Tipo de representação inválido. Use 'matriz' ou 'lista'.\n");
        return 1;
    }

    // Open the file to read the number of vertices
    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int numVertices;
    if (fscanf(arquivo, "%d", &numVertices) != 1) {
        printf("Erro ao ler o número de vértices.\n");
        fclose(arquivo);
        return 1;
    }
    fclose(arquivo);

    // Create the graph with the number of vertices read
    Grafo *grafo = criarGrafo(numVertices, tipo);

    if (grafo) {
        // Read the edges from the file and populate the graph
        lerArestas(grafo, argv[1]);

        // Create the output file name by appending "_out"
        char nomeArquivoSaida[256];
        snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s-%s_out", argv[1], argv[2]);

        // Open the output file to write the results
        FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
        if (!arquivoSaida) {
            printf("Erro ao criar o arquivo de saída.\n");
            liberarGrafo(grafo);
            return 1;
        }

        // Calculate graph statistics and write to the output file
        EstatisticasGrafo *stats = calcularEstatisticasGrafo(grafo);
        fprintf(arquivoSaida, "Número de arestas: %d\n", stats->numArestas);
        fprintf(arquivoSaida, "Grau mínimo: %d\n", stats->grauMinimo);
        fprintf(arquivoSaida, "Grau máximo: %d\n", stats->grauMaximo);
        fprintf(arquivoSaida, "Grau médio: %.2f\n", stats->grauMedio);
        fprintf(arquivoSaida, "Mediana do grau: %d\n", stats->medianaGrau);

        // Execute DFS and write the result to the output file
        int *visitadosDFS = (int *)calloc(numVertices, sizeof(int));
        int *paisDFS = (int *)malloc(numVertices * sizeof(int));
        int *niveisDFS = (int *)malloc(numVertices * sizeof(int));
        Grafo *arvoreDFS = criarGrafoVazio(numVertices, tipo);

        fprintf(arquivoSaida, "\nExecutando DFS com pilha a partir do vértice 1:\n");
        dfsComPilhaArvore(grafo, 0, visitadosDFS, arvoreDFS, paisDFS, niveisDFS, arquivoSaida);

        // Free allocated memory for DFS
        free(visitadosDFS);
        free(paisDFS);
        free(niveisDFS);
        liberarGrafo(arvoreDFS);

        // Execute BFS and write the result to the output file
        int *visitadosBFS = (int *)calloc(numVertices, sizeof(int));
        int *paisBFS = (int *)malloc(numVertices * sizeof(int));
        int *niveisBFS = (int *)malloc(numVertices * sizeof(int));
        Grafo *arvoreBFS = criarGrafoVazio(numVertices, tipo);

        fprintf(arquivoSaida, "\nExecutando BFS a partir do vértice 1:\n");
        bfsComFilaArvore(grafo, 0, visitadosBFS, arvoreBFS, paisBFS, niveisBFS, arquivoSaida);

        // Free allocated memory for BFS
        free(visitadosBFS);
        free(paisBFS);
        free(niveisBFS);
        liberarGrafo(arvoreBFS);

        // Free allocated memory and close the output file
        liberarGrafo(grafo);
        free(stats);
        fclose(arquivoSaida);
    } else {
        printf("Erro ao carregar o grafo.\n");
        return 1;
    }

    return 0;
}

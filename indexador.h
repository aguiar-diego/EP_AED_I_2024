#ifndef INDEXADOR_H
#define INDEXADOR_H

#define MAX_PALAVRA 100
#define MAX_OCORRENCIAS 1000

typedef struct Palavra {
    char texto[MAX_PALAVRA];
    int ocorrencias;
    int linhas[MAX_OCORRENCIAS];
    struct Palavra *prox;
} Palavra;

typedef struct Lista {
    Palavra *inicio;
    int numero_linhas;
    int total_palavras;
} Lista;

// Declarações das funções
Lista *carregar_e_indexar(const char *nome_arquivo);
void buscar_palavra(Lista *lista, const char *palavra);
void liberar_lista(Lista *lista);
int obter_numero_linhas(Lista *lista);
int obter_total_palavras(Lista *lista);

#endif

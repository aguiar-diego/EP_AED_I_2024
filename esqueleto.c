#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include <time.h>

#include "indexador.h"

#define TAMANHO 10000

typedef struct No_arvore {
    char *palavra;       
    int *linhas;          
    int ocorrencias;      
    int num_linhas;           
    struct No_arvore *esq;    
    struct No_arvore *dir;   
}No_arvore;

typedef struct No_lista {
    char *palavra;
    int *linhas;
    int ocorrencias;
    int num_linhas;
    struct No_lista *prox;
} No_lista;

No_arvore* criar_no_arvore(char *palavra, int linha);
int insere_arvore(No_arvore** current, char* palavra, int linha);
No_arvore *busca_arvore(No_arvore *current, char *palavra);
int altura(No_arvore * no);

No_lista *criar_no_lista(char *palavra, int linha);
int insere_lista(No_lista **current, char *palavra, int linha);
No_lista *busca_lista(No_lista *current, char *palavra);

void remover_pontuacao(char *palavra);
void strtolower(char *palavra);
void main_arvore(char * filename);
void main_lista(char * filename);

int num_palavras_arvore(No_arvore *raiz);
int num_palavras_lista(No_lista *lista);

void print_arvore(No_arvore *raiz, char *palavra, char **linhas_texto);
void print_lista(No_lista *lista, char *palavra, char **linhas_texto);


int main(int argc, char ** argv){
	 



	if(argc == 3) {
        char *filename = argv[1];
        char *tipo = argv[2];
        int linhas_max = 1000;
        No_arvore *raiz = NULL;
        No_lista *lista = NULL;
        char linha[TAMANHO];
        int num_linha = 1;
        char **linhas_texto = malloc( linhas_max * sizeof(char *));
        int num_palavras = 0;
        struct timespec inicio, fim;

        clock_gettime(CLOCK_MONOTONIC, &inicio);

		FILE *arquivo = fopen(filename, "r");
        if(arquivo == NULL){
            printf("Erro ao abrir o arquivo\n");
            return 1;
        }

        while(fgets(linha, TAMANHO, arquivo) != NULL){
            if(num_linha >= linhas_max){
                linhas_max *= 2;
                linhas_texto = realloc(linhas_texto, linhas_max * sizeof(char *));
            }
            linhas_texto[num_linha - 1] = strdup(linha);
            char *palavra = strtok(linha, " ");
            while(palavra != NULL){
                remover_pontuacao(palavra);
                strtolower(palavra);
                if(strcmp(tipo, "arvore") == 0) num_palavras += insere_arvore(&raiz, palavra, num_linha);
                else if(strcmp(tipo, "lista") == 0) num_palavras += insere_lista(&lista, palavra, num_linha);
                palavra = strtok(NULL, " ");
            }
            num_linha++;
        }
        fclose(arquivo);
        clock_gettime(CLOCK_MONOTONIC, &fim);
        long seconds = fim.tv_sec - inicio.tv_sec;
        long nanoseconds = fim.tv_nsec - inicio.tv_nsec;
        double elapsed = seconds + nanoseconds*1e-9;
        elapsed *=1000;
        int alt = altura(raiz);

        printf("Arquivo: %s\n", filename);
        printf("Tipo de indice: '%s'\n", tipo);
        printf("Numero de linhas no arquivo: %d\n", num_linha - 1);
        printf("Numero de palavras indexadas: %d\n", num_palavras);
        if(strcmp(tipo, "arvore") == 0) printf("Altura da arvore: %d\n", alt);
        printf("Tempo de carga do arquivo e construcao do indice: %f ms\n", elapsed);


        printf("> ");
        char comando[100];
        char buscada[100];

        scanf("%s", comando);
        while(strcmp(comando, "fim") != 0){
            if(strcmp(comando, "busca") == 0){
                scanf("%s", buscada);
                clock_gettime(CLOCK_MONOTONIC, &inicio);
                if(strcmp(tipo, "arvore") == 0) print_arvore(raiz, buscada, linhas_texto);
                else if(strcmp(tipo, "lista") == 0) print_lista(lista, buscada, linhas_texto);
                else printf("Tipo de indice invalido\n");
                clock_gettime(CLOCK_MONOTONIC, &fim);
                seconds = fim.tv_sec - inicio.tv_sec;
                nanoseconds = fim.tv_nsec - inicio.tv_nsec;
                elapsed = seconds + nanoseconds*1e-9;
                elapsed *=1000;
                printf("Tempo de busca: %f ms\n", elapsed);            
            }
            else{
                printf("Opcao Invalida\n");
            }
            printf("> ");
            scanf("%s", comando);
        }
	}

	return 1;
}
void print_arvore(No_arvore *raiz, char *palavra, char **linhas_texto){
    No_arvore *no = busca_arvore(raiz, palavra);
    if(no){
        printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n", no->ocorrencias, no->palavra);
        int ultima = -1;
        for(int i = 0; i < no->num_linhas; i++){
            if(no->linhas[i] != ultima){
                printf("%05d: %s\n", no->linhas[i], linhas_texto[no->linhas[i] - 1]);
                ultima = no->linhas[i];
            }
        }
    }
    else{
        printf("Palavra '%s' nao encontrada\n", palavra);
    }
}

void print_lista(No_lista *lista, char *palavra, char **linhas_texto){
    No_lista *no = busca_lista(lista, palavra);
    if(no){
        printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n", no->ocorrencias, no->palavra);
        for(int i = 0; i < no->num_linhas; i++){
            printf("%05d: %s\n", no->linhas[i], linhas_texto[no->linhas[i] - 1]);
        }
    }
    else{
        printf("Palavra '%s' nao encontrada\n", palavra);
    }
}

int num_palavras_arvore(No_arvore *raiz){
    if(raiz == NULL){
        return 0;
    }
    return 1 + num_palavras_arvore(raiz->esq) + num_palavras_arvore(raiz->dir);
}
int num_palavras_lista(No_lista *lista){
    int num = 0;
    while(lista){
        num++;
        lista = lista->prox;
    }
    return num;
}



No_arvore* criar_no_arvore(char *palavra, int linha){
    No_arvore *novo = (No_arvore *) malloc(sizeof(No_arvore));
    novo->palavra = strdup(palavra);
    novo->linhas = (int *) malloc(2 * sizeof(int));
    novo->linhas[0] = linha;
    novo->num_linhas = 1;
    novo->ocorrencias = 1;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

int insere_arvore(No_arvore** current, char* palavra, int linha){
    if (*current == NULL) {
        *current = criar_no_arvore(palavra, linha);
        return 1;
    }
	else{
        
		int cmp = strcmp(palavra, (*current)->palavra);
		if (cmp < 0) return insere_arvore(&(*current)->esq, palavra, linha);
        
		else if (cmp > 0) return insere_arvore(&(*current)->dir, palavra, linha);
        
		else {
           
		    if ((*current)->num_linhas == (*current)->ocorrencias) {
                (*current)->linhas = realloc((*current)->linhas, (*current)->num_linhas * 2 * sizeof(int));
            }
			
            (*current)->linhas[(*current)->num_linhas] = linha;
            (*current)->num_linhas++;
            (*current)->ocorrencias++;
            return 0;
        }
    }
}

No_arvore *busca_arvore(No_arvore *current, char *palavra){
    if(current == NULL){
        return NULL;
    }
    if(strcmp(palavra, current->palavra) < 0){
        return busca_arvore(current->esq, palavra);
    }
    else if(strcmp(palavra, current->palavra) > 0){
        return busca_arvore(current->dir, palavra);
    }
    else{
        return current;
    }
}

int altura(No_arvore * no){
    if(no == NULL){
        return -1;
    }

    int hEsq = altura(no->esq);
    int hDir = altura(no->dir);

    if(hEsq>hDir) return hEsq +1;
    
    else return hDir +1 ; 
}

void remover_pontuacao(char *palavra) {
    
    int i = 0;
    int j = 0;
    while (palavra[i]) {
        if (isalpha(palavra[i])) {
            palavra[j] = palavra[i];
            j++;
        }
        i++;
    }
    palavra[j] = '\0';
}

void strtolower(char *palavra) {
    for (int i = 0; i<strlen(palavra); i++) {
        palavra[i] = tolower(palavra[i]);
    }
}


No_lista *criar_no_lista(char *palavra, int linha){
    No_lista *novo = (No_lista *) malloc(sizeof(No_lista));
    novo->palavra = strdup(palavra);
    novo->linhas = (int *) malloc(sizeof(int));
    novo->linhas[0] = linha;
    novo->ocorrencias = 1;
    novo->num_linhas = 1;
    novo->prox = NULL;

    return novo;
}

int insere_lista(No_lista **current, char *palavra, int linha){
    No_lista *atual = *current;
    while(atual){
        if(strcmp(atual->palavra, palavra) == 0){
            if(atual->num_linhas == atual->ocorrencias){
                atual->linhas = realloc(atual->linhas, atual->num_linhas * 2 * sizeof(int));
            }
            atual->linhas[atual->ocorrencias] = linha;
            atual->num_linhas++;
            atual->ocorrencias++;
            return 0;
        }
        atual = atual->prox;
    }

    No_lista *novo = criar_no_lista(palavra, linha);
    novo->prox = *current;
    *current = novo;
    return 1;
}

No_lista *busca_lista(No_lista *current, char *palavra){
    while(current){
        if(strcmp(current->palavra, palavra) == 0){
            return current;
        }
        current = current->prox;
    }
    return NULL;
}


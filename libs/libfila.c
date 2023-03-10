#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

/*
 * Cria uma fila vazia e a retorna, se falhar retorna NULL.
 */
fila_t *cria_fila()
{
    fila_t *fila;

    if (!(fila = (fila_t *)malloc(sizeof(fila_t))))
        return NULL;

    fila->tamanho = 0;

    return fila;
}

/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
 */
fila_t *destroi_fila(fila_t *f)
{
    int i;
    nodo_f_t *nodo, *aux;

    nodo = f->ini;

    for (i = 0; i < f->tamanho; i++)
    {
        aux = nodo->prox;
        free(nodo);
        nodo = aux;
    }

    free(f);
    f = NULL;

    return NULL;
}

/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
 */
int fila_vazia(fila_t *f)
{
    return f->tamanho == 0;
}

/*
 * Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela.
 */
int tamanho_fila(fila_t *f)
{
    return f->tamanho;
}

/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int insere_fila(fila_t *f, int elemento)
{
    nodo_f_t *nodo;

    if (!(nodo = (nodo_f_t *)malloc(sizeof(nodo_f_t))))
        return 0;

    nodo->chave = elemento;

    if (fila_vazia(f))
    {
        f->ini = nodo;
        f->fim = nodo;
        f->tamanho++;

        return 1;
    }

    f->fim->prox = nodo;
    f->fim = nodo;
    f->tamanho++;
    
    return 1;
}

/*
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int retira_fila(fila_t *f, int *elemento)
{
    nodo_f_t *aux;

    if (fila_vazia(f))
        return 0;
        
    *elemento = f->ini->chave;
    aux = f->ini;
    f->ini = f->ini->prox;
    
    free(aux);
    aux = NULL;

    f->tamanho--;
    
    return 1;
}

/* para depuracao */
void imprime_fila(fila_t *f)
{
    int i;
    nodo_f_t *aux;

    aux = f->ini;
    
    for (i = 1; i <= f->tamanho; i++)
    {
        printf("%d", aux->chave);
    
        if (i < f->tamanho)
        {
            aux = aux->prox;
            printf(" ");
        }
    }
    
    printf("\n");
}

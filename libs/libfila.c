#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

/*
 * Cria uma fila vazia e a retorna, se falhar retorna NULL.
 */
fila_t *cria_fila()
{
    fila_t *fila;
    if (!(fila = malloc(sizeof(fila_t))))
        return NULL;
    fila->tamanho = 0;
    return fila;
}

/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
 */
fila_t *destroi_fila(fila_t *f)
{
    nodo_f_t *nodo, *nodo2;
    int i;
    nodo = f->ini;
    for (i = 0; i < f->tamanho; i++)
    {
        nodo2 = nodo->prox;
        free(nodo);
        nodo = nodo2;
    }
    free(f);
    return NULL;
}

/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
 */
int fila_vazia(fila_t *f)
{
    if (f->tamanho == 0)
        return 1;
    return 0;
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

    if (!(nodo = malloc(sizeof(nodo_f_t))))
        return 0;
    nodo->chave = elemento;

    if (f->tamanho == 0)
    {
        f->ini = nodo;
        f->fim = nodo;
        f->tamanho++;
        return 1;
    }

    (f->fim)->prox = nodo;
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
    nodo_f_t *nodo2;

    if (f->tamanho == 0)
        return 0;
    *elemento = (f->ini)->chave;
    nodo2 = f->ini;
    f->ini = (f->ini)->prox;
    free(nodo2);
    f->tamanho--;
    return 1;
}

/* para depuracao */
void imprime_fila(fila_t *f)
{
    nodo_f_t *nodo2;
    int i;
    nodo2 = f->ini;
    for (i = 1; i <= f->tamanho; i++)
    {
        printf("%d", nodo2->chave);
        if (i < f->tamanho)
        {
            nodo2 = nodo2->prox;
            printf(" ");
        }
    }
    printf("\n");
}

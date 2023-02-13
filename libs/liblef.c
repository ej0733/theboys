#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"

/* A  LEF é uma lista que armazena eventos ordenados pelo membro tempo
 * do tipo evento_t. Cada evento pode ter um tipo e armazenar em dados
 * diferentes estruturas de dados que permitem descrever cada evento.
 */

/*
 * Cria uma LEF vazia
 */
lef_t *cria_lef()
{
  lef_t *lef;

  if (!(lef = (lef_t *)malloc(sizeof(lef_t))))
    return NULL;

  lef->Primeiro = NULL;

  return lef;
}

/*
 * Destroi a LEF e retorna NULL.
 */
lef_t *destroi_lef(lef_t *l)
{
  nodo_lef_t *nodo, *aux;

  nodo = l->Primeiro;

  while (nodo != NULL)
  {
    aux = nodo->prox;

    free(nodo->evento);
    free(nodo);

    nodo = aux;
  }

  free(l);
  l = NULL;

  return NULL;
}

/*
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita.
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_inicio_lef(lef_t *l, evento_t *evento)
{
  nodo_lef_t *nodo;

  if (!(nodo = (nodo_lef_t *)malloc(sizeof(nodo_lef_t))))
    return 0;

  if (!(nodo->evento = (evento_t *)malloc(sizeof(evento_t))))
    return 0;

  memcpy(nodo->evento, evento, sizeof(evento_t));

  nodo->prox = l->Primeiro;
  l->Primeiro = nodo;

  return 1;
}

nodo_lef_t *aux_ordem_lef(nodo_lef_t *nodo, nodo_lef_t *nodo2, nodo_lef_t *nodo3)
{
  if (nodo3 == NULL || nodo3->evento->tempo > nodo->evento->tempo)
  {
    nodo->prox = nodo3;
    nodo2->prox = nodo;

    return nodo2;
  }

  nodo2->prox = aux_ordem_lef(nodo, nodo2->prox, nodo3->prox);

  return nodo2;
}

/*
 * Adiciona um evento de acordo com o valor evento->tempo na LEF.
 * Uma nova copia da estrutura evento sera feita
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef(lef_t *l, evento_t *evento)
{
  nodo_lef_t *nodo, *nodo2, *nodo3;

  if (!(nodo = (nodo_lef_t *)malloc(sizeof(nodo_lef_t))))
    return 0;

  if (!(nodo->evento = (evento_t *)malloc(sizeof(evento_t))))
    return 0;

  nodo2 = l->Primeiro;
  memcpy(nodo->evento, evento, sizeof(evento_t));

  if (nodo2 == NULL || nodo2->evento->tempo > nodo->evento->tempo)
  {
    nodo->prox = l->Primeiro;
    l->Primeiro = nodo;

    return 1;
  }

  nodo3 = nodo2->prox;
  l->Primeiro = aux_ordem_lef(nodo, nodo2, nodo3);

  return 1;
}

/*
 * Retorna e retira o primeiro evento da LEF.
 * A responsabilidade por desalocar
 * a memoria associada eh de quem chama essa funcao.
 */
evento_t *obtem_primeiro_lef(lef_t *l)
{
  nodo_lef_t *nodo;
  evento_t *evento;

  if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
    return 0;

  nodo = l->Primeiro;

  memcpy(evento, nodo->evento, sizeof(evento_t));

  l->Primeiro = nodo->prox;

  free(nodo->evento);
  free(nodo);

  nodo = NULL;

  return evento;
}

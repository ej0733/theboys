#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libconjunto.h"

/*
 * Cria um conjunto vazio e o retorna, se falhar retorna NULL.
 * max eh o tamanho maximo do conjunto, isto eh, o tamanho maximo do vetor
 */
conjunto_t *cria_cjt(int max)
{

    conjunto_t *c;

    if (!(c = (conjunto_t *)malloc(sizeof(conjunto_t))))
        return NULL;

    if (!(c->v = (int *)malloc(sizeof(int) * (max))))
        return NULL;

    c->max = max;
    c->card = 0;

    return c;
}

/*
 * Remove todos os elementos do conjunto, libera espaco e devolve NULL.
 */
conjunto_t *destroi_cjt(conjunto_t *c)
{
    free(c->v);
    free(c);

    c = NULL;

    return NULL;
}

/*
 * Retorna 1 se o conjunto esta vazio e 0 caso contrario.
 */
int vazio_cjt(conjunto_t *c)
{
    return (cardinalidade_cjt(c) == 0);
}

/*
 * Retorna a cardinalidade do conjunto, isto eh, o numero de elementos presentes nele.
 */
int cardinalidade_cjt(conjunto_t *c)
{
    return (c->card);
}

/*
 *Busca binaria no conjunto, retorna a posição onde o elemento está,
 *Ou estaria se estivesse no conjunto.
 */
int busca_cjt(conjunto_t *c, int elemento)
{
    int inicio, fim, i;

    inicio = 0;
    fim = (cardinalidade_cjt(c) - 1);

    do
    {
        i = (inicio + fim) / 2;

        if (c->v[i] > elemento)
            fim = i - 1;
        else
            inicio = i + 1;
    } while (inicio <= fim);

    if (inicio > 0)
    {
        if (c->v[inicio - 1] == elemento)
            return inicio - 1;
    }

    return inicio;
}

/*
 * Insere o elemento no conjunto, garante que nao existam repeticoes.
 * Retorna 1 se a operacao foi bem sucedida. Se tentar inserir elemento existente,
 * nao faz nada e retorna 1 tambem. Retorna 0 em caso de falha por falta de espaco.
 */
int insere_cjt(conjunto_t *c, int elemento)
{
    int indice, i;

    if (cardinalidade_cjt(c) >= (c->max))
        return 0;

    if (vazio_cjt(c))
    {
        *(c->v) = elemento;
        c->card = 1;

        return 1;
    }

    indice = busca_cjt(c, elemento);

    if (indice < cardinalidade_cjt(c))
    {
        if (c->v[indice] == elemento)
            return 1;

        for (i = cardinalidade_cjt(c); i > indice; i--)
            c->v[i] = c->v[i - 1];
    }

    c->v[indice] = elemento;
    c->card = c->card + 1;

    return 1;
}

/*
 * Remove o elemento 'elemento' do conjunto caso ele exista.
 * Retorna 1 se a operacao foi bem sucedida e 0 se o elemento nao existe.
 */
int retira_cjt(conjunto_t *c, int elemento)
{
    int i, indice;

    if (cardinalidade_cjt(c) == 0)
        return 0;

    indice = busca_cjt(c, elemento);

    if (indice < c->card)
    {
        if (c->v[indice] != elemento)
            return 0;

        for (i = indice; i <= (c->card - 2); i++)
            c->v[i] = c->v[i + 1];

        c->card--;

        return 1;
    }

    return 0;
}

/*
 * Retorna 1 se o elemento pertence ao conjunto e 0 caso contrario.
 */
int pertence_cjt(conjunto_t *c, int elemento)
{
    int indice;

    indice = busca_cjt(c, elemento);

    if (indice >= cardinalidade_cjt(c) || c->v[indice] != elemento)
        return 0;

    return 1;
}

/*
 * Retorna 1 se o conjunto c1 esta contido no conjunto c2 e 0 caso contrario.
 */
int contido_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;

    if (cardinalidade_cjt(c1) > cardinalidade_cjt(c2))
        return 0;

    for (i = 0; i < cardinalidade_cjt(c1); i++)
        if (!(pertence_cjt(c2, c1->v[i])))
            return 0;

    return 1;
}

/*
 * Retorna 1 se o conjunto c1 eh igual ao conjunto c2 e 0 caso contrario.
 */
int sao_iguais_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;

    if (cardinalidade_cjt(c1) != cardinalidade_cjt(c2))
        return 0;

    for (i = 0; i < c1->card; i++)
        if (c1->v[i] != c2->v[i])
            return 0;

    return 1;
}

/*
 * Cria e retorna o conjunto diferenca entre c1 e c2, nesta ordem.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *diferenca_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    conjunto_t *diferenca;

    diferenca = copia_cjt(c1);

    for (i = 0; i < cardinalidade_cjt(c2); i++)
        retira_cjt(diferenca, c2->v[i]);

    return diferenca;
}

/*
 * Cria e retorna o conjunto interseccao entre os conjuntos c1 e c2.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *interseccao_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    conjunto_t *interseccao;

    if (!(interseccao = cria_cjt(cardinalidade_cjt(c1) + cardinalidade_cjt(c2))))
        return NULL;

    for (i = 0; i < cardinalidade_cjt(c2); i++)
    {
        if (pertence_cjt(c1, c2->v[i]))
            insere_cjt(interseccao, c2->v[i]);
    }
    return interseccao;
}

/*
 * Cria e retorna o conjunto uniao entre os conjunto c1 e c2.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *uniao_cjt(conjunto_t *c1, conjunto_t *c2)
{
    int i;
    conjunto_t *uniao;

    uniao = copia_cjt(c1);

    if (uniao == NULL)
    {
        free(uniao);
        uniao = copia_cjt(c2);

        if (uniao == NULL)
            return NULL;

        return uniao;
    }

    for (i = 0; i < cardinalidade_cjt(c2); i++)
        insere_cjt(uniao, c2->v[i]);

    return uniao;
}

/*
 * Cria e retorna uma copia do conjunto c e NULL em caso de falha.
 */
conjunto_t *copia_cjt(conjunto_t *c)
{
    int i;
    conjunto_t *copia;

    if ((copia = cria_cjt(c->max)) == (NULL))
        return NULL;

    for (i = 0; i < cardinalidade_cjt(c); i++)
        insere_cjt(copia, c->v[i]);

    return copia;
}

/*
 * Cria e retorna um subconjunto com elementos aleatorios do conjunto c.
 * Se o conjunto for vazio, retorna um subconjunto vazio.
 * Se n >= cardinalidade (c) entao retorna o proprio conjunto c.
 * Supoe que a funcao srand () tenha sido chamada antes.
 */
conjunto_t *cria_subcjt_cjt(conjunto_t *c, int n)
{
    int posicao_aleatoria, max;
    conjunto_t *subcjt;

    max = cardinalidade_cjt(c) - 1;

    if (n >= cardinalidade_cjt(c))
    {
        subcjt = copia_cjt(c);

        return subcjt;
    }

    subcjt = cria_cjt(n);
    
    while (cardinalidade_cjt(subcjt) < n)
    {
        posicao_aleatoria = 0 + rand() % (max);
        insere_cjt(subcjt, c->v[posicao_aleatoria]);
    }
    
    return subcjt;
}

/*
 * Imprime os elementos do conjunto sempre em ordem crescente,
 * mesmo que a estrutura interna nao garanta isso.
 * Na impressao os elementos sao separados por um unico espaco
 * em branco entre os elementos, sendo que apos o ultimo nao
 * deve haver espacos em branco. Ao final imprime um \n.
 */
void imprime_cjt(conjunto_t *c)
{
    int i;

    if (cardinalidade_cjt(c) == 0)
        printf("conjunto vazio");
    else
    {
        for (i = 0; i < cardinalidade_cjt(c); i++)
        {
            printf("%d", c->v[i]);

            if (i < cardinalidade_cjt(c) - 1)
                printf(" ");
        }
    }

    printf("\n");
}

/*
 * As funcoes abaixo implementam um iterador que vao permitir
 * percorrer os elementos do conjunto.
 * O ponteiro ptr da struct (iterador) pode ser inicializado para apontar
 * para o primeiro elemento e incrementado ate' o ultimo elemento
 * do conjunto.
 */

/*
 * Inicializa ptr usado na funcao incrementa_iterador
 */
void inicia_iterador_cjt(conjunto_t *c)
{
    c->ptr = 0;
}

/*
 * Devolve no parametro ret_iterador o elemento apontado e incrementa o iterador.
 * A funcao retorna 0 caso o iterador ultrapasse o ultimo elemento, ou retorna
 * 1 caso o iterador aponte para um elemento valido (dentro do conjunto).
 */
int incrementa_iterador_cjt(conjunto_t *c, int *ret_iterador)
{
    if (c->ptr > cardinalidade_cjt(c) - 1)
        return 0;

    *ret_iterador = c->v[c->ptr];
    c->ptr++;

    return 1;
}

/*
 * Escolhe um elemento qualquer do conjunto para ser removido, o remove e
 * o retorna.
 * Nao faz teste se conjunto eh vazio, deixa para main fazer isso
 */
int retira_um_elemento_cjt(conjunto_t *c)
{
    int elemento = *(c->v);

    retira_cjt(c, elemento);
    
    return elemento;
}

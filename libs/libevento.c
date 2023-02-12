#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libevento.h"

int calcular_tpl(int paciencia)
{
    return (max(1, (paciencia / 10) + (aleat(-2, 6))));
}

int calcular_tdl(mundo_t *mundo, int id_local, int id_local_dest, int id_heroi)
{
    int distancia, velocidade, x1, x2, y1, y2;

    x1 = mundo->locais[id_local]->locallizacao->x;
    x2 = mundo->locais[id_local_dest]->locallizacao->x;
    y1 = mundo->locais[id_local]->locallizacao->y;
    y2 = mundo->locais[id_local_dest]->locallizacao->y;

    distancia = sqrt(pow(x2 - x1, 2) + pow((y2 - y1), 2));
    velocidade = 100 - max(0, mundo->herois[id_heroi]->idade - 40);

    return ((distancia / velocidade) / 15);
}

conjunto_t **instanciar_vetor_cjt(int tamanho, int tamanho2)
{
    int i;
    conjunto_t **vetor_cjt;

    if (!(vetor_cjt = (conjunto_t **)malloc(sizeof(conjunto_t) * tamanho)))
        return NULL;

    for (i = 0; i < tamanho; i++)
        vetor_cjt[i] = cria_cjt(tamanho2);

    return vetor_cjt;
}

conjunto_t **destruir_vetor_cjt(conjunto_t **vetor_cjt, int tamanho)
{
    int i;

    for (i = 0; i < tamanho; i++)
        vetor_cjt[i] = destroi_cjt(vetor_cjt[i]);

    free(vetor_cjt);
    vetor_cjt = NULL;

    return NULL;
}

int tratar_evento_chegada(lef_t *lef, evento_t *evento, mundo_t *mundo)
{
    evento_t *evento_saida;
    int tpl, num_herois_no_lugar, num_lotacao_max, paciencia_heroi, num_tamanho_fila;

    num_herois_no_lugar = mundo->locais[evento->dado2]->herois_no_lugar;
    num_lotacao_max = mundo->locais[evento->dado2]->lotacao_maxima;
    paciencia_heroi = mundo->herois[evento->dado1]->paciencia;
    num_tamanho_fila = tamanho_fila(mundo->locais[evento->dado2]->fila);

    if (num_herois_no_lugar == num_lotacao_max)
    {

        if (paciencia_heroi / 4 - num_tamanho_fila > 0)
        {
            insere_fila(mundo->locais[evento->dado2]->fila, evento->dado1);
            printf("FILA %2d\n", tamanho_fila(mundo->locais[evento->dado2]->fila));

            return SUCCESS;
        }
        else
        {
            printf("DESISTE\n");

            if (!(evento_saida = (evento_t *)malloc(sizeof(evento_t))))
            {
                printf("ERRO: Não foi possível alocar memória para o evento de saída.\n");
                return FAILURE;
            }

            evento_saida->tipo = TIPO_SAIDA;
            evento_saida->dado1 = evento->dado1;
            evento_saida->dado2 = evento->dado2;
            evento_saida->tempo = mundo->tempo_atual;

            if (!(adiciona_ordem_lef(lef, evento_saida)))
            {
                printf("ERRO: Houve um erro ao adicionar o evento de saída.\n");
                return FAILURE;
            }

            free(evento_saida);
            evento_saida = NULL;

            return SUCCESS;
        }
    }

    printf("ENTRA\n");
    mundo->locais[evento->dado2]->herois_no_lugar++;

    if (!(insere_cjt(mundo->locais[evento->dado2]->herois, evento->dado1)))
    {
        printf("ERRO: Houve um erro ao adicionar o herói ao conjunto de heróis do local.\n");
        return FAILURE;
    }

    if (!(evento_saida = (evento_t *)malloc(sizeof(evento_t))))
    {
        printf("ERRO: Não foi possível alocar memória para o evento de saída.\n");
        return FAILURE;
    }

    evento_saida->tipo = TIPO_SAIDA;
    evento_saida->dado1 = evento->dado1;
    evento_saida->dado2 = evento->dado2;
    tpl = calcular_tpl(mundo->herois[evento->dado1]->paciencia);
    evento_saida->tempo = mundo->tempo_atual + tpl;
    ;

    if (!(adiciona_ordem_lef(lef, evento_saida)))
    {
        printf("ERRO: Houve um erro ao adicionar o evento de saída.\n");
        return FAILURE;
    }

    free(evento_saida);
    evento_saida = NULL;

    return SUCCESS;
}

int tratar_evento_saida(lef_t *lef, evento_t *evento, mundo_t *mundo)
{
    int *elemento, tdl;
    evento_t *evento_chegada;

    if (!(evento_chegada = (evento_t *)malloc(sizeof(evento_t))))
    {
        printf("ERRO: Não foi possível alocar memória para o evento de chegada.\n");
        return FAILURE;
    }

    evento_chegada->tipo = TIPO_CHEGADA;
    evento_chegada->dado1 = evento->dado1;
    evento_chegada->dado2 = aleat(0, mundo->n_locais - 1);
    tdl = calcular_tdl(mundo, evento->dado2, evento_chegada->dado2, evento_chegada->dado1);
    evento_chegada->tempo = mundo->tempo_atual + tdl;
    ;

    if (!(adiciona_ordem_lef(lef, evento_chegada)))
    {
        printf("ERRO: Houve um erro ao adicionar o evento de chegada.\n");
        return FAILURE;
    }

    free(evento_chegada);
    evento_chegada = NULL;

    if (pertence_cjt((mundo->locais[evento->dado2]->herois), evento->dado1))
    {
        if (tamanho_fila(mundo->locais[evento->dado2]->fila) > 0)
        {
            if (!(evento_chegada = (evento_t *)malloc(sizeof(evento_t))))
            {
                printf("ERRO: Não foi possível alocar memória para o evento de chegada.\n");
                return FAILURE;
            }

            if (!(elemento = (int *)malloc(sizeof(int))))
            {
                printf("ERRO: Não foi possível alocar memória para o elemento da fila.\n");
                return FAILURE;
            }

            if (!(retira_fila(mundo->locais[evento->dado2]->fila, elemento)))
            {
                printf("ERROR: Houve um problema ao retirar o elemento da fila.\n");
                return FAILURE;
            }

            evento_chegada->tipo = TIPO_CHEGADA;
            evento_chegada->dado1 = *elemento;
            evento_chegada->dado2 = evento->dado2;
            evento_chegada->tempo = mundo->tempo_atual;

            if (!(adiciona_inicio_lef(lef, evento_chegada)))
            {
                printf("ERRO: Houve um problema ao adicionar o evento de chegada.\n");
                return FAILURE;
            }

            printf(", REMOVE FILA HEROI %2d", *elemento);

            free(elemento);
            free(evento_chegada);

            elemento = NULL;
            evento_chegada = NULL;
        }

        mundo->locais[evento->dado2]->herois_no_lugar--;

        if (!(retira_cjt(mundo->locais[evento->dado2]->herois, evento->dado1)))
        {
            printf("ERRO: Houve um erro ao retirar o herói do conjunto de heróis do local.\n");
            return FAILURE;
        }
    }

    printf("\n");
    return SUCCESS;
}

/* procura uma equipe que solucuine a missão
 * devolve -1 caso não exista               */
int encontrar_equipe_missao(int tamanho, local_t **locais, heroi_t **herois, conjunto_t *missao, evento_t *evento)
{
    conjunto_t *uniao_habilidade_cjt, **habilidades_herois_cjt;
    int i, j, card1, card2, posicao_equipe, tamanho_max = N_HABILIDADES;

    /* cria um vetor com a união das habilidades dos herois da equipe*/
    habilidades_herois_cjt = instanciar_vetor_cjt(tamanho, tamanho_max);

    /*faz a união dos conjuntos de habilidades dos herois
     * e guarda os valores no vetor de habilidad cria e retorna uma struct local
    retorna null em caso de falha*/
    if (tamanho > 0)
    {
        for (i = 0; i < tamanho; i++)
        {
            for (j = 0; j < cardinalidade_cjt(locais[i]->herois); j++)
            {
                posicao_equipe = locais[i]->herois->v[j];
                uniao_habilidade_cjt = uniao_cjt(habilidades_herois_cjt[i], herois[posicao_equipe]->habilidades);
                memcpy(habilidades_herois_cjt[i]->v, uniao_habilidade_cjt->v, (tamanho_max * sizeof(int)));
                habilidades_herois_cjt[i]->card = uniao_habilidade_cjt->card;
                destroi_cjt(uniao_habilidade_cjt);
            }
        }
    }

    /* verifica se existe pelo menos uma equipe que possa efetuar a missão*/
    i = 0;
    posicao_equipe = NOT_FOUND;

    while ((i < tamanho) && (posicao_equipe == NOT_FOUND))
    {
        if (contido_cjt(missao, habilidades_herois_cjt[i]))
            posicao_equipe = i;
        i++;
    }

    /* Imprime os conjuntos de habilidades de herois*/
    for (i = 0; i < tamanho; i++)
    { /* cria e retorna uma struct local
       * retorna null em caso de falha*/

        printf("%6d:MISSAO %2d HAB_EQL %d:", evento->tempo, evento->dado1, locais[i]->id);
        imprime_cjt(habilidades_herois_cjt[i]);
    }
    /*caso exista pelo menos uma equipe que possa efetuar a missão
     * confere qual a menor equipe*/
    if (posicao_equipe != NOT_FOUND)
    {
        for (i = posicao_equipe; i < tamanho; i++)
        {
            card1 = locais[i]->herois->card;
            card2 = locais[posicao_equipe]->herois->card;

            if ((contido_cjt(missao, habilidades_herois_cjt[i])) && ((card1 < card2)))
                posicao_equipe = i;
        }
    }

    /* libera o vetor de ponteiros de habilidades dos herois*/
    habilidades_herois_cjt = destruir_vetor_cjt(habilidades_herois_cjt, tamanho);

    return posicao_equipe;
}

int tratar_evento_missao(lef_t *lef, evento_t *evento, mundo_t *mundo, missao_t **missoes)
{
    int posicao_equipe, i, j;
    evento_t *evento_missao;

    /* imprime as habilidades necessárias para a missão*/
    printf("HAB_REQ ");
    imprime_cjt(missoes[evento->dado1]->missao);

    /* verifica se existe um conjunto de heróis que possa realizar a missão
     * retorna a id de 0 até n_locais caso exista e -1 caso contrário*/
    posicao_equipe = encontrar_equipe_missao(mundo->n_locais, mundo->locais, mundo->herois, missoes[evento->dado1]->missao, evento);

    if (posicao_equipe != NOT_FOUND)
    {
        printf("%6d:MISSAO %2d HER_EQS %d:", evento->tempo, evento->dado1, mundo->locais[posicao_equipe]->id);
        imprime_cjt(mundo->locais[posicao_equipe]->herois);

        for (i = 0; i < cardinalidade_cjt(mundo->locais[posicao_equipe]->herois); i++)
        {
            j = mundo->locais[posicao_equipe]->herois->v[i];
            mundo->herois[j]->experiencia++;
        }

        return SUCCESS;
    }

    printf("%6d:MISSAO %2d IMPOSSIVEL\n", evento->tempo, evento->dado1);

    if (!(evento_missao = (evento_t *)malloc(sizeof(evento_t))))
    {
        printf("ERRO: Não foi possível alocar memória para o evento missao.\n");
        return FAILURE;
    }

    evento_missao->tipo = TIPO_MISSAO;
    evento_missao->dado1 = evento->dado1;
    evento_missao->tempo = aleat(mundo->tempo_atual, FIM_DO_MUNDO);

    if (!(adiciona_ordem_lef(lef, evento_missao)))
    {
        printf("ERRO: Houve um problema ao adicionar o evento missao na lista de eventos.\n");
        return FAILURE;
    }

    free(evento_missao);
    evento_missao = NULL;

    return SUCCESS;
}

void imprime_evento_chegada(evento_t *evento, mundo_t *mundo)
{
    int tempo_atual, id_heroi, id_local, herois_local, lotacao_maxima;

    tempo_atual = evento->tempo;
    id_heroi = evento->dado1;
    id_local = evento->dado2;
    herois_local = cardinalidade_cjt(mundo->locais[evento->dado2]->herois);
    lotacao_maxima = mundo->locais[evento->dado2]->lotacao_maxima;

    printf("%6d:CHEGA HEROI %2d LOCAL %d (%2d/%2d), ", tempo_atual, id_heroi, id_local, herois_local, lotacao_maxima);
}

void imprime_evento_saida(evento_t *evento, mundo_t *mundo)
{
    int tempo_atual, id_heroi, id_local, herois_local, lotacao_maxima;

    tempo_atual = evento->tempo;
    id_heroi = evento->dado1;
    id_local = evento->dado2;
    herois_local = cardinalidade_cjt(mundo->locais[evento->dado2]->herois);
    lotacao_maxima = mundo->locais[evento->dado2]->lotacao_maxima;

    printf("%6d:SAIDA HEROI %2d LOCAL %d (%2d/%2d)", tempo_atual, id_heroi, id_local, herois_local, lotacao_maxima);
}

void imprime_evento_missao(evento_t *evento, mundo_t *mundo)
{
    int tempo_atual, id_missao;

    tempo_atual = evento->tempo;
    id_missao = evento->dado1;

    printf("%6d:MISSAO %3d ", tempo_atual, id_missao);
}

void imprime_evento_fim(evento_t *evento, mundo_t *mundo)
{
    int i, tempo_atual, id_heroi, experiencia;

    tempo_atual = evento->tempo;
    printf("%6d:FIM\n", tempo_atual);

    for (i = 0; i < mundo->n_herois; i++)
    {
        id_heroi = mundo->herois[i]->id;
        experiencia = mundo->herois[i]->experiencia;
        printf("HEROI %2d EXPERIENCIA %2d\n", id_heroi, experiencia);
    }
}

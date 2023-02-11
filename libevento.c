#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libevento.h"

int calcular_tpl(int paciencia)
{
    return (max(1, (paciencia / 10) + (aleat(-2, 6))));
}

int calcular_tdl(mundo_t *mundo, int a, int b, int id)
{
    int d, v;
    int x_a = ((*(mundo->locais + a))->locallizacao)->x;
    int x_b = ((*(mundo->locais + b))->locallizacao)->x;
    int y_a = ((*(mundo->locais + a))->locallizacao)->y;
    int y_b = ((*(mundo->locais + b))->locallizacao)->y;
    int idade = (*(mundo->herois + id))->idade;
    d = sqrt(pow(x_b - x_a, 2) + pow((y_b - y_a), 2));
    v = 100 - max(0, idade - 40);
    return ((d / v) / 15);
}

/* cria vetor de conjuntos
 * retorna NULL em caso de falha*/
conjunto_t **instanciar_vetor_cjt(int tam, int tam2)
{
    conjunto_t **v;
    int i;
    /* aloca espaço num vetor de tam ponteiros de conjuntos*/
    if (!(v = malloc(sizeof(conjunto_t *) * tam)))
        return NULL;
    /* cria tam2 conjuntos*/
    for (i = 0; i < tam; i++)
        *(v + i) = cria_cjt(tam2);

    return v;
}

/* libera espaço alocado num vetor de tam ponteiros de conjuntos*/
conjunto_t **destruir_vetor_cjt(conjunto_t **v, int tam)
{
    int i;
    for (i = 0; i < tam; i++)
        *(v + i) = destroi_cjt(*(v + i));

    free(v);
    return NULL;
}

int tratar_evento_chegada(lef_t *cronologia, evento_t *evento, mundo_t *mundo)
{
    evento_t *sai;
    int tpl;

    int n_herois = (*(mundo->locais + (evento->dado2)))->herois_no_lugar;
    int n_max = (*(mundo->locais + (evento->dado2)))->lotacao_maxima;
    int paciencia = (*(mundo->herois + (evento->dado1)))->paciencia;
    int n_fila = tamanho_fila((*(mundo->locais + (evento->dado2)))->fila);

    if (n_herois == n_max)
    {

        if (paciencia / 4 - n_fila > 0)
        {
            insere_fila(((*(mundo->locais + (evento->dado2)))->fila), (evento->dado1));
            printf("FILA%2d\n", tamanho_fila((*(mundo->locais + (evento->dado2)))->fila));
            return 1;
        }
        else
        {
            printf("DESISTE\n");

            if (!(sai = malloc(sizeof(evento_t))))
            {
                printf("ERRO 4.1: falha ao alocar memória na variável sai.\n");
                return 0;
            }
            sai->tipo = SAIDA;
            sai->dado1 = evento->dado1;
            sai->dado2 = evento->dado2;
            sai->tempo = mundo->tempo_atual;
            if (!(adiciona_ordem_lef(cronologia, sai)))
            {
                printf("ERRO 4.2: falha ao adicionar o evento sai.\n");
                return 0;
            }
            free(sai);
            return 1;
        }
    }

    printf("ENTRA\n");
    ((*(mundo->locais + (evento->dado2)))->herois_no_lugar)++;

    if (!(insere_cjt(((*(mundo->locais + (evento->dado2)))->herois), evento->dado1)))
    {
        printf("ERRO 4.4: falha ao adicionar heroi.\n");
        return 0;
    }

    if (!(sai = malloc(sizeof(evento_t))))
    {
        printf("ERRO 4.1: falha ao alocar memória na variável sai.\n");
        return 0;
    }

    sai->tipo = SAIDA;
    sai->dado1 = evento->dado1;
    sai->dado2 = evento->dado2;
    tpl = calcular_tpl((*(mundo->herois + (evento->dado1)))->paciencia);
    sai->tempo = (mundo->tempo_atual) + tpl;
    if (!(adiciona_ordem_lef(cronologia, sai)))
    {
        printf("ERRO 2.3: falha ao adicionar o evento sai.\n");
        return 0;
    }
    free(sai);

    return 1;
}

/* trata o evento saída
 * retorna 1 em caso de sucesso e 0 em caso de falha*/
int tratar_evento_saida(lef_t *cronologia, evento_t *evento, mundo_t *mundo)
{
    evento_t *chegada;
    int *elemento, tdl;

    /* aloca espaço para um evento de chegada do herói que está saindo*/
    if (!(chegada = malloc(sizeof(evento_t))))
    {
        printf("ERRO 5.1: falha ao alocar memória na variável chegada.\n");
        return 0;
    }

    /* adiciona os parametros ao evento de chegada*/
    chegada->tipo = CHEGADA;
    chegada->dado1 = evento->dado1;
    chegada->dado2 = aleat(0, ((mundo->n_locais) - 1));
    tdl = calcular_tdl(mundo, evento->dado2, chegada->dado2, chegada->dado1);
    chegada->tempo = mundo->tempo_atual + tdl;

    /* adiciona em ordem o evento chegada na lista de eventos futuros cronologia*/
    if (!(adiciona_ordem_lef(cronologia, chegada)))
    {
        printf("ERRO 5.2: falha ao adicionar o evento chegada.\n");
        return 0;
    }

    /* libera espaço alocado no evento chegada*/
    free(chegada);

    /* confere se o herói que está saindo estava dentro do local ou apenas na fila*/
    if (pertence_cjt(((*(mundo->locais + (evento->dado2)))->herois), evento->dado1))
    {
        /* confere se existe uma fila de heróis no local*/
        if (tamanho_fila((*(mundo->locais + (evento->dado2)))->fila) > 0)
        {
            /* aloca espaço para um evento de chegada do herói que está na fila*/
            if (!(chegada = malloc(sizeof(evento_t))))
            {
                printf("ERRO 5.3: falha ao alocar memória na variável chegada.\n");
                return 0;
            }

            /* aloca espaço para um ponteiro de inteiro*/
            if (!(elemento = malloc(sizeof(int))))
            {
                printf("ERRO 5.4: falha ao alocar memória na variável elemento.\n");
                return 0;
            }

            /* retira o id herói da fila e retorna para o ponteiro de inteiro*/
            if (!(retira_fila((*(mundo->locais + (evento->dado2)))->fila, elemento)))
            {
                printf("ERRO 5.5: falha ao retirar elemento da fila.\n");
                return 0;
            }

            /* adiciona parametros para o evento de chegada para o herói que está na fila*/
            chegada->tipo = CHEGADA;
            chegada->dado1 = *elemento;
            chegada->dado2 = evento->dado2;
            chegada->tempo = mundo->tempo_atual;

            /* adiciona evento chegada no inicio da lista de eventos futuros cronologia*/
            if (!(adiciona_inicio_lef(cronologia, chegada)))
            {
                printf("ERRO 5.6: falha ao adicionar o evento chegada.\n");
                return 0;
            }

            /* imprime o resto da menssagem*/
            printf(", REMOVE FILA HEROI %2d", *elemento);

            /* desaloca memórias alocadas*/
            free(elemento);
            free(chegada);
        }

        /* altera o número de heróis no local e retira o herói que saiu do conjunto de herois no local*/
        ((*(mundo->locais + (evento->dado2)))->herois_no_lugar)--;
        if (!(retira_cjt(((*(mundo->locais + (evento->dado2)))->herois), evento->dado1)))
        {
            printf("ERRO 5.7: falha ao retirar elemento do conjunto.\n");
            return 0;
        }
    }
    printf("\n");
    return 1;
}


/* procura uma equipe que solucuine a missão
 * devolve -1 caso não exista               */
int encontrar_equipe_missao(int tam, locais_t **locais, heroi_t **herois, conjunto_t *missao, evento_t *evento)
{
    conjunto_t *uni_hab, **hab_hero;
    int i, j, card1, card2, posi, tam_max = N_HAB;

    /* cria um vetor com a união das habilidades dos herois da equipe*/
    hab_hero = instanciar_vetor_cjt(tam, tam_max);

    /*faz a união dos conjuntos de habilidades dos herois
     * e guarda os valores no vetor de habilidad cria e retorna uma struct local
    retorna null em caso de falha*/
    if (tam > 0)
    {
        for (i = 0; i <= (tam - 1); i++)
        {
            for (j = 0; j < (((*((locais + i)))->herois)->card); j++)
            {
                posi = (*(((*((locais + i)))->herois)->v + j));
                uni_hab = uniao_cjt(*(hab_hero + i), ((*(herois + posi))->hab));
                memcpy((*(hab_hero + i))->v, uni_hab->v, (tam_max * sizeof(int)));
                (*(hab_hero + i))->card = uni_hab->card;
                destroi_cjt(uni_hab);
            }
        }
    }

    /* verifica se existe pelo menos uma equipe que possa efetuar a missão*/
    posi = -1;
    i = 0;
    while ((i < tam) && (posi == -1))
    {
        if (contido_cjt(missao, *(hab_hero + i)))
            posi = i;
        i++;
    }

    /* Imprime os conjuntos de habilidades de herois*/
    for (i = 0; i <= (tam - 1); i++)
    { /* cria e retorna uma struct local
       * retorna null em caso de falha*/

        printf("%6d:MISSAO %3d HAB_EQL %d:", evento->tempo, evento->dado1, ((*(locais + i))->id));
        imprime_cjt(*(hab_hero + i));
    }
    /*caso exista pelo menos uma equipe que possa efetuar a missão
     * confere qual a menor equipe*/
    if (posi != -1)
    {
        for (i = posi; i <= (tam - 1); i++)
        {
            card1 = ((*((locais + i)))->herois)->card;
            card2 = ((*((locais + posi)))->herois)->card;

            if ((contido_cjt(missao, *(hab_hero + i))) && ((card1 < card2)))
                posi = i;
        }
    }

    /* libera o vetor de ponteiros de habilidades dos herois*/
    hab_hero = destruir_vetor_cjt(hab_hero, tam);

    return posi;
}

int tratar_evento_missao(lef_t *cronologia, evento_t *evento, mundo_t *mundo, missoes_t **missoes)
{
    int posi, i, j;
    evento_t *missao;

    /* imprime as habilidades necessárias para a missão*/
    printf("HAB_REQ ");
    imprime_cjt(((*(missoes + (evento->dado1)))->missao));

    /* verifica se existe um conjunto de heróis que possa realizar a missão
     * retorna a id de 0 até n_locais caso exista e -1 caso contrário*/
    posi = encontrar_equipe_missao((mundo->n_locais), (mundo->locais), (mundo->herois), ((*(missoes + (evento->dado1)))->missao), evento);

    if (posi != -1)
    {
        printf("%6d:MISSAO %3d HER_EQS %d:", evento->tempo, evento->dado1, ((*(mundo->locais + posi))->id));
        imprime_cjt(((*(mundo->locais + posi)))->herois);

        for (i = 0; i < cardinalidade_cjt((((*(mundo->locais + posi)))->herois)); i++)
        {
            j = *(((*(mundo->locais + posi))->herois)->v + i);
            (*(mundo->herois + j))->experiencia++;
        }
        return 1;
    }

    printf("%6d:MISSAO %3d IMPOSSIVEL\n", evento->tempo, evento->dado1);
    if (!(missao = malloc(sizeof(evento_t))))
    {
        printf("ERRO 6.3: falha ao alocar memória na variável missao.\n");
        return 0;
    }
    missao->tipo = MISSAO;
    missao->dado1 = evento->dado1;
    missao->tempo = aleat((mundo->tempo_atual), TEMPO_FIM);
    if (!(adiciona_ordem_lef(cronologia, missao)))
    {
        printf("ERRO 6.4: falha ao adicionar o evento missao.\n");
        return 0;
    }
    free(missao);
    return 1;
}

void imprime_evento_chegada(evento_t *evento, mundo_t *mundo)
{
    printf("%6d:CHEGA HEROI %2d LOCAL %d (%2d/%2d), ", evento->tempo, evento->dado1,
           evento->dado2, (cardinalidade_cjt((*(mundo->locais + (evento->dado2)))->herois)),
           ((*(mundo->locais + (evento->dado2)))->lotacao_maxima));
}

void imprime_evento_saida(evento_t *evento, mundo_t *mundo)
{
    printf("%6d:SAIDA HEROI %2d LOCAL %d (%2d/%2d)", evento->tempo, evento->dado1,
           evento->dado2, (cardinalidade_cjt((*(mundo->locais + (evento->dado2)))->herois)),
           ((*(mundo->locais + (evento->dado2)))->lotacao_maxima));
}

void imprime_evento_missao(evento_t *evento, mundo_t *mundo)
{
    printf("%6d:MISSAO %3d ", evento->tempo, evento->dado1);
    ;
}

void imprime_evento_fim(evento_t *evento, mundo_t *mundo)
{
    int i;

    printf("%6d:FIM\n", evento->tempo);

    for (i = 0; i < (mundo->n_herois); i++)
        printf("HEROI %2d EXPERIENCIA %2d\n", (*(mundo->herois + i))->id, (*(mundo->herois + i))->experiencia);
}

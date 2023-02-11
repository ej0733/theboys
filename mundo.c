#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libmundo.h"

int main()
{
    int i;
    mundo_t *mundo;
    lef_t *cronologia;
    evento_t *evento;
    missoes_t **missoes;

    srand(time(NULL));

    mundo = cria_mundo();
    if (mundo == NULL)
    {
        printf("ERRO 1: falha na função cria_mundo.\n");
        return 0;
    }

    cronologia = cria_cronologia(mundo->n_herois, mundo->n_locais);
    if (cronologia == NULL)
    {
        printf("ERRO 2: falha na função cria_cronologia.\n");
        return 0;
    }

    missoes = cria_missoes(cronologia, (mundo->habilidades));
    if (missoes == NULL)
    {
        printf("ERRO 3: falha na função cria_missoes.\n");
        return 0;
    }

    /* trata os eventos até que cornologia seja igual a null.*/
    do
    {
        /* retira o primeiro evento da lista de eventos futuros*/
        evento = obtem_primeiro_lef(cronologia);

        /* atualiza o tempo atual para o tempo do evento retirado*/
        mundo->tempo_atual = evento->tempo;
        switch (evento->tipo)
        {
        /* trata os eventos do tipo chegada*/
        case CHEGADA:
        {
            printf("%6d:CHEGA HEROI %2d LOCAL %d (%2d/%2d), ", evento->tempo, evento->dado1,
                   evento->dado2, (cardinalidade_cjt((*(mundo->locais + (evento->dado2)))->herois)),
                   ((*(mundo->locais + (evento->dado2)))->lotacao_maxima));
            if (!(chegada(cronologia, evento, mundo)))
            {
                printf("ERRO 4: falha na função chegada.\n");
                return 0;
            }
            free(evento);
        }
        break;

        /* trata os eventos do tipo saída*/
        case SAIDA:
        {
            printf("%6d:SAIDA HEROI %2d LOCAL %d (%2d/%2d)", evento->tempo, evento->dado1,
                   evento->dado2, (cardinalidade_cjt((*(mundo->locais + (evento->dado2)))->herois)),
                   ((*(mundo->locais + (evento->dado2)))->lotacao_maxima));
            if (!(saida(cronologia, evento, mundo)))
            {
                printf("ERRO 5: falha na função saida.\n");
                return 0;
            }
            free(evento);
        }
        break;

        /* trata os eventos do tipo missão*/
        case MISSAO:
        {
            printf("%6d:MISSAO %3d ", evento->tempo, evento->dado1);
            if (!(missao(cronologia, evento, mundo, missoes)))
            {
                printf("ERRO 6: falha na função missao.\n");
                return 0;
            }
            free(evento);
        }
        break;

        /* chama as funções de desalocar espaço
         * cronologia recebe null.*/
        case FIM:
        {
            printf("%6d:FIM\n", evento->tempo);
            for (i = 0; i < (mundo->n_herois); i++)
                printf("HEROI %2d EXPERIENCIA %2d\n", (*(mundo->herois + i))->id, (*(mundo->herois + i))->experiencia);
            free(evento);
            missoes = destroi_missoes(missoes);
            cronologia = destroi_lef(cronologia);
            mundo = destroi_mundo(mundo);
        }
        break;

        /* caso o valor do evento não se encaixe em nenhum dos casos
         * retorna menssagem de erro*/
        default:
        {
            printf("ERRO 10: valor inválido no tipo de evento\n");
            return 0;
        }
        }
    } while (cronologia != NULL);
    return 0;
}

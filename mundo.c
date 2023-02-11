#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libmundo.h"

int main()
{
    int i;

    lef_t *lef;
    mundo_t *mundo;
    evento_t *evento;
    missoes_t **missoes;

    srand(time(NULL));

    mundo = instanciar_mundo();

    if (mundo == NULL)
    {
        printf("ERRO 1: falha na função cria_mundo.\n");
        return 0;
    }

    lef = instanciar_lef(mundo->n_herois, mundo->n_locais);

    if (lef == NULL)
    {
        printf("ERRO 2: falha na função cria_cronologia.\n");
        return 0;
    }

    missoes = instanciar_missoes(lef, (mundo->habilidades));

    if (missoes == NULL)
    {
        printf("ERRO 3: falha na função cria_missoes.\n");
        return 0;
    }

    do
    {
        evento = obtem_primeiro_lef(lef);
        mundo->tempo_atual = evento->tempo;

        switch (evento->tipo)
        {
        case CHEGADA:
        {
            printf("%6d:CHEGA HEROI %2d LOCAL %d (%2d/%2d), ", evento->tempo, evento->dado1,
                   evento->dado2, (cardinalidade_cjt((*(mundo->locais + (evento->dado2)))->herois)),
                   ((*(mundo->locais + (evento->dado2)))->lotacao_maxima));
            if (!(tratar_evento_chegada(lef, evento, mundo)))
            {
                printf("ERRO 4: falha na função chegada.\n");
                return 0;
            }
            free(evento);
        }
        break;

        case SAIDA:
        {
            printf("%6d:SAIDA HEROI %2d LOCAL %d (%2d/%2d)", evento->tempo, evento->dado1,
                   evento->dado2, (cardinalidade_cjt((*(mundo->locais + (evento->dado2)))->herois)),
                   ((*(mundo->locais + (evento->dado2)))->lotacao_maxima));
            if (!(tratar_evento_saida(lef, evento, mundo)))
            {
                printf("ERRO 5: falha na função saida.\n");
                return 0;
            }
            free(evento);
        }
        break;

        case MISSAO:
        {
            printf("%6d:MISSAO %3d ", evento->tempo, evento->dado1);
            if (!(tratar_evento_missao(lef, evento, mundo, missoes)))
            {
                printf("ERRO 6: falha na função missao.\n");
                return 0;
            }
            free(evento);
        }
        break;

        case FIM:
        {
            printf("%6d:FIM\n", evento->tempo);
            for (i = 0; i < (mundo->n_herois); i++)
                printf("HEROI %2d EXPERIENCIA %2d\n", (*(mundo->herois + i))->id, (*(mundo->herois + i))->experiencia);
            free(evento);
            missoes = destruir_missoes(missoes);
            lef = destroi_lef(lef);
            mundo = destruir_mundo(mundo);
        }
        break;

        default:
        {
            printf("ERRO 10: valor inválido no tipo de evento\n");
            return 0;
        }
        }
    } while (lef != NULL);

    return 0;
}

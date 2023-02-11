#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libs/libevento.h"

int main()
{
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
            imprime_evento_chegada(evento, mundo);

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
            imprime_evento_saida(evento, mundo);

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
            imprime_evento_missao(evento, mundo);

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
            imprime_evento_fim(evento, mundo);

            free(evento);

            missoes = destruir_missoes(missoes);
            lef = destroi_lef(lef);
            mundo = destruir_mundo(mundo);
        }
        break;
        }
    } while (lef != NULL);

    return 0;
}

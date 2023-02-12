#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "libmundo.h"

int max(int num1, int num2)
{
    return num2 < num1 ? num1 : num2;
}

/* retorna um valor aleatório entre min e max*/
int aleat(int min, int max)
{
    return (min + (rand() % ((max - min + 1))));
}

/* cria um vetor de ponteiros de herois*/
heroi_t **instanciar_herois(mundo_t *mundo)
{
    int i;
    heroi_t **heroi;

    /* aloca espaço no vetor de ponteiros heroi*/
    if (!(heroi = (heroi_t **)malloc(sizeof(heroi_t) * mundo->n_herois)))
    {
        printf("ERRO 1.4.1: falha ao alocar memória no vetor 'heroi'.\n");
        return NULL;
    }

    /* aloca espaço e adiciona os valores no ponteiro de herois*/
    for (i = 0; i < mundo->n_herois; i++)
    {
        if (!(heroi[i] = (heroi_t *)malloc(sizeof(heroi_t))))
        {
            printf("ERRO 1.4.2: falha ao alocar memória no heroi %d.\n", i);
            return NULL;
        }

        heroi[i]->id = i;
        heroi[i]->experiencia = 0;
        heroi[i]->paciencia = aleat(0, 100);
        heroi[i]->idade = aleat(18, 100);
        heroi[i]->hab = cria_subcjt_cjt((mundo->habilidades), aleat(2, 5));
    }

    return heroi;
}

/* destrói um vetor de ponteiros de heróis*/
heroi_t **destruir_herois(heroi_t **herois, int tam)
{
    int i;

    for (i = 0; i < tam; i++)
    {
        herois[i]->hab = destroi_cjt(herois[i]->hab);
        free(herois[i]);
        herois[i] = NULL;
    }

    free(herois);
    herois = NULL;

    return NULL;
}

/* cria e retorna uma struct local
 * retorna null em caso de falha*/
locais_t *instanciar_local(int tam, int id)
{
    locais_t *local;
    /* aloca espaço para a struct local*/
    if (!(local = (locais_t *)malloc(sizeof(locais_t))))
    {
        printf("ERRO 1.5.2.1: falha ao alocar memória no local.\n");
        return NULL;
    }

    /* aloca espaço para a struct de coordenadas do local*/
    if (!(local->locallizacao = (par_ordenado_t *)malloc(sizeof(par_ordenado_t))))
    {
        printf("ERRO 1.5.2.2: falha ao alocar memória na coordenada.\n");
        return NULL;
    }

    /* adiciona os parametros na struct local*/
    local->id = id;
    local->lotacao_maxima = aleat(5, 30);
    local->locallizacao->x = aleat(0, (tam - 1));
    local->locallizacao->y = aleat(0, (tam - 1));
    local->herois_no_lugar = 0;
    local->fila = cria_fila();
    local->herois = cria_cjt(local->lotacao_maxima);

    return local;
}

/* cria e retorna um vetor de structs locais
 * retorna null em caso de falha                */
locais_t **instanciar_locais(mundo_t *mundo)
{
    int i;
    locais_t **locais;

    if (!(locais = (locais_t **)malloc(sizeof(locais_t) * mundo->n_locais)))
    {
        printf("ERRO 1.5.1: falha ao alocar memória no vetor 'locais'.\n");
        return NULL;
    }

    for (i = 0; i < mundo->n_locais; i++)
    {
        locais[i] = instanciar_local(mundo->n_tamanho_mundo, i);

        if (locais[i] == NULL)
        {
            printf("ERRO 1.5.2: falha ao alocar memória no vetor 'locais+%d'.\n", i);
            return NULL;
        }
    }

    return locais;
}

/* desaloca memória de um vetor de structs locais
 * retorna null                                     */
locais_t **destruir_locais(mundo_t *mundo)
{
    int i;

    for (i = 0; i < mundo->n_locais; i++)
    {
        mundo->locais[i]->fila = destroi_fila(mundo->locais[i]->fila);
        mundo->locais[i]->herois = destroi_cjt(mundo->locais[i]->herois);

        free(mundo->locais[i]->locallizacao);
        free(mundo->locais[i]);

        mundo->locais[i] = NULL;
    }

    free(mundo->locais);
    mundo->locais = NULL;

    return NULL;
}

mundo_t *instanciar_mundo()
{
    int i;
    mundo_t *mundo;

    if (!(mundo = malloc(sizeof(mundo_t))))
    {
        printf("ERRO 1.1: falha ao alocar memória na variável mundo.\n");
        return NULL;
    }

    mundo->tempo_atual = 0;
    mundo->n_tamanho_mundo = TAM_MUNDO;
    mundo->n_herois = N_HAB * 5;
    mundo->n_locais = (mundo->n_herois) / 6;
    mundo->habilidades = cria_cjt(N_HAB);

    for (i = 0; i < N_HAB; i++)
        if (!(insere_cjt(mundo->habilidades, i)))
        {
            printf("ERRO 1.2: falha ao inserir elemento.\n");
            return NULL;
        }

    if (mundo->habilidades == NULL)
    {
        printf("ERRO 1.3: falha na função cria_cjt.\n");
        return NULL;
    }

    mundo->herois = instanciar_herois(mundo);

    if (mundo->herois == NULL)
    {
        printf("ERRO 1.4: falha na função cria_herois.\n");
        return NULL;
    }

    mundo->locais = instanciar_locais(mundo);

    if (mundo->locais == NULL)
    {
        printf("ERRO 1.5: falha na função cria_locais.\n");
        return NULL;
    }

    return mundo;
}

mundo_t *destruir_mundo(mundo_t *mundo)
{
    mundo->herois = destruir_herois(mundo->herois, mundo->n_herois);
    mundo->locais = destruir_locais(mundo);
    mundo->habilidades = destroi_cjt(mundo->habilidades);

    free(mundo);
    mundo = NULL;

    return NULL;
}

lef_t *instanciar_lef(int n_herois, int n_locais)
{
    int i;
    lef_t *lef;
    evento_t *evento;

    lef = cria_lef();

    if (lef == NULL)
    {
        printf("ERRO 2.1: falha ao alocar memória na variável lef.\n");
        return 0;
    }

    if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
    {
        printf("ERRO 2.2: falha ao alocar memória na variável evento.\n");
        return 0;
    }

    evento->tipo = FIM;
    evento->tempo = TEMPO_FIM;

    if (!(adiciona_inicio_lef(lef, evento)))
    {
        printf("ERRO 2.3: falha ao adicionar o evento evento.\n");
        return 0;
    }

    free(evento);
    evento = NULL;

    for (i = 0; i < n_herois; i++)
    {
        if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        {
            printf("ERRO 2.4: falha ao alocar memória na variável chegada.\n");
            return 0;
        }

        evento->tipo = CHEGADA;
        evento->dado1 = i;
        evento->dado2 = aleat(0, (n_locais - 1));
        evento->tempo = aleat(0, 96 * 7);

        if (!(adiciona_ordem_lef(lef, evento)))
        {
            printf("ERRO 2.5: falha ao adicionar o evento chegada %d.\n", i);
            return NULL;
        }

        free(evento);
        evento = NULL;
    }

    return lef;
}

/* cria o vetor de missões
 * retorna null em caso de falha*/
missoes_t **instanciar_missoes(lef_t *lef, conjunto_t *hab)
{
    int i, n_missoes;
    evento_t *evento;
    missoes_t **missoes;

    n_missoes = TAM_MUNDO / 100;

    /* aloca espaço num vetor de n_missoes ponteiros de missões*/
    if (!(missoes = (missoes_t **)malloc((n_missoes) * sizeof(missoes_t *))))
    {
        printf("ERRO 3.1: falha ao alocar memória no vetor de ponteiros 'missões'.\n");
        return NULL;
    }

    /* cria n_missoes de missões*/
    for (i = 0; i < n_missoes - 1; i++)
    {
        /* aloca espaço para uma struct missões*/
        if (!(missoes[i] = (missoes_t *)malloc(sizeof(missoes_t))))
        {
            printf("ERRO 3.2: falha ao alocar memória no ponteiro 'missões+%d'.\n", i);
            return NULL;
        }

        missoes[i]->id = i;
        missoes[i]->missao = cria_subcjt_cjt(hab, (aleat(3, 6)));

        /* aloca espaço para um evento*/
        if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        {
            printf("ERRO 3.3: falha ao alocar memória na variável evento.\n");
            return NULL;
        }

        evento->tipo = MISSAO;
        evento->dado1 = i;
        evento->tempo = aleat(0, TEMPO_FIM);

        /* adiciona o evento de missão na lef*/
        if (!(adiciona_ordem_lef(lef, evento)))
        {
            printf("ERRO 3.4: falha ao adicionar o evento chegada.\n");
            return NULL;
        }

        /* desaloca o espaço do evento demissão*/
        free(evento);
        evento = NULL;
    }

    return missoes;
}

/* libera espaço no vetor de ponteiros de missões*/
missoes_t **destruir_missoes(missoes_t **missoes)
{
    int i;

    for (i = 0; i < TAM_MUNDO / 100 - 1; i++)
    {
        missoes[i]->missao = destroi_cjt(missoes[i]->missao);
        free(missoes[i]);
        missoes[i] = NULL;
    }

    free(missoes);
    missoes = NULL;

    return NULL;
}

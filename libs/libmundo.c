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

int aleat(int min, int max)
{
    return (min + (rand() % ((max - min + 1))));
}

heroi_t **instanciar_herois(mundo_t *mundo)
{
    int i;
    heroi_t **heroi;

    if (!(heroi = (heroi_t **)malloc(sizeof(heroi_t) * mundo->n_herois)))
    {
        printf("ERRO: Não foi possível alocar memória para vetor de heróis.\n");
        return NULL;
    }

    for (i = 0; i < mundo->n_herois; i++)
    {
        if (!(heroi[i] = (heroi_t *)malloc(sizeof(heroi_t))))
        {
            printf("ERRO: Não foi possível alocar memória para herói %d.\n", i);
            return NULL;
        }

        heroi[i]->id = i;
        heroi[i]->experiencia = 0;
        heroi[i]->paciencia = aleat(0, 100);
        heroi[i]->idade = aleat(18, 100);
        heroi[i]->habilidades = cria_subcjt_cjt((mundo->habilidades), aleat(2, 5));
    }

    return heroi;
}

heroi_t **destruir_herois(heroi_t **herois, int tam)
{
    int i;

    for (i = 0; i < tam; i++)
    {
        herois[i]->habilidades = destroi_cjt(herois[i]->habilidades);
        free(herois[i]);
        herois[i] = NULL;
    }

    free(herois);
    herois = NULL;

    return NULL;
}

local_t *instanciar_local(int tam, int id)
{
    local_t *local;
    
    if (!(local = (local_t *)malloc(sizeof(local_t))))
    {
        printf("ERRO: Não foi possível alocar memória para local.\n");
        return NULL;
    }

    if (!(local->locallizacao = (par_ordenado_t *)malloc(sizeof(par_ordenado_t))))
    {
        printf("ERRO: Não foi possível alocar memória para localização.\n");
        return NULL;
    }

    local->id = id;
    local->lotacao_maxima = aleat(5, 30);
    local->locallizacao->x = aleat(0, (tam - 1));
    local->locallizacao->y = aleat(0, (tam - 1));
    local->herois_no_lugar = 0;
    local->fila = cria_fila();
    local->herois = cria_cjt(local->lotacao_maxima);

    return local;
}

local_t **instanciar_locais(mundo_t *mundo)
{
    int i;
    local_t **locais;

    if (!(locais = (local_t **)malloc(sizeof(local_t) * mundo->n_locais)))
    {
        printf("ERRO: Não foi possível alocar memória para vetor de locais.\n");
        return NULL;
    }

    for (i = 0; i < mundo->n_locais; i++)
    {
        locais[i] = instanciar_local(mundo->n_tamanho_mundo, i);

        if (locais[i] == NULL)
        {
            printf("ERRO: Não foi possível alocar memória para local %d.\n", i);
            return NULL;
        }
    }

    return locais;
}

local_t **destruir_locais(mundo_t *mundo)
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
        printf("ERRO: Não foi possível alocar memória para o mundo.\n");
        return NULL;
    }

    mundo->tempo_atual = 0;
    mundo->n_tamanho_mundo = TAMANHO_MUNDO;
    mundo->n_herois = N_HABILIDADES * 5;
    mundo->n_locais = (mundo->n_herois) / 6;
    mundo->habilidades = cria_cjt(N_HABILIDADES);

    for (i = 0; i < N_HABILIDADES; i++)
        if (!(insere_cjt(mundo->habilidades, i)))
        {
            printf("ERRO: Não foi possível inserir habilidade %d no conjunto de habilidades.\n", i);
            return NULL;
        }

    if (mundo->habilidades == NULL)
    {
        printf("ERRO: Não foi possível alocar memória para o conjunto de habilidades.\n");
        return NULL;
    }

    mundo->herois = instanciar_herois(mundo);

    if (mundo->herois == NULL)
    {
        printf("ERRO: Não foi possível alocar memória para o vetor de heróis.\n");
        return NULL;
    }

    mundo->locais = instanciar_locais(mundo);

    if (mundo->locais == NULL)
    {
        printf("ERRO: Não foi possível alocar memória para o vetor de locais.\n");
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
        printf("ERRO: Não foi possível alocar memória para LEF.\n");
        return 0;
    }

    if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
    {
        printf("ERRO: Não foi possível alocar memória para evento.\n");
        return 0;
    }

    evento->tipo = TIPO_FIM;
    evento->tempo = FIM_DO_MUNDO;

    if (!(adiciona_inicio_lef(lef, evento)))
    {
        printf("ERRO: Houve um problema ao adicionar evento a LEF.\n");
        return 0;
    }

    free(evento);
    evento = NULL;

    for (i = 0; i < n_herois; i++)
    {
        if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        {
            printf("ERRO: Não foi possível alocar memória para evento.\n");
            return 0;
        }

        evento->tipo = TIPO_CHEGADA;
        evento->dado1 = i;
        evento->dado2 = aleat(0, (n_locais - 1));
        evento->tempo = aleat(0, 96 * 7);

        if (!(adiciona_ordem_lef(lef, evento)))
        {
            printf("ERRO: Houve um problema ao adicionar evento a LEF.\n");
            return NULL;
        }

        free(evento);
        evento = NULL;
    }

    return lef;
}

missao_t **instanciar_missoes(lef_t *lef, conjunto_t *hab)
{
    int i, n_missoes;
    evento_t *evento;
    missao_t **missoes;

    n_missoes = TAMANHO_MUNDO / 100;

    if (!(missoes = (missao_t **)malloc((n_missoes) * sizeof(missao_t *))))
    {
        printf("ERRO: Não foi possível alocar memória para o vetor de missões.\n");
        return NULL;
    }

    for (i = 0; i < n_missoes - 1; i++)
    {
        if (!(missoes[i] = (missao_t *)malloc(sizeof(missao_t))))
        {
            printf("ERRO: Não foi possível alocar memória para a missão %d.\n", i);
            return NULL;
        }

        missoes[i]->id = i;
        missoes[i]->missao = cria_subcjt_cjt(hab, (aleat(3, 6)));

        if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        {
            printf("ERRO: Não foi possível alocar memória para o evento de missão.\n");
            return NULL;
        }

        evento->tipo = TIPO_MISSAO;
        evento->dado1 = i;
        evento->tempo = aleat(0, FIM_DO_MUNDO);

        if (!(adiciona_ordem_lef(lef, evento)))
        {
            printf("ERRO: Houve um problema ao adicionar evento a LEF.\n");
            return NULL;
        }

        free(evento);
        evento = NULL;
    }

    return missoes;
}

missao_t **destruir_missoes(missao_t **missoes)
{
    int i;

    for (i = 0; i < TAMANHO_MUNDO / 100 - 1; i++)
    {
        missoes[i]->missao = destroi_cjt(missoes[i]->missao);
        free(missoes[i]);
        missoes[i] = NULL;
    }

    free(missoes);
    missoes = NULL;

    return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "libmundo.h"

/*
 * Função que retorna o maior número entre dois números.
 * Recebe como argumentos dois números inteiros.
 * Retorna o maior número entre os dois.
 */
int max(int num1, int num2)
{
    return num2 < num1 ? num1 : num2;
}

/*
 * Função que retorna um número aleatório entre o mínimo e o máximo.
 * Recebe como argumentos o mínimo e o máximo.
 * Retorna um número aleatório entre o mínimo e o máximo.
 */
int aleat(int min, int max)
{
    return (min + (rand() % ((max - min + 1))));
}

/*
 * Função para instanciar um vetor de heróis
 * Recebe como argumento o mundo.
 * Retorna um ponteiro para o herói criado ou NULL em caso de erro.
 */
heroi_t **instanciar_herois(mundo_t *mundo)
{
    int i;
    heroi_t **heroi;

    /* Aloca memória para o vetor de heróis */
    if (!(heroi = (heroi_t **)malloc(sizeof(heroi_t) * mundo->n_herois)))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para vetor de heróis.\n");
        return NULL;
    }

    /* Aloca memória para cada herói */
    for (i = 0; i < mundo->n_herois; i++)
    {
        /* Aloca memória para a estrutura herói */
        if (!(heroi[i] = (heroi_t *)malloc(sizeof(heroi_t))))
        {
            /* Imprime mensagem de erro caso não seja possível alocar memória */
            printf("ERRO: Não foi possível alocar memória para herói %d.\n", i);
            return NULL;
        }

        /* Atribui o id do herói */
        heroi[i]->id = i;

        /* Atribui a experiência do herói como 0 */
        heroi[i]->experiencia = 0;

        /* Atribui a paciência do herói como um valor aleatório entre 0 e 100 */
        heroi[i]->paciencia = aleat(0, 100);

        /* Atribui a idade do herói como um valor aleatório entre 18 e 100 */
        heroi[i]->idade = aleat(18, 100);

        /* Atribui as habilidades do herói como um subconjunto aleatório do conjunto de habilidades do mundo */
        heroi[i]->habilidades = cria_subcjt_cjt((mundo->habilidades), aleat(2, 5));
    }

    /* Retorna o ponteiro para o vetor de heróis criado */
    return heroi;
}

/*
 * Função para destruir um vetor de heróis.
 * Recebe como argumento o vetor de heróis e o tamanho do vetor.
 * Retorna NULL.
 */
heroi_t **destruir_herois(heroi_t **herois, int tam)
{
    int i;

    /* Libera a memória alocada para cada herói */
    for (i = 0; i < tam; i++)
    {
        /* Libera a memória alocada para o conjunto de habilidades do herói */
        herois[i]->habilidades = destroi_cjt(herois[i]->habilidades);

        /* Libera a memória alocada para a estrutura herói */
        free(herois[i]);

        /* Atribui NULL para o ponteiro do herói */
        herois[i] = NULL;
    }

    /* Libera a memória alocada para o vetor de heróis */
    free(herois);

    /* Atribui NULL para o ponteiro do vetor de heróis */
    herois = NULL;

    /* Retorna NULL */
    return NULL;
}

/*
 * Função para instanciar um local.
 * Recebe como argumentos o tamanho e o id do local.
 * Retorna um ponteiro para o local criado ou NULL em caso de erro.
 */
local_t *instanciar_local(int tam, int id)
{
    local_t *local;

    /* Aloca memória para a estrutura local */
    if (!(local = (local_t *)malloc(sizeof(local_t))))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para local.\n");
        return NULL;
    }

    /* Aloca memória para a estrutura de localização */
    if (!(local->locallizacao = (par_ordenado_t *)malloc(sizeof(par_ordenado_t))))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para localização.\n");
        return NULL;
    }

    /* Atribui o id ao local */
    local->id = id;

    /* Atribui a lotação máxima do local como um valor aleatório entre 5 e 30 */
    local->lotacao_maxima = aleat(5, 30);

    /* Atribui as coordenadas x e y da localização como valores aleatórios entre 0 e (tam - 1) */
    local->locallizacao->x = aleat(0, (tam - 1));
    local->locallizacao->y = aleat(0, (tam - 1));

    /* Inicializa o número de heróis no local com 0 */
    local->herois_no_lugar = 0;

    /* Cria uma fila vazia */
    local->fila = cria_fila();

    /* Cria um conjunto vazio com capacidade máxima igual a lotação máxima do local */
    local->herois = cria_cjt(local->lotacao_maxima);

    /* Retorna o ponteiro para o local criado */
    return local;
}

/*
 * Função para instanciar um vetor de locais.
 * Recebe como argumento o mundo.
 * Retorna um ponteiro para o vetor de locais criado ou NULL em caso de erro.
 */
local_t **instanciar_locais(mundo_t *mundo)
{
    int i;
    local_t **locais;

    /* Aloca memória para o vetor de locais */
    if (!(locais = (local_t **)malloc(sizeof(local_t) * mundo->n_locais)))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para vetor de locais.\n");
        return NULL;
    }

    /* Instancia cada local do vetor de locais */
    for (i = 0; i < mundo->n_locais; i++)
    {
        /* Aloca memória para a estrutura local */
        locais[i] = instanciar_local(mundo->n_tamanho_mundo, i);

        /* Verifica se foi possível alocar memória para o local */
        if (locais[i] == NULL)
        {
            /* Imprime mensagem de erro caso não seja possível alocar memória */
            printf("ERRO: Não foi possível alocar memória para local %d.\n", i);
            return NULL;
        }
    }

    /* Retorna o ponteiro para o vetor de locais criado */
    return locais;
}

/*
 * Função para destruir um vetor de locais.
 * Recebe como argumento o mundo.
 * Retorna NULL.
 */
local_t **destruir_locais(mundo_t *mundo)
{
    int i;

    /* Libera a memória alocada para cada local */
    for (i = 0; i < mundo->n_locais; i++)
    {
        /* Libera a memória alocada para a fila do local */
        mundo->locais[i]->fila = destroi_fila(mundo->locais[i]->fila);

        /* Libera a memória alocada para o conjunto de heróis do local */
        mundo->locais[i]->herois = destroi_cjt(mundo->locais[i]->herois);

        /* Libera a memória alocada para a estrutura de localização */
        free(mundo->locais[i]->locallizacao);

        /* Libera a memória alocada para a estrutura local */
        free(mundo->locais[i]);

        /* Atribui NULL para o ponteiro do local */
        mundo->locais[i] = NULL;
    }

    /* Libera a memória alocada para o vetor de locais */
    free(mundo->locais);

    /* Atribui NULL para o ponteiro do vetor de locais */
    mundo->locais = NULL;

    /* Retorna NULL */
    return NULL;
}

/*
 * Função para instanciar um mundo.
 * Retorna um ponteiro para o mundo criado ou NULL em caso de erro.
 */
mundo_t *instanciar_mundo()
{
    int i;
    mundo_t *mundo;

    /* Aloca memória para a estrutura mundo */
    if (!(mundo = malloc(sizeof(mundo_t))))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o mundo.\n");
        return NULL;
    }

    /* Inicializa o tempo atual do mundo com 0 */
    mundo->tempo_atual = 0;

    /* Inicializa o tamanho do mundo com o valor da constante TAMANHO_MUNDO */
    mundo->n_tamanho_mundo = TAMANHO_MUNDO;

    /* Inicializa o número de heróis com o valor da constante N_HEROIS multiplicado por 5 */
    mundo->n_herois = N_HABILIDADES * 5;

    /* Inicializa o número de locais com o valor de N_LOCAIS dividido por 6 */
    mundo->n_locais = mundo->n_herois / 6;

    /* Inicializa o conjunto de habilidades com um conjunto vazio de capacidade máxima igual a N_HABILIDADES */
    mundo->habilidades = cria_cjt(N_HABILIDADES);

    /* Verifica se foi possível alocar memória para o conjunto de habilidades */
    if (mundo->habilidades == NULL)
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o conjunto de habilidades.\n");
        return NULL;
    }

    for (i = 0; i < N_HABILIDADES; i++)
    {
        /* Insere habilidade no conjunto de habilidades */
        if (!(insere_cjt(mundo->habilidades, i)))
        {
            /* Imprime mensagem de erro caso não seja possível inserir a habilidade no conjunto */
            printf("ERRO: Não foi possível inserir habilidade %d no conjunto de habilidades.\n", i);
            return NULL;
        }
    }

    /* Instancia o vetor de heróis */
    mundo->herois = instanciar_herois(mundo);

    /* Verifica se foi possível alocar memória para o vetor de heróis */
    if (mundo->herois == NULL)
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o vetor de heróis.\n");
        return NULL;
    }

    /* Instancia o vetor de locais */
    mundo->locais = instanciar_locais(mundo);

    /* Verifica se foi possível alocar memória para o vetor de locais */
    if (mundo->locais == NULL)
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o vetor de locais.\n");
        return NULL;
    }

    /* Retorna o ponteiro para a estrutura mundo criada */
    return mundo;
}

/*
 * Função para destruir um mundo.
 * Recebe como argumento o mundo.
 * Retorna NULL.
 */
mundo_t *destruir_mundo(mundo_t *mundo)
{
    /* Libera a memória alocada para o vetor de heróis */
    mundo->herois = destruir_herois(mundo->herois, mundo->n_herois);

    /* Libera a memória alocada para o vetor de locais */
    mundo->locais = destruir_locais(mundo);

    /* Libera a memória alocada para o conjunto de habilidades */
    mundo->habilidades = destroi_cjt(mundo->habilidades);

    /* Libera a memória alocada para a estrutura mundo */
    free(mundo);

    /* Atribui NULL para o ponteiro do mundo */
    mundo = NULL;

    /* Retorna NULL */
    return NULL;
}

/*
 * Função para instanciar um LEF.
 * Recebe como argumento o número de heróis e o número de locais.
 * Retorna um ponteiro para o LEF criado ou NULL em caso de erro.
 */
lef_t *instanciar_lef(int n_herois, int n_locais)
{
    int i;
    lef_t *lef;
    evento_t *evento;

    /* Aloca memória para a estrutura LEF */
    lef = cria_lef();

    /* Verifica se foi possível alocar memória para a estrutura LEF */
    if (lef == NULL)
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para LEF.\n");
        return 0;
    }

    /* Aloca memória para o evento de fim do mundo */
    if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para evento.\n");
        return 0;
    }

    /* Inicializa o evento de fim do mundo */
    evento->tipo = TIPO_FIM;
    evento->tempo = FIM_DO_MUNDO;

    /* Adiciona o evento de fim do mundo na LEF */
    if (!(adiciona_inicio_lef(lef, evento)))
    {
        /* Imprime mensagem de erro caso não seja possível adicionar o evento na LEF */
        printf("ERRO: Houve um problema ao adicionar evento a LEF.\n");
        return 0;
    }

    /* Libera a memória alocada para o evento de fim do mundo */
    free(evento);

    /* Atribui NULL para o ponteiro do evento de fim do mundo */
    evento = NULL;

    /* Aloca memória para os eventos de chegada dos heróis */
    for (i = 0; i < n_herois; i++)
    {
        /* Aloca memória para o evento de chegada */
        if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        {
            /* Imprime mensagem de erro caso não seja possível alocar memória */
            printf("ERRO: Não foi possível alocar memória para evento.\n");
            return 0;
        }

        /* Inicializa o evento de chegada */
        evento->tipo = TIPO_CHEGADA;

        /* Atribui o id do herói ao evento */
        evento->dado1 = i;

        /* Atribui o id do local de chegada ao evento */
        evento->dado2 = aleat(0, (n_locais - 1));

        /* Atribui o tempo de chegada ao evento */
        evento->tempo = aleat(0, 96 * 7);

        /* Adiciona o evento de chegada na LEF */
        if (!(adiciona_ordem_lef(lef, evento)))
        {
            /* Imprime mensagem de erro caso não seja possível adicionar o evento na LEF */
            printf("ERRO: Houve um problema ao adicionar evento a LEF.\n");
            return NULL;
        }

        /* Libera a memória alocada para o evento de chegada */
        free(evento);

        /* Atribui NULL para o ponteiro do evento de chegada */
        evento = NULL;
    }

    /* Retorna o ponteiro para a estrutura LEF criada */
    return lef;
}

/*
 * Função para instanciar missões.
 * Recebe como argumento a LEF e o conjunto de habilidades.
 * Retorna um ponteiro para o vetor de missões criado ou NULL em caso de erro.
 */
missao_t **instanciar_missoes(lef_t *lef, conjunto_t *hab)
{
    int i, n_missoes;
    evento_t *evento;
    missao_t **missoes;

    /* Atribui o número de missões com base no tamanho do mundo */
    n_missoes = TAMANHO_MUNDO / 100;

    /* Aloca memória para o vetor de missões */
    if (!(missoes = (missao_t **)malloc((n_missoes) * sizeof(missao_t *))))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o vetor de missões.\n");
        return NULL;
    }

    /* Aloca memória para as missões */
    for (i = 0; i < n_missoes - 1; i++)
    {
        /* Aloca memória para a missão */
        if (!(missoes[i] = (missao_t *)malloc(sizeof(missao_t))))
        {
            /* Imprime mensagem de erro caso não seja possível alocar memória */
            printf("ERRO: Não foi possível alocar memória para a missão %d.\n", i);
            return NULL;
        }

        /* Inicializa a missão */
        missoes[i]->id = i;

        /* Cria um subconjunto de habilidades para a missão */
        missoes[i]->missao = cria_subcjt_cjt(hab, (aleat(3, 6)));

        /* Aloca memória para o evento de missão */
        if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        {
            /* Imprime mensagem de erro caso não seja possível alocar memória */
            printf("ERRO: Não foi possível alocar memória para o evento de missão.\n");
            return NULL;
        }

        /* Inicializa o evento de missão */
        evento->tipo = TIPO_MISSAO;

        /* Atribui o id da missão ao evento */
        evento->dado1 = i;

        /* Atribui o tempo de início da missão ao evento */
        evento->tempo = aleat(0, FIM_DO_MUNDO);

        /* Adiciona o evento de missão na LEF */
        if (!(adiciona_ordem_lef(lef, evento)))
        {
            /* Imprime mensagem de erro caso não seja possível adicionar o evento na LEF */
            printf("ERRO: Houve um problema ao adicionar evento a LEF.\n");
            return NULL;
        }

        /* Libera a memória alocada para o evento de missão */
        free(evento);

        /* Atribui NULL para o ponteiro do evento de missão */
        evento = NULL;
    }

    /* Aloca memória para a missão final */
    return missoes;
}

/*
 * Função para destruir missões.
 * Recebe como argumento o vetor de missões.
 * Retorna NULL.
 */
missao_t **destruir_missoes(missao_t **missoes)
{
    int i;

    /* Libera a memória alocada para as missões */
    for (i = 0; i < TAMANHO_MUNDO / 100 - 1; i++)
    {
        /* Libera a memória alocada para o subconjunto de habilidades da missão */
        missoes[i]->missao = destroi_cjt(missoes[i]->missao);

        /* Libera a memória alocada para a missão */
        free(missoes[i]);

        /* Atribui NULL para o ponteiro da missão */
        missoes[i] = NULL;
    }

    /* Libera a memória alocada para o vetor de missões */
    free(missoes);

    /* Atribui NULL para o ponteiro do vetor de missões */
    missoes = NULL;

    /* Retorna NULL */
    return NULL;
}

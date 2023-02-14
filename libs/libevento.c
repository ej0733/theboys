#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libevento.h"

/*
 * Função que calcula o TPL (Tempo de Permanência no Local) de um herói.
 * Recebe como parâmetro a paciência do herói.
 * Retorna o TPL calculado.
 */
int calcular_tpl(int paciencia)
{
    return (max(1, (paciencia / 10) + (aleat(-2, 6))));
}

/*
 * Função que calcula o TDL (Tempo de Deslocamento entre Locais) de um herói.
 * Recebe como parâmetro o mundo, o id do local de origem, o id do local de destino e o id do herói.
 * Retorna o TDL calculado.
 */
int calcular_tdl(mundo_t *mundo, int id_local, int id_local_dest, int id_heroi)
{
    int distancia, velocidade, x1, x2, y1, y2;

    /* Obtém as coordenadas do local de origem */
    x1 = mundo->locais[id_local]->locallizacao->x;
    y1 = mundo->locais[id_local]->locallizacao->y;

    /* Obtém as coordenadas do local de destino */
    x2 = mundo->locais[id_local_dest]->locallizacao->x;
    y2 = mundo->locais[id_local_dest]->locallizacao->y;

    /* Calcula a distância euclidiana entre os dois locais */
    distancia = sqrt(pow(x2 - x1, 2) + pow((y2 - y1), 2));

    /* Calcula a velocidade do herói */
    velocidade = 100 - max(0, mundo->herois[id_heroi]->idade - 40);

    /* Retorna o TDL calculado */
    return ((distancia / velocidade) / 15);
}

/*
 * Função que instancia um evento de chegada.
 * Recebe como parâmetro o id do herói, o id do local e o tempo atual.
 * Retorna o evento instanciado.
 */
evento_t *instanciar_evento_chegada(int id_heroi, int id_local, int tempo_atual)
{
    evento_t *evento;

    /* Aloca memória para o evento, retornando NULL caso não seja possível alocar memória */
    if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        return NULL;

    /* Define o tipo do evento como TIPO_CHEGADA */
    evento->tipo = TIPO_CHEGADA;

    /* Define dado1 como o id do herói */
    evento->dado1 = id_heroi;

    /* Define dado2 como o id do local */
    evento->dado2 = id_local;

    /* Define tempo como o tempo atual */
    evento->tempo = tempo_atual;

    /* Retorna o evento instanciado */
    return evento;
}

/*
 * Função que instancia um evento de saída.
 * Recebe como parâmetro o id do herói, o id do local e o tempo atual.
 * Retorna o evento instanciado.
 */
evento_t *instanciar_evento_saida(int id_heroi, int id_local, int tempo_atual)
{
    evento_t *evento;

    /* Aloca memória para o evento, retornando NULL caso não seja possível alocar memória */
    if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        return NULL;

    /* Define o tipo do evento como TIPO_SAIDA */
    evento->tipo = TIPO_SAIDA;

    /* Define dado1 como o id do herói */
    evento->dado1 = id_heroi;

    /* Define dado2 como o id do local */
    evento->dado2 = id_local;

    /* Define tempo como o tempo atual */
    evento->tempo = tempo_atual;

    /* Retorna o evento instanciado */
    return evento;
}

evento_t *instanciar_evento_missao(int id_missao, int tempo_atual)
{
    evento_t *evento;

    /* Aloca memória para o evento, retornando NULL caso não seja possível alocar memória */
    if (!(evento = (evento_t *)malloc(sizeof(evento_t))))
        return NULL;

    /* Define o tipo do evento como TIPO_MISSAO */
    evento->tipo = TIPO_MISSAO;

    /* Define dado1 como o id da missão */
    evento->dado1 = id_missao;

    /* Define dado2 como 0 */
    evento->dado2 = 0;

    /* Define tempo como o tempo atual */
    evento->tempo = tempo_atual;

    /* Retorna o evento instanciado */
    return evento;
}

/*
 * Função que instancia um vetor de conjuntos.
 * Recebe como parâmetro o tamanho do vetor e o tamanho dos conjuntos.
 * Retorna o vetor de conjuntos instanciado.
 */
conjunto_t **instanciar_vetor_cjt(int tamanho, int tamanho2)
{
    int i;
    conjunto_t **vetor_cjt;

    /* Aloca memória para o vetor de conjuntos, retornando NULL caso não seja possível alocar memória */
    if (!(vetor_cjt = (conjunto_t **)malloc(sizeof(conjunto_t) * tamanho)))
        return NULL;

    /* Instancia os conjuntos do vetor */
    for (i = 0; i < tamanho; i++)
        vetor_cjt[i] = cria_cjt(tamanho2);

    /* Retorna o vetor de conjuntos instanciado */
    return vetor_cjt;
}

/*
 * Função que destrói um vetor de conjuntos.
 * Recebe como parâmetro o vetor de conjuntos e o tamanho do vetor.
 * Retorna NULL.
 */
conjunto_t **destruir_vetor_cjt(conjunto_t **vetor_cjt, int tamanho)
{
    int i;

    /* Destroi os conjuntos do vetor */
    for (i = 0; i < tamanho; i++)
        vetor_cjt[i] = destroi_cjt(vetor_cjt[i]);

    /* Libera a memória alocada para o vetor de conjuntos */
    free(vetor_cjt);

    /* Define o vetor de conjuntos como NULL */
    vetor_cjt = NULL;

    /* Retorna NULL */
    return NULL;
}

/*
 * Função que trata o evento de chegada de um herói a um local.
 * Recebe como parâmetro a LEF, o evento de chegada e o mundo.
 * Retorna SUCCESS se o evento for tratado com sucesso e FAILURE caso contrário.
 */
int tratar_evento_chegada(lef_t *lef, evento_t *evento, mundo_t *mundo)
{
    evento_t *evento_saida;
    int tpl, num_herois_no_lugar, num_lotacao_max, paciencia_heroi, num_tamanho_fila;

    /* Obtém dados do evento de chegada */
    num_herois_no_lugar = mundo->locais[evento->dado2]->herois_no_lugar;
    num_lotacao_max = mundo->locais[evento->dado2]->lotacao_maxima;
    paciencia_heroi = mundo->herois[evento->dado1]->paciencia;
    num_tamanho_fila = tamanho_fila(mundo->locais[evento->dado2]->fila);

    /* Verifica se o local está cheio */
    if (num_herois_no_lugar == num_lotacao_max)
    {
        /* Verifica se o herói possui paciência de aguardar no local */
        if (paciencia_heroi / 4 - num_tamanho_fila > 0)
        {
            /* Insere o herói na fila do local */
            insere_fila(mundo->locais[evento->dado2]->fila, evento->dado1);

            /* Imprime o evento */
            printf("FILA %2d\n", tamanho_fila(mundo->locais[evento->dado2]->fila));

            return SUCCESS;
        }
        else
        {
            printf("DESISTE\n");

            /* Aloca memória para o evento de saída */
            if (!(evento_saida = instanciar_evento_saida(evento->dado1, evento->dado2, mundo->tempo_atual)))
            {
                /* Imprime mensagem de erro caso não seja possível alocar memória */
                printf("ERRO: Não foi possível alocar memória para o evento de saída.\n");

                return FAILURE;
            }

            /* Adiciona o evento de saída na LEF */
            if (!(adiciona_ordem_lef(lef, evento_saida)))
            {
                /* Imprime mensagem de erro caso não seja possível adicionar o evento de saída */
                printf("ERRO: Houve um erro ao adicionar o evento de saída.\n");

                return FAILURE;
            }

            /* Libera a memória alocada para o evento de saída */
            free(evento_saida);

            /* Define o ponteiro para o evento de saída como NULL */
            evento_saida = NULL;

            return SUCCESS;
        }
    }

    printf("ENTRA\n");

    /* Incrementa o número de heróis no local */
    mundo->locais[evento->dado2]->herois_no_lugar++;

    /* Insere o herói no conjunto de heróis do local */
    if (!(insere_cjt(mundo->locais[evento->dado2]->herois, evento->dado1)))
    {
        /* Imprime mensagem de erro caso não seja possível inserir o herói no conjunto de heróis do local */
        printf("ERRO: Houve um erro ao adicionar o herói ao conjunto de heróis do local.\n");

        return FAILURE;
    }

    /* Calcula o Tempo de Permanência no Local (TPL) */
    tpl = calcular_tpl(mundo->herois[evento->dado1]->paciencia);

    /* Aloca memória para o evento de saída */
    if (!(evento_saida = instanciar_evento_saida(evento->dado1, evento->dado2, mundo->tempo_atual + tpl)))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o evento de saída.\n");

        return FAILURE;
    }

    /* Adiciona o evento de saída na LEF */
    if (!(adiciona_ordem_lef(lef, evento_saida)))
    {
        /* Imprime mensagem de erro caso não seja possível adicionar o evento de saída */
        printf("ERRO: Houve um erro ao adicionar o evento de saída.\n");

        return FAILURE;
    }

    /* Libera a memória alocada para o evento de saída */
    free(evento_saida);

    /* Define o ponteiro para o evento de saída como NULL */
    evento_saida = NULL;

    return SUCCESS;
}

/*
 * Função que trata o evento de saída.
 * Recebe como parâmetros a LEF, o evento de saída, e o mundo.
 * Retorna SUCCESS caso o evento seja tratado com sucesso, ou FAILURE caso contrário.
 */
int tratar_evento_saida(lef_t *lef, evento_t *evento, mundo_t *mundo)
{
    int *elemento, id_local_dest, tdl;
    evento_t *evento_chegada;

    /* Atribui o id do local de destino do herói com valor aleatório entre 0 e o número de locais - 1 */
    id_local_dest = aleat(0, mundo->n_locais - 1);

    /* Calcula o Tempo de Deslocamento entre Locais (TDL) */
    tdl = calcular_tdl(mundo, evento->dado2, id_local_dest, evento->dado1);

    /* Aloca memória para o evento de chegada */
    if (!(evento_chegada = instanciar_evento_chegada(evento->dado1, id_local_dest, mundo->tempo_atual + tdl)))
    {
        /* Imprime mensagem de erro caso não seja possível alocar memória */
        printf("ERRO: Não foi possível alocar memória para o evento de chegada.\n");
        return FAILURE;
    }

    /* Adiciona o evento de chegada na LEF */
    if (!(adiciona_ordem_lef(lef, evento_chegada)))
    {
        /* Imprime mensagem de erro caso não seja possível adicionar o evento de chegada */
        printf("ERRO: Houve um erro ao adicionar o evento de chegada.\n");

        return FAILURE;
    }

    /* Libera a memória alocada para o evento de chegada */
    free(evento_chegada);

    /* Define o ponteiro para o evento de chegada como NULL */
    evento_chegada = NULL;

    /* Verifica se o id do herói pertence ao conjunto de heróis do local */
    if (pertence_cjt((mundo->locais[evento->dado2]->herois), evento->dado1))
    {
        /* Verifica se o tamanho da fila do id do local é maior que 0 */
        if (tamanho_fila(mundo->locais[evento->dado2]->fila) > 0)
        {
            /* Aloca memória para o elemento da fila */
            if (!(elemento = (int *)malloc(sizeof(int))))
            {
                /* Imprime mensagem de erro caso não seja possível alocar memória */
                printf("ERRO: Não foi possível alocar memória para o elemento da fila.\n");
                return FAILURE;
            }

            /* Retira o elemento da fila */
            if (!(retira_fila(mundo->locais[evento->dado2]->fila, elemento)))
            {
                /* Imprime mensagem de erro caso não seja possível retirar o elemento da fila */
                printf("ERROR: Houve um problema ao retirar o elemento da fila.\n");
                return FAILURE;
            }
            /* Aloca memória para o evento de chegada */
            if (!(evento_chegada = instanciar_evento_chegada(*elemento, evento->dado2, mundo->tempo_atual)))
            {
                /* Imprime mensagem de erro caso não seja possível alocar memória */
                printf("ERRO: Não foi possível alocar memória para o evento de chegada.\n");
                return FAILURE;
            }

            /* Adiciona o evento de chegada na LEF */
            if (!(adiciona_inicio_lef(lef, evento_chegada)))
            {
                /* Imprime mensagem de erro caso não seja possível adicionar o evento de chegada */
                printf("ERRO: Houve um problema ao adicionar o evento de chegada.\n");

                return FAILURE;
            }

            /* Imprime o id do herói retirado da fila */
            printf(", REMOVE FILA HEROI %2d", *elemento);

            /* Libera a memória alocada para o elemento da fila */
            free(elemento);

            /* Libera a memória alocada para o evento de chegada */
            free(evento_chegada);

            /* Define o ponteiro para o elemento da fila como NULL */
            elemento = NULL;

            /* Define o ponteiro para o evento de chegada como NULL */
            evento_chegada = NULL;
        }

        /* Decrementa o número de heróis no local */
        mundo->locais[evento->dado2]->herois_no_lugar--;

        /* Retira o id do herói do conjunto de heróis do local */
        if (!(retira_cjt(mundo->locais[evento->dado2]->herois, evento->dado1)))
        {
            /* Imprime mensagem de erro caso não seja possível retirar o id do herói do conjunto de heróis do local */
            printf("ERRO: Houve um erro ao retirar o herói do conjunto de heróis do local.\n");

            return FAILURE;
        }
    }

    printf("\n");

    return SUCCESS;
}

/*
 * Função que encontra uma equipe apta a realizar uma missão
 * Recebe como parâmetro o tamanho da equipe, um vetor de locais, um vetor de herois, um conjunto de missão e um evento
 * Retorna a posição do vetor de herois onde começa a equipe ou NOT_FOUND em caso de falha
 */
int encontrar_equipe_missao(int tamanho, local_t **locais, heroi_t **herois, conjunto_t *missao, evento_t *evento)
{
    conjunto_t *uniao_habilidade_cjt, **habilidades_herois_cjt;
    int i, j, card1, card2, posicao, tamanho_max = N_HABILIDADES;

    /* Instancia um vetor da união das habilidades dos herois da equipe */
    habilidades_herois_cjt = instanciar_vetor_cjt(tamanho, tamanho_max);

    if (tamanho > 0)
    {
        for (i = 0; i < tamanho; i++)
        {
            for (j = 0; j < cardinalidade_cjt(locais[i]->herois); j++)
            {
                /* Encontra a posição do heroi no vetor de herois */
                posicao = locais[i]->herois->v[j];

                /* Realiza a união das habilidades dos herois da equipe */
                uniao_habilidade_cjt = uniao_cjt(habilidades_herois_cjt[i], herois[posicao]->habilidades);

                /* Realiza a copia do vetor da união das habilidades dos herois da equipe */
                memcpy(habilidades_herois_cjt[i]->v, uniao_habilidade_cjt->v, tamanho_max * sizeof(int));

                /* Realiza a copia da cardinalidade do vetor da união das habilidades dos herois da equipe */
                habilidades_herois_cjt[i]->card = uniao_habilidade_cjt->card;

                /* Libera a memória alocada para a união das habilidades dos herois da equipe */
                destroi_cjt(uniao_habilidade_cjt);
            }
        }
    }

    /* Encontra uma equipe que possa efetuar a missão */
    i = 0;
    posicao = NOT_FOUND;

    while ((i < tamanho) && (posicao == NOT_FOUND))
    {
        /* Verifica se a missão está contida no conjunto de habilidades dos herois da equipe */
        if (contido_cjt(missao, habilidades_herois_cjt[i]))
            posicao = i;

        i++;
    }

    /* Imprime os conjuntos de habilidades de herois*/
    for (i = 0; i < tamanho; i++)
    {
        printf("%6d:MISSAO %2d HAB_EQL %d:", evento->tempo, evento->dado1, locais[i]->id);
        imprime_cjt(habilidades_herois_cjt[i]);
    }

    /* Encontra a menor equipe que possa efetuar a missão */
    if (posicao != NOT_FOUND)
    {
        for (i = posicao; i < tamanho; i++)
        {
            /* Obtém a cardinalidade dos conjuntos de habilidades dos herois */
            card1 = cardinalidade_cjt(locais[i]->herois);
            card2 = cardinalidade_cjt(locais[posicao]->herois);

            /* Verifica se a missão está contida no conjunto de habilidades dos herois */
            if ((contido_cjt(missao, habilidades_herois_cjt[i])) && ((card1 < card2)))
                posicao = i;
        }
    }

    /* Libera a memória alocada para o vetor da união das habilidades dos herois da equipe */
    habilidades_herois_cjt = destruir_vetor_cjt(habilidades_herois_cjt, tamanho);

    return posicao;
}

/*
 * Função que trata um evento de missão
 * Recebe como parâmetros um ponteiro para LEF, um ponteiro para evento, um ponteiro para o mundo e um vetor de missões
 * Retorna SUCCESS em caso de sucesso e FAILURE em caso de falha
 */
int tratar_evento_missao(lef_t *lef, evento_t *evento, mundo_t *mundo, missao_t **missoes)
{
    int posicao_equipe, i, j;
    evento_t *evento_missao;

    /* Imprime as habilidades necessárias para a missão */
    printf("HAB_REQ ");
    imprime_cjt(missoes[evento->dado1]->missao);

    /* Verifica se existe uma equipe apta a realizar a missão */
    posicao_equipe = encontrar_equipe_missao(mundo->n_locais, mundo->locais, mundo->herois, missoes[evento->dado1]->missao, evento);

    /* Verifica se a equipe foi encontrada */
    if (posicao_equipe != NOT_FOUND)
    {
        /* Imprime o identificador da equipe */
        printf("%6d:MISSAO %2d HER_EQS %d:", evento->tempo, evento->dado1, mundo->locais[posicao_equipe]->id);

        /* Imprime os herois da equipe */
        imprime_cjt(mundo->locais[posicao_equipe]->herois);

        /* Incrementa a experiência dos herois da equipe */
        for (i = 0; i < cardinalidade_cjt(mundo->locais[posicao_equipe]->herois); i++)
        {
            j = mundo->locais[posicao_equipe]->herois->v[i];
            mundo->herois[j]->experiencia++;
        }

        return SUCCESS;
    }

    /* Imprime a mensagem de missão impossível de ser realizada */
    printf("%6d:MISSAO %2d IMPOSSIVEL\n", evento->tempo, evento->dado1);

    /* Aloca memória para o evento missao */
    if (!(evento_missao = instanciar_evento_missao(evento->dado1, aleat(mundo->tempo_atual, FIM_DO_MUNDO))))
    {
        /* Imprime mensagem de erro */
        printf("ERRO: Não foi possível alocar memória para o evento missao.\n");

        return FAILURE;
    }

    /* Adiciona o evento missao na lista de eventos */
    if (!(adiciona_ordem_lef(lef, evento_missao)))
    {
        /* Imprime mensagem de erro caso não seja possível adicionar o evento missao na lista de eventos */
        printf("ERRO: Houve um problema ao adicionar o evento missao na lista de eventos.\n");
     
        return FAILURE;
    }

    /* Libera a memória alocada para o evento missao */
    free(evento_missao);

    /* Define o ponteiro para o evento missao como NULL */
    evento_missao = NULL;

    return SUCCESS;
}

/*
 * Função que imprime um evento de chegada
 * Recebe como parâmetros um ponteiro para evento e um ponteiro para o mundo
 */
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

/*
 * Função que imprime um evento de saída
 * Recebe como parâmetros um ponteiro para evento e um ponteiro para o mundo
 */
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

/*
 * Função que imprime um evento de missão
 * Recebe como parâmetros um ponteiro para evento e um ponteiro para o mundo
 */
void imprime_evento_missao(evento_t *evento, mundo_t *mundo)
{
    int tempo_atual, id_missao;

    tempo_atual = evento->tempo;
    id_missao = evento->dado1;

    printf("%6d:MISSAO %3d ", tempo_atual, id_missao);
}

/*
 * Função que imprime um evento de fim e a experiência de cada herói
 * Recebe como parâmetros um ponteiro para evento e um ponteiro para o mundo
 */
void imprime_evento_fim(evento_t *evento, mundo_t *mundo)
{
    int i, tempo_atual, id_heroi, experiencia;

    /* Imprime o evento de fim */
    tempo_atual = evento->tempo;
    printf("%6d:FIM\n", tempo_atual);

    /* Imprime a experiência de cada herói */
    for (i = 0; i < mundo->n_herois; i++)
    {
        id_heroi = mundo->herois[i]->id;
        experiencia = mundo->herois[i]->experiencia;
        printf("HEROI %2d EXPERIENCIA %2d\n", id_heroi, experiencia);
    }
}

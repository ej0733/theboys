#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "libfila.h"
#include "liblef.h"
#include "libconjunto.h"

#define INI 0
#define TAM_MUNDO 20000
#define N_HAB 10
#define TEMPO_FIM 34944

#define CHEGADA 10
#define SAIDA 20
#define MISSAO 30
#define FIM 40

typedef struct heroi
{
    int id;          /* identificação do herói   */
    int paciencia;   /* paciência do herói       */
    int idade;       /* idade do herói           */
    int experiencia;          /* experiência do herói     */
    conjunto_t *hab; /* habilidades do herói     */
} heroi_t;

typedef struct coord
{
    int x;
    int y;
} coord_t;

typedef struct locais
{
    int id;             /* identificação do local               */
    int lotacao_maxima;          /* número máximo de herois              */
    int herois_no_lugar;       /* numero de heróis no local            */
    conjunto_t *herois; /* conjunto de heróis                   */
    fila_t *fila;       /* fila de herois para entrar na cidade */
    coord_t *locallizacao;     /* coordenadas x e y do local           */

} locais_t;

typedef struct mundo
{
    int tempo_atual;         /* tempo atual do mundo                 */
    int n_tamanho_mundo;   /* tamanho do mundo                     */
    int n_herois;      /* número de heróis no mundo            */
    int n_locais;      /* número de locais no mundo            */
    heroi_t **herois;  /* vetor de ponteiros de heróis no mundo*/
    locais_t **locais; /* vetor de ponteiros de locais no mundo*/
    conjunto_t *habilidades;   /* conjunto de habilidades totais       */
} mundo_t;

typedef struct missoes
{
    int id;             /* identificação da missão          */
    conjunto_t *missao; /* conjunto de habilidades de missão*/
} missoes_t;

/* retorna um valor caso ele seja maior que o valor mínimo*/
int maxi(int min, int valor)
{
    if (valor < min)
        return min;
    return valor;
}

/* retorna um valor aleatório entre min e max*/
int aleat(int min, int max)
{
    int r;

    if (min == max)
    {
        return min;
    }
    r = (min + (rand() % ((max - min + 1))));

    return r;
}

/* cria um vetor de ponteiros de herois*/
heroi_t **cria_herois(mundo_t *mundo)
{
    heroi_t **heroi;
    int i;

    /* aloca espaço no vetor de ponteiros heroi*/
    if (!(heroi = malloc((mundo->n_herois) * sizeof(heroi_t *))))
    {
        printf("ERRO 1.4.1: falha ao alocar memória no vetor 'heroi'.\n");
        return NULL;
    }

    /* aloca espaço e adiciona os valores no ponteiro de herois*/
    for (i = 0; i < (mundo->n_herois); i++)
    {
        if (!(*(heroi + i) = malloc(sizeof(heroi_t))))
        {
            printf("ERRO 1.4.2: falha ao alocar memória no heroi %d.\n", i);
            return NULL;
        }
        (*(heroi + i))->id = i;
        (*(heroi + i))->experiencia = 0;
        (*(heroi + i))->paciencia = aleat(0, 100);
        (*(heroi + i))->idade = aleat(18, 100);
        (*(heroi + i))->hab = cria_subcjt_cjt((mundo->habilidades), aleat(2, 5));
    }
    return heroi;
}

/* destrói um vetor de ponteiros de heróis*/
heroi_t **destroi_herois(heroi_t **herois, int tam)
{
    int i;
    for (i = 0; i < tam; i++)
    {
        (*(herois + i))->hab = destroi_cjt((*(herois + i))->hab);
        free(*(herois + i));
    }
    free(herois);
    return NULL;
}

/* cria e retorna uma struct local
 * retorna null em caso de falha*/
locais_t *cria_local(int tam, int id)
{
    locais_t *local;
    /* aloca espaço para a struct local*/
    if (!(local = malloc(sizeof(locais_t))))
    {
        printf("ERRO 1.5.2.1: falha ao alocar memória no local.\n");
        return NULL;
    }

    /* aloca espaço para a struct de coordenadas do local*/
    if (!(local->locallizacao = malloc(sizeof(coord_t))))
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
locais_t **cria_locais(mundo_t *mundo)
{
    locais_t **locais;
    int i;
    if (!(locais = malloc((mundo->n_locais) * sizeof(locais_t *))))
    {
        printf("ERRO 1.5.1: falha ao alocar memória no vetor 'locais'.\n");
        return NULL;
    }
    for (i = 0; i < (mundo->n_locais); i++)
    {
        (*(locais + i)) = cria_local(mundo->n_tamanho_mundo, i);
        if ((*(locais + i)) == NULL)
        {
            printf("ERRO 1.5.2: falha ao alocar memória no vetor 'locais+%d'.\n", i);
            return NULL;
        }
    }
    return locais;
}

/* desaloca memória de um vetor de structs locais
 * retorna null                                     */
locais_t **destroi_locais(mundo_t *mundo)
{
    int i;
    for (i = 0; i < mundo->n_locais; i++)
    {
        (*(mundo->locais + i))->fila = destroi_fila((*(mundo->locais + i))->fila);
        (*(mundo->locais + i))->herois = destroi_cjt((*(mundo->locais + i))->herois);
        free((*(mundo->locais + i))->locallizacao);
        free(*(mundo->locais + i));
    }
    free(mundo->locais);
    return NULL;
}

mundo_t *cria_mundo()
{
    mundo_t *mundo;
    int i;

    if (!(mundo = malloc(sizeof(mundo_t))))
    {
        printf("ERRO 1.1: falha ao alocar memória na variável mundo.\n");
        return NULL;
    }
    mundo->tempo_atual = INI;
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

    mundo->herois = cria_herois(mundo);
    if (mundo->herois == NULL)
    {
        printf("ERRO 1.4: falha na função cria_herois.\n");
        return NULL;
    }
    mundo->locais = cria_locais(mundo);
    if (mundo->locais == NULL)
    {
        printf("ERRO 1.5: falha na função cria_locais.\n");
        return NULL;
    }

    return mundo;
}

mundo_t *destroi_mundo(mundo_t *mundo)
{

    mundo->herois = destroi_herois(mundo->herois, mundo->n_herois);
    mundo->locais = destroi_locais(mundo);
    mundo->habilidades = destroi_cjt(mundo->habilidades);
    free(mundo);

    return NULL;
}

lef_t *cria_cronologia(int n_herois, int n_locais)
{
    lef_t *cronologia;
    evento_t *evento;
    int i;
    cronologia = cria_lef();

    if (cronologia == NULL)
    {
        printf("ERRO 2.1: falha ao alocar memória na variável cronologia.\n");
        return 0;
    }
    if (!(evento = malloc(sizeof(evento_t))))
    {
        printf("ERRO 2.2: falha ao alocar memória na variável evento.\n");
        return 0;
    }
    evento->tipo = FIM;
    evento->tempo = TEMPO_FIM;
    if (!(adiciona_inicio_lef(cronologia, evento)))
    {
        printf("ERRO 2.3: falha ao adicionar o evento evento.\n");
        return 0;
    }
    free(evento);

    for (i = 0; i < n_herois; i++)
    {
        if (!(evento = malloc(sizeof(evento_t))))
        {
            printf("ERRO 2.4: falha ao alocar memória na variável chegada.\n");
            return 0;
        }
        evento->tipo = CHEGADA;
        evento->dado1 = i;
        evento->dado2 = aleat(0, (n_locais - 1));
        evento->tempo = aleat(0, 96 * 7);
        if (!(adiciona_ordem_lef(cronologia, evento)))
        {
            printf("ERRO 2.5: falha ao adicionar o evento chegada %d.\n", i);
            return NULL;
        }
        free(evento);
    }

    return cronologia;
}

int TPL(int paciencia)
{
    return (maxi(1, (paciencia / 10) + (aleat(-2, 6))));
}

int chegada(lef_t *cronologia, evento_t *evento, mundo_t *mundo)
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
    tpl = TPL((*(mundo->herois + (evento->dado1)))->paciencia);
    sai->tempo = (mundo->tempo_atual) + tpl;
    if (!(adiciona_ordem_lef(cronologia, sai)))
    {
        printf("ERRO 2.3: falha ao adicionar o evento sai.\n");
        return 0;
    }
    free(sai);

    return 1;
}

int TDL(mundo_t *mundo, int a, int b, int id)
{
    int d, v;
    int x_a = ((*(mundo->locais + a))->locallizacao)->x;
    int x_b = ((*(mundo->locais + b))->locallizacao)->x;
    int y_a = ((*(mundo->locais + a))->locallizacao)->y;
    int y_b = ((*(mundo->locais + b))->locallizacao)->y;
    int idade = (*(mundo->herois + id))->idade;
    d = sqrt(pow(x_b - x_a, 2) + pow((y_b - y_a), 2));
    v = 100 - maxi(0, idade - 40);
    return ((d / v) / 15);
}

/* trata o evento saída
 * retorna 1 em caso de sucesso e 0 em caso de falha*/
int saida(lef_t *cronologia, evento_t *evento, mundo_t *mundo)
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
    tdl = TDL(mundo, evento->dado2, chegada->dado2, chegada->dado1);
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

/* cria o vetor de missões
 * retorna null em caso de falha*/
missoes_t **cria_missoes(lef_t *cronologia, conjunto_t *hab)
{
    int i, n_missoes = TAM_MUNDO / 100;
    missoes_t **missoes;
    evento_t *evento;
    /* aloca espaço num vetor de n_missoes ponteiros de missões*/
    if (!(missoes = malloc((n_missoes) * sizeof(missoes_t *))))
    {
        printf("ERRO 3.1: falha ao alocar memória no vetor de ponteiros 'missões'.\n");
        return NULL;
    }

    /* cria n_missoes de missões*/
    for (i = 0; i < ((n_missoes)-1); i++)
    {
        /* aloca espaço para uma struct missões*/
        if (!((*(missoes + i)) = malloc(sizeof(missoes_t))))
        {
            printf("ERRO 3.2: falha ao alocar memória no ponteiro 'missões+%d'.\n", i);
            return NULL;
        }
        (*(missoes + i))->id = i;
        ((*(missoes + i))->missao) = cria_subcjt_cjt(hab, (aleat(3, 6)));
        /* aloca espaço para um evento*/
        if (!(evento = malloc(sizeof(evento_t))))
        {
            printf("ERRO 3.3: falha ao alocar memória na variável evento.\n");
            return NULL;
        }
        evento->tipo = MISSAO;
        evento->dado1 = i;
        evento->tempo = aleat(0, TEMPO_FIM);
        /* adiciona o evento de missão na cronologia*/
        if (!(adiciona_ordem_lef(cronologia, evento)))
        {
            printf("ERRO 3.4: falha ao adicionar o evento chegada.\n");
            return NULL;
        }
        /* desaloca o espaço do evento demissão*/
        free(evento);
    }
    return missoes;
}

/* libera espaço no vetor de ponteiros de missões*/
missoes_t **destroi_missoes(missoes_t **missoes)
{
    int i;
    for (i = 0; i < ((TAM_MUNDO / 100) - 1); i++)
    {
        (*(missoes + i))->missao = destroi_cjt((*(missoes + i))->missao);
        free(*(missoes + i));
    }
    free(missoes);
    return NULL;
}

/* cria vetor de conjuntos
 * retorna NULL em caso de falha*/
conjunto_t **cria_vetor_cjt(int tam, int tam2)
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
conjunto_t **liberar_vetor_cjt(conjunto_t **v, int tam)
{
    int i;
    for (i = 0; i < tam; i++)
        *(v + i) = destroi_cjt(*(v + i));

    free(v);
    return NULL;
}

/* procura uma equipe que solucuine a missão
 * devolve -1 caso não exista               */
int solucao(int tam, locais_t **locais, heroi_t **herois, conjunto_t *missao, evento_t *evento)
{
    conjunto_t *uni_hab, **hab_hero;
    int i, j, card1, card2, posi, tam_max = N_HAB;

    /* cria um vetor com a união das habilidades dos herois da equipe*/
    hab_hero = cria_vetor_cjt(tam, tam_max);

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
    hab_hero = liberar_vetor_cjt(hab_hero, tam);

    return posi;
}

/* trata o evento missão
 * retorna 1 se for bem sucedido de e 0 em caso de falha*/
int missao(lef_t *cronologia, evento_t *evento, mundo_t *mundo, missoes_t **missoes)
{
    int posi, i, j;
    evento_t *missao;

    /* imprime as habilidades necessárias para a missão*/
    printf("HAB_REQ ");
    imprime_cjt(((*(missoes + (evento->dado1)))->missao));

    /* verifica se existe um conjunto de heróis que possa realizar a missão
     * retorna a id de 0 até n_locais caso exista e -1 caso contrário*/
    posi = solucao((mundo->n_locais), (mundo->locais), (mundo->herois), ((*(missoes + (evento->dado1)))->missao), evento);

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

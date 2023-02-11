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

typedef struct par_ordenado
{
    int x;
    int y;
} par_ordenado_t;

typedef struct locais
{
    int id;             /* identificação do local               */
    int lotacao_maxima;          /* número máximo de herois              */
    int herois_no_lugar;       /* numero de heróis no local            */
    conjunto_t *herois; /* conjunto de heróis                   */
    fila_t *fila;       /* fila de herois para entrar na cidade */
    par_ordenado_t *locallizacao;     /* coordenadas x e y do local           */

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

/* Responsável por criar o mundo, alocando espaço para o mesmo e inicializando
 * os seus campos.
 * Retorna um ponteiro para o mundo criado.*/
mundo_t *cria_mundo();

lef_t *cria_cronologia(int n_herois, int n_locais);

missoes_t **cria_missoes(lef_t *cronologia, conjunto_t *hab);

int chegada(lef_t *cronologia, evento_t *evento, mundo_t *mundo);

int saida(lef_t *cronologia, evento_t *evento, mundo_t *mundo);

int missao(lef_t *cronologia, evento_t *evento, mundo_t *mundo, missoes_t **missoes);

missoes_t **destroi_missoes(missoes_t **missoes);

mundo_t *destroi_mundo(mundo_t *mundo);

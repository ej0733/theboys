#include "libfila.h"
#include "liblef.h"
#include "libconjunto.h"

#define TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define FIM_DO_MUNDO 34944

#define TIPO_CHEGADA 0
#define TIPO_SAIDA 1
#define TIPO_MISSAO 2
#define TIPO_FIM 3

typedef struct heroi
{
    int id;
    int paciencia;
    int idade;
    int experiencia;
    conjunto_t *habilidades;
} heroi_t;

typedef struct par_ordenado
{
    int x;
    int y;
} par_ordenado_t;

typedef struct local
{
    int id;
    int lotacao_maxima;
    int herois_no_lugar;
    conjunto_t *herois;
    fila_t *fila;
    par_ordenado_t *locallizacao;

} local_t;

typedef struct mundo
{
    int tempo_atual;
    int n_tamanho_mundo;
    int n_herois;
    int n_locais;
    heroi_t **herois;
    local_t **locais;
    conjunto_t *habilidades;
} mundo_t;

typedef struct missao
{
    int id;
    conjunto_t *missao;
} missao_t;

/* Responsável por criar o mundo, alocando espaço para o mesmo e inicializando
 * os seus campos.
 * Retorna um ponteiro para o mundo criado.*/
mundo_t *instanciar_mundo();

mundo_t *destruir_mundo(mundo_t *mundo);

lef_t *instanciar_lef(int n_herois, int n_locais);

missao_t **instanciar_missoes(lef_t *cronologia, conjunto_t *hab);

missao_t **destruir_missoes(missao_t **missoes);

int max(int num1, int num2);

int aleat(int min, int max);

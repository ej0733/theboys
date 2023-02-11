#include "libfila.h"
#include "liblef.h"
#include "libconjunto.h"

#define INI 0
#define TAM_MUNDO 20000
#define N_HAB 10
#define TEMPO_FIM 34944

#define CHEGADA 0
#define SAIDA 1
#define MISSAO 2
#define FIM 3

typedef struct heroi
{
    int id;
    int paciencia;
    int idade;
    int experiencia;
    conjunto_t *hab;
} heroi_t;

typedef struct par_ordenado
{
    int x;
    int y;
} par_ordenado_t;

typedef struct locais
{
    int id;
    int lotacao_maxima;
    int herois_no_lugar;
    conjunto_t *herois;
    fila_t *fila;
    par_ordenado_t *locallizacao;

} locais_t;

typedef struct mundo
{
    int tempo_atual;
    int n_tamanho_mundo;
    int n_herois;
    int n_locais;
    heroi_t **herois;
    locais_t **locais;
    conjunto_t *habilidades;
} mundo_t;

typedef struct missoes
{
    int id;
    conjunto_t *missao;
} missoes_t;

/* Responsável por criar o mundo, alocando espaço para o mesmo e inicializando
 * os seus campos.
 * Retorna um ponteiro para o mundo criado.*/
mundo_t *instanciar_mundo();

lef_t *instanciar_lef(int n_herois, int n_locais);

missoes_t **instanciar_missoes(lef_t *cronologia, conjunto_t *hab);

int tratar_evento_chegada(lef_t *cronologia, evento_t *evento, mundo_t *mundo);

int tratar_evento_saida(lef_t *cronologia, evento_t *evento, mundo_t *mundo);

int tratar_evento_missao(lef_t *cronologia, evento_t *evento, mundo_t *mundo, missoes_t **missoes);

missoes_t **destruir_missoes(missoes_t **missoes);

mundo_t *destruir_mundo(mundo_t *mundo);

#include "libmundo.h"

int tratar_evento_chegada(lef_t *cronologia, evento_t *evento, mundo_t *mundo);

int tratar_evento_saida(lef_t *cronologia, evento_t *evento, mundo_t *mundo);

int tratar_evento_missao(lef_t *cronologia, evento_t *evento, mundo_t *mundo, missao_t **missoes);

void imprime_evento_chegada(evento_t *evento, mundo_t *mundo);

void imprime_evento_saida(evento_t *evento, mundo_t *mundo);

void imprime_evento_missao(evento_t *evento, mundo_t *mundo);

void imprime_evento_fim(evento_t *evento, mundo_t *mundo);


#ifndef _ROTA_H
#define _ROTA_H
#include <stdbool.h>
//#include <gmp.h>

#define TRUE 1
#define FALSE 0
#define QTGERA 20000
#define TAMPOP 60
#define TAMCROMO 30
#define TAXASEL 1
#define TAXACRUZ 1
#define TAXAMUTA 0.80

#define LIN 5
#define COL 6
//#define INSTALL_DEBUG

typedef struct {
    int linha;
    int col;
    int dado;
}posicao;

typedef struct {
    double inf;
    double sup;
    double porc;
    posicao** p;
}faixas_roleta;

void init_mapa();
void criapop(void);
void embaralha_alelos(int i);
void avaliapop(void);
void ordenar_cromo(unsigned long long int v[][TAMPOP]);
void pos_cromo(posicao** cromo, int g, int * j);
void roleta();
unsigned long long int dis(posicao * inicio, posicao * atual);
posicao** selecionapais();
void reproduzpop(void);
void elitismo();
bool cruzapais(posicao** pai_1, posicao** pai_2, int* j1, int* j2);
void mutapais(int j);
void verifica_repeticoes(int j_pai, int j_filho, int pt_corte_1, int pt_corte_2);
int checaparada(void);
void mostrapop(void);
void motrar_nota_geracao();
unsigned long long int reavalia(int _final, int j);

#endif

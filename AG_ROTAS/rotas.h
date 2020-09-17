#ifndef _ROTA_H
#define _ROTA_H
#include <stdbool.h>

#define TRUE 1
#define FALSE 0
#define QTGERA 1000
#define TAMPOP 60
#define TAMCROMO 30
#define TAXASEL 0.80
#define TAXACRUZ 1
#define TAXAMUTA 0.05
#define LIN 5
#define COL 6
//#define INSTALL_DEBUG

typedef struct {
    int linha;
    int col;
    int dado;
}posicao;

typedef struct {
    float inf;
    float sup;
    float porc;
    posicao** p;
}faixas_roleta;

#ifdef INSTALL_DEBUG
void print_mapa();
void teste_roleta();
void teste_gera();
void debug(posicao p1, posicao p2);
void print_cromo(int j, int i);
void debug_pais(posicao** p1, posicao** p2);
void teste_unit_cromo(posicao** p1, posicao** p2, posicao** f1, posicao** f2, int corte);
void debug_cruzamento(int j, int l);
void debug_muta(posicao** p1);
#endif // INSTALL_DEBUG

void init_mapa();
void criapop(void);
void embaralha_alelos(int i);
void avaliapop(void);
void ordenar_cromo(int v[][TAMPOP]);
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

#endif

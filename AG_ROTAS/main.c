#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define QTGERA 1000
#define TAMPOP 100
#define TAMCROMO 30
#define TAXASEL 0.80
#define TAXACRUZ 0.90
#define TAXAMUTA 0.05


#define INSTALL_DEBUGE
#define LIN 5
#define COL 6

typedef struct {
    int linha;
    int col;
    int dado;
}posicao;

posicao inicio = {4, 0, 25};
posicao final = {1, 5, 12};

void criapop(void);
void avaliapop(void);
void reproduzpop(void);
void selecionapais(void);
void cruzapais(void);
void mutapais(void);
int checaparada(void);
void mostrapop(void);

int i_geraativa = 0; //geracao atual
void * m_i_pop[QTGERA][TAMPOP][TAMCROMO]; //matriz de cromossomos
float m_f_popaval[QTGERA][TAMPOP]; //matriz de avalia��es
float m_f_estataval[QTGERA][3]; //matriz de estatisticas: fitness m�nimo, m�ximo, m�dio
int i_pai1; //primeiro pai selecionado
int i_pai2; //segundo pai selecionado

typedef struct
{
	void * cromo;
	int peso_cromo[TAMCROMO];
}pesos_pop;

pesos_pop pesos_cromo_pop[TAMPOP];

void init_mapa(void);
void print_mapa(void);
void testa_cria();
int dis(posicao inicio, posicao atual);

posicao matriz[LIN][COL];

int main(void) {

	srand(time(NULL));
	init_mapa();
	print_mapa();


	criapop();
	testa_cria();
	while(TRUE) {
		avaliapop();
		reproduzpop();

		if(checaparada()) break;
	}
	mostrapop();

	return 0;
}

void init_mapa()
{
    int i, j, cont = 0;
    for(i = 0; i < LIN; i++) {
        for(j = 0; j < COL; j++) {
            matriz[i][j].dado = ++cont;
            matriz[i][j].linha = i;
            matriz[i][j].col = j;
        }
    }
}

void testa_cria()
{
    printf("\n\nTestando primeira ciracao\n");
    int i, j, k;
    for(i = 0; i < 1; i++) {
        for(j = 0; j < 2; j++) {
            for(k = 0; k < TAMCROMO; k++) {
            posicao * pos =  (posicao *) m_i_pop[i][j][k];
            printf("[%d(%d, %d)], ",  pos->dado, pos->linha, pos->col);
            }
            printf("\n");
        }
        printf("\n");
    }

}
void print_mapa()
{
    int i, j;
    for(i = 0; i < LIN; i++) {
        for(j = 0; j < COL; j++) {
            printf("[%d][%d] - %d, ", matriz[i][j].linha, matriz[i][j].col, matriz[i][j].dado);
        }
        printf("\n");
    }
}

void criapop(void) {
    int i, j, k;
    for(i = 0; i < 1; i++) {
        for(j = 0; j < 2; j++) {
            for(k = 0; k < TAMCROMO; k++) {
                m_i_pop[i][j][k] = &matriz[rand() % (LIN - 1)][rand() % (COL - 1)];
            }
        }
    }
    ++i_geraativa;
	return;
}

void avaliapop(void) {
    int i, j, k, v;
    //posicao * pos = (posicao *)m_i_pop[i][j][k];
    //debug(inicio, *pos);
    for(i = 0; i < i_geraativa; i++) {
        for(j = 0; i < TAMPOP; j++) {
            for(k = 0;  k < TAMCROMO; k++){
                if(k == 0) {

                	pesos_cromo_pop[j].cromo = m_i_pop[i][j][k];
                	posicao * pos = pesos_cromo_pop[j].cromo;
                	pesos_cromo_pop[j].peso_cromo[k] = dis(inicio, *pos);
                	debug(inicio, pesos_cromo_pop[j].cromo);
                	printf("\ndistancia %d\n", pesos_cromo_pop[j].peso_cromo[k]);
                	exit(1);
                }
            }
        }
    }
	return;
}

void debug(posicao p1, posicao p2)
{
    printf("[%d(%d, %d)], [%d(%d, %d)], dist = %d", p1.dado, p1.linha, p1.col, p2.dado, p2.linha, p2.col, dis(p1, p2));
}

int dis(posicao inicio, posicao atual)
{
    int lin = (inicio.linha - atual.linha) * (inicio.linha - atual.linha);
    int col = (inicio.col - atual.col) * (inicio.col - atual.col);
    return lin + col;
}

void reproduzpop(void) {

	int _i_novapop = 0;

	while(_i_novapop < TAMPOP) {
		selecionapais();
		cruzapais();
		mutapais();

		_i_novapop+=2;
	}

	return;
}

void selecionapais(void) {

	if((rand()%100)<=TAXASEL) {

	}

	return;
}

void cruzapais(void) {

	if((rand()%100)<=TAXACRUZ) {

	}

	return;
}

void mutapais(void) {

	if((rand()%100)<=TAXAMUTA) {

	}
	return;
}

int checaparada(void){

	return TRUE;
}

void mostrapop(void) {

	return;
}

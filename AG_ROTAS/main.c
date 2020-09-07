#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define QTGERA 1000
#define TAMPOP 60
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
typedef struct {
    float inf;
    float sup;
    float porc;
    posicao ** p;
}faixas_roleta;

faixas_roleta fx_roleta[TAMPOP];

posicao inicio = {4, 0, 25};
posicao final = {1, 5, 12};

void criapop(void);
void avaliapop(void);
void reproduzpop(void);
void cruzapais(void);
void mutapais(void);
int checaparada(void);
void mostrapop(void);

int i_geraativa = 0; //geracao atual
posicao * m_i_pop[QTGERA][TAMPOP][TAMCROMO + 1] = {0, 0, 0}; //matriz de cromossomos
int m_f_popaval[QTGERA][TAMPOP]; //matriz de avalia��es
posicao * * indice_notas[TAMCROMO];
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
int dis(posicao * inicio, posicao * atual);
void teste_gera(float total);

posicao matriz[LIN][COL];

int main(void) {

	srand(time(NULL));
	init_mapa();
	//print_mapa();


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
    printf("\n\nTestando primeira geracao\n");
    int i, j, k;
    for(i = 0; i < 1; i++) {
        for(j = 0; j < TAMPOP; j++) {
            for(k = 0; k < TAMCROMO; k++) {
                posicao * pos =  (posicao *) m_i_pop[i][j][k];
                printf("%d.%d [%d]\n", j + 1, k + 1, pos->dado);
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
        for(j = 0; j < TAMPOP; j++) {
            for(k = 0; k < TAMCROMO; k++) {
                m_i_pop[i][j][k] = &matriz[rand() % (LIN - 1)][rand() % (COL - 1)];
            }
        }
    }
	return;
}

void avaliapop(void) {
    int j, k, i = 0;
    float soma_pesos = 0;
    for(j = 0; j < TAMPOP; j++) {
        posicao * pos = m_i_pop[i_geraativa][j][0]; //cromo-init
        for(k = 0;  k < TAMCROMO; k++){
            if(k == 0)
                m_f_popaval[i_geraativa][j] += dis(&inicio, pos);
            else
                m_f_popaval[i_geraativa][j] += dis(m_i_pop[i_geraativa][j][k - 1], m_i_pop[i_geraativa][j][k]);
        }
        indice_notas[j] = &(m_i_pop[i_geraativa][j][0]);
        soma_pesos += m_f_popaval[i_geraativa][j];
        //printf("[Cromo %d, dado= %d] peso= %d\n", j + 1, (*indice_notas[j])->dado, m_f_popaval[i_geraativa][j]);
        //exit(1);
    }

    printf("\npesos totais %f\n\n", soma_pesos);
    ordenar_cromo(m_f_popaval);
    roleta(soma_pesos);
    teste_gera(soma_pesos);
    teste_roleta();
    exit(1);
    //i_geraativa += 1;
}

void ordenar_cromo(int v[][TAMPOP])
{
    int i, j, x;
    posicao * p;
	for (j = 1; j < TAMPOP; ++j) {
		x = v[i_geraativa][j];
		p = indice_notas[j];
		for (i = j - 1; i >= 0 && v[i_geraativa][i] < x; --i){
            v[i_geraativa][i + 1] = v[i_geraativa][i];
            indice_notas[i + 1] = indice_notas[i];
		}
		v[i_geraativa][i + 1] = x;
		indice_notas[i + 1] = p;
	}
}
void pos_cromo(posicao * * cromo, int g, int * j)
{
    int i;
    for(i = 0; i < TAMPOP; i++) {
        posicao ** pos  = &m_i_pop[g][i][0];
        if(pos == cromo) {
            *j = i;
             return;
        }

    }
    return NULL;
}

void roleta(double total)
{
    int i;
    for(i = 0; i < TAMPOP; i++) {
        fx_roleta[i].porc = m_f_popaval[i_geraativa][i] / total;
        fx_roleta[i].inf = i == 0 ? 0 : fx_roleta[i - 1].sup ;
        fx_roleta[i].sup = fx_roleta[i].inf + (fx_roleta[i].porc * total);
        fx_roleta[i].p = indice_notas[TAMPOP - i];
    }
}

void teste_roleta()
{
    printf("\n\n *** TESTE ROLETA ***\n\n");
    int i, j, k;
    for(int i =0; i < TAMPOP; i++) {
        pos_cromo(fx_roleta[i].p, i_geraativa, &j); //retorna indice no endere
        for(k = 0; k < TAMCROMO; k++) {
           posicao * pos = m_i_pop[i_geraativa][j][k];
            printf("%d.%d [%d]\n", j + 1, k + 1, pos->dado);
        }
        printf("porc= %f, lim_inf= %f, lim_sup= %f\n\n", fx_roleta[i].porc, fx_roleta[i].inf, fx_roleta[i].sup);
    }
}


void teste_gera(float total)
{
    printf("\ntotal f = %f\n", total);
    int j, k, i;
    for(j = 0; j < TAMPOP; j++) {
        pos_cromo(indice_notas[j], i_geraativa, &i);
        for(k = 0; k < TAMCROMO; k++) {
            posicao * pos = m_i_pop[i_geraativa][i][k];
            printf("%d.%d [%d]\n", i + 1, k + 1, pos->dado);
        }
        printf("Total= %d, pct = %f\n\n", m_f_popaval[i_geraativa][j], m_f_popaval[i_geraativa][j] / total);
    }
}

void debug(posicao p1, posicao p2)
{
    printf("[%d(%d, %d)], [%d(%d, %d)], dist = %d", p1.dado, p1.linha, p1.col, p2.dado, p2.linha, p2.col, dis(&p1, &p2));
}

int dis(posicao * inicio, posicao * atual)
{
    int lin = (inicio->linha - atual->linha) * (inicio->linha - atual->linha);
    int col = (inicio->col - atual->col) * (inicio->col - atual->col);
    return lin + col;
}

void selecionapais()
{

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

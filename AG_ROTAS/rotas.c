#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "rotas.h"

faixas_roleta fx_roleta[TAMPOP];
posicao inicio = {4, 0, 25};
posicao final = {1, 5, 12};
int i_geraativa = 0; //geracao atual
posicao* m_i_pop[QTGERA][TAMPOP][TAMCROMO + 1] = {0, 0, 0}; //matriz de cromossomo
unsigned long long int m_f_popaval[QTGERA][TAMPOP]; //matriz de avalia��es
posicao** indice_notas[TAMCROMO];
float m_f_estataval[QTGERA][3]; //matriz de estatisticas: fitness m�nimo, m�ximo, m�dio
posicao** i_pai1; //primeiro pai selecionado
posicao** i_pai2; //segundo pai selecionado
unsigned long long int soma_pesos = 0;
posicao matriz[LIN][COL];

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

void print_mapa()
{
    printf("\n\n*** PRINT MAPA ***\n\n");
    int i, j;
    for(i = 0; i < LIN; i++) {
        for(j = 0; j < COL; j++) {
            printf("[%d][%d] - %d, ", matriz[i][j].linha, matriz[i][j].col, matriz[i][j].dado);
        }
        printf("\n");
    }
}

void criapop(void) {
    int i, j, k = 0, i_, j_;
    for(i = 0; i < 1; i++) { //for gerecao
        for(j = 0; j < TAMPOP; j++) { //for cromosso
            for(i_ = 0; i_ < LIN; i_++) {
                for(j_ = 0; j_ < COL; j_++) {
                    m_i_pop[i][j][k++] = &matriz[i_][j_];
                }
            }
            k = 0;
        }
        embaralha_alelos(i);
    }
}

void embaralha_alelos(int i)
{
    int j, k;
    for(j = 0; j < TAMPOP; j++) {
        for(k = 0; k < TAMCROMO; k++) {
            posicao * troca_aux = m_i_pop[i][j][k];
            int troca = rand() % TAMCROMO;
            m_i_pop[i][j][k] = m_i_pop[i][j][troca];
            m_i_pop[i][j][troca] = troca_aux;
        }
    }
}

unsigned long long int dis(posicao * inicio, posicao * atual)
{
    unsigned int lin = sqrt(pow((inicio->linha - atual->linha), 2));
    unsigned int col = sqrt(pow((inicio->col - atual->col), 2));
    return pow((lin + col) * 20, 2);
}

void avaliapop(void) {
    int j, k;
    unsigned long long int peso = 0;
    soma_pesos = 0;
    for(j = 0; j < TAMPOP; j++) {
         for(k = 0;  k < TAMCROMO; k++) {
            posicao * pos = m_i_pop[i_geraativa][j][k]; //cromo-init
            if(k == 0) {
                m_f_popaval[i_geraativa][j] = 0;
            }
            else {
                peso = pow(dis(m_i_pop[i_geraativa][j][k - 1], m_i_pop[i_geraativa][j][k]), 1);
                m_f_popaval[i_geraativa][j] += peso;
            }
            if (k != 0 && pos->dado == final.dado) {
                m_f_popaval[i_geraativa][j] += k;
                m_f_popaval[i_geraativa][j] += reavalia(k, j);
                break;
            }
        }
        indice_notas[j] = &(m_i_pop[i_geraativa][j][0]); //guarda o endereco do cromosso
        soma_pesos += m_f_popaval[i_geraativa][j]; //conteudo da nota
    }
    ordenar_cromo(m_f_popaval);
    roleta(soma_pesos);
}

unsigned long long int reavalia(int _final, int j)
{
    unsigned long long int peso = 0;
    return peso = (m_i_pop[i_geraativa][j][0]->dado != inicio.dado) ? (m_f_popaval[i_geraativa][j]) * 100 : peso;
}

void ordenar_cromo(unsigned long long int v[][TAMPOP])
{
    int i, j, x;
    posicao * p;
	for (j = 1; j < TAMPOP; ++j) {
		x = v[i_geraativa][j];
		p = indice_notas[j];
		for (i = j - 1; i >= 0 && v[i_geraativa][i] < x; --i) {
            v[i_geraativa][i + 1] = v[i_geraativa][i];
            indice_notas[i + 1] = indice_notas[i];
		}
		v[i_geraativa][i + 1] = x;
		indice_notas[i + 1] = p;
	}
}

void bests_cromo()
{
    int j;
    pos_cromo(indice_notas[TAMPOP - 1], i_geraativa, &j);
    printf("\ni = %d, j = %d\n", i_geraativa, j);
    //print_cromo(j, i_geraativa);
    //exit(111);
}

void pos_cromo(posicao** cromo, int g, int * j)
{
    int i;
    for(i = 0; i < TAMPOP; i++) {
        posicao** pos  = &m_i_pop[g][i][0];
        if(pos == cromo) {
            *j = i;
             return;
        }
    }
}

void roleta()
{
    int k;
    for(k = 0; k < TAMPOP; k++) {
        fx_roleta[k].porc = (double)m_f_popaval[i_geraativa][k] / soma_pesos;
        fx_roleta[k].inf = k == 0 ? 0 : fx_roleta[k - 1].sup ;
        fx_roleta[k].sup = fx_roleta[k].inf + (fx_roleta[k].porc);
        fx_roleta[k].p = indice_notas[(TAMPOP - 1) - k];
    }
}

posicao** selecionapais()
{
    double n = rand() / (double)RAND_MAX;
    for(int i = 0; i < TAMPOP; i++) {
        if(n > fx_roleta[i].inf && n <= fx_roleta[i].sup) {
            return fx_roleta[i].p;
        }
    }
    return NULL;
}

void print_cromo(int j, int i)
{
    printf("Print Cromo%d\n", j);
    int k;
    for(k = 0; k < TAMCROMO; k++)
        printf("C.%d.%d dado= %d\n", j + 1, k + 1, m_i_pop[i][j][k]->dado);
}

void reproduzpop(void) {

	int _i_novapop = 0, j1, j2;
    i_geraativa += 1;
    posicao** i_pai1_;
    posicao** i_pai2_;
    elitismo();

	while(_i_novapop < TAMPOP) {
        do {
            while(!((i_pai1_ = selecionapais()) != NULL));
            while(!((i_pai2_ = selecionapais()) != NULL));
            do {
                if(i_pai2_ == i_pai1_)
                    while(!((i_pai2_ = selecionapais()) != NULL));
                else
                    break;
            } while(!(i_pai2_ != i_pai1_));
            pos_cromo(i_pai1_, i_geraativa - 1, &j1);
            pos_cromo(i_pai2_, i_geraativa - 1, &j2);

        } while(!cruzapais(i_pai1_, i_pai2_, &j1, &j2));
		mutapais(_i_novapop);
		mutapais(_i_novapop + 1);
		_i_novapop+=2;
	}
}

void elitismo()
{
    int j1, j2, k;
    pos_cromo(indice_notas[TAMPOP - 1], i_geraativa - 1, &j1);
    pos_cromo(indice_notas[TAMPOP - 2], i_geraativa - 1, &j2);
    for(k = 0; k < TAMCROMO; k++) {
        posicao * pos1 = m_i_pop[i_geraativa - 1][j1][k];
        posicao * pos2 = m_i_pop[i_geraativa - 1][j2][k];
        m_i_pop[i_geraativa][0][k] = pos1;
        m_i_pop[i_geraativa][1][k] = pos2;
    }
}

bool cruzapais(posicao** pai_1, posicao** pai_2, int* j1, int* j2) {
	int static id_cruz = 0;
	if(((double)rand() / RAND_MAX)<=TAXACRUZ) {
        int j_pai, k, l_mae;
        int pt_corte_1, pt_corte_2;
        pt_corte_1 = rand() % (TAMCROMO);
        pt_corte_2 = pt_corte_1 + rand() % (TAMCROMO  - pt_corte_1);
        pos_cromo(pai_1, i_geraativa - 1, &j_pai);
        pos_cromo(pai_2, i_geraativa - 1, &l_mae);
        for(k = 0; k < TAMCROMO; k++) {
            if(k >= pt_corte_1 && k < pt_corte_2) {
                m_i_pop[i_geraativa][id_cruz + 1][k] = m_i_pop[i_geraativa - 1][j_pai][k]; //filho mae
                m_i_pop[i_geraativa][id_cruz][k] = m_i_pop[i_geraativa - 1][l_mae][k]; //filho pai
            }
            else {
                m_i_pop[i_geraativa][id_cruz][k] = m_i_pop[i_geraativa - 1][j_pai][k];
                m_i_pop[i_geraativa][id_cruz + 1][k] = m_i_pop[i_geraativa - 1][l_mae][k];
            }
        }
        verifica_repeticoes(j_pai, id_cruz, pt_corte_1, pt_corte_2);
        verifica_repeticoes(l_mae, id_cruz + 1, pt_corte_1, pt_corte_2);
        *j1 = id_cruz;
        *j2 = id_cruz + 1;

        id_cruz  += 2;
        id_cruz = (id_cruz == TAMPOP) ? 0 : id_cruz;
        return true;
    }
    return false;
}

void verifica_repeticoes(int j_pai, int j_filho, int pt_corte_1, int pt_corte_2)
{
    int k, k1;
    bool cked_1, cked_2;
    do {
        cked_1 = true;
        cked_2 = true;
        for(k = TAMCROMO - 1; k >= pt_corte_2; k--) {
            for(k1 = pt_corte_1; k1 < pt_corte_2; k1++) {
                if(m_i_pop[i_geraativa][j_filho][k]->dado == m_i_pop[i_geraativa][j_filho][k1]->dado){//go do, comparar end ou dado?
                     m_i_pop[i_geraativa][j_filho][k] = m_i_pop[i_geraativa - 1][j_pai][k1];
                    cked_1 = false;
                    break;
                }
            }
        }
        for(k = 0; k < pt_corte_1; k++){
            for(k1 = pt_corte_1; k1 < pt_corte_2; k1++){
                if(m_i_pop[i_geraativa][j_filho][k]->dado == m_i_pop[i_geraativa][j_filho][k1]->dado){//go do, comparar end ou dado?
                     m_i_pop[i_geraativa][j_filho][k] = m_i_pop[i_geraativa - 1][j_pai][k1];
                    cked_2 = false;
                    break;
                }
            }
        }
    } while(!(cked_1 && cked_2));
}

void mutapais(int j) {
    if(((double)rand() / RAND_MAX) <= TAXAMUTA) {
	    int pt_mt_1 = rand() % TAMCROMO;
	    int pt_mt_2 = rand() % TAMCROMO;
	    posicao* aux = m_i_pop[i_geraativa][j][pt_mt_1];
        m_i_pop[i_geraativa][j][pt_mt_1] = m_i_pop[i_geraativa][j][pt_mt_2];
        m_i_pop[i_geraativa][j][pt_mt_2] = aux;
	}
}

int checaparada(void){

	return (i_geraativa == QTGERA - 1);
}

void mostrapop(void) {
    printf("\n\n**PRINT POPULACAO***\n\n");
    int j, k;
    for(j = 0; j < TAMPOP; j++) {
        for(k = 0; k < TAMCROMO; k++) {
            printf("G.%d, %d.%d dado= %d, [%d][%d]\n",
                    i_geraativa + 1, j + 1, k + 1, m_i_pop[i_geraativa][j][k]->dado,
                     m_i_pop[i_geraativa][j][k]->linha, m_i_pop[i_geraativa][j][k]->col);
        }
        printf("\n");
    }
	return;
}

void print_arq_rota()
{
    int k;
    int j;
    FILE *ptr_arq;
    ptr_arq = fopen("result_rota.txt","w");
    fprintf(ptr_arq , "%s", "v\n");
    pos_cromo(indice_notas[TAMPOP - 1], i_geraativa, &j);
    for(k = 0; m_i_pop[QTGERA - 1][j][k]->dado != final.dado; k++) {
        fprintf(ptr_arq , "%d\n", m_i_pop[QTGERA - 1][j][k]->dado);
    }
    fprintf(ptr_arq , "%d\n", m_i_pop[QTGERA - 1][j][k]->dado);
    fprintf(ptr_arq , "%s", "v\n");
    fclose(ptr_arq);
}

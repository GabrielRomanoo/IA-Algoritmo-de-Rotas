#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "rotas.h"
#include <gmp.h>
/*
 * Variaveis Globais
 */
posicao inicio = {4, 0, 25};
posicao final = {1, 5, 12};
int i_geraativa = 0; //geracao atual
posicao* m_i_pop[QTGERA][TAMPOP][TAMCROMO + 1] = {0, 0, 0}; //matriz de cromossomo
mpz_t m_f_popaval[QTGERA][TAMPOP]; //mp
posicao** indice_notas[TAMCROMO];
posicao** i_pai1; //primeiro pai selecionado
posicao** i_pai2; //segundo pai selecionado
mpz_t soma_pesos; //mp
posicao matriz[LIN][COL];

typedef struct {
    mpf_t inf;
    mpf_t sup;
    mpf_t porc;
    posicao** p;
}faixas_roleta;

faixas_roleta fx_roleta[TAMPOP];

void init_var_pt_flt()
{
    int i, j;
    for(i = 0; i < TAMPOP; i++) {
        mpf_init(fx_roleta[i].inf);
        mpf_init(fx_roleta[i].sup);
        mpf_init(fx_roleta[i].porc);
    }
    for(i = 0; i < QTGERA; i++) {
        for(j = 0; j < TAMPOP; j++){
           mpz_init(m_f_popaval[i][j]);
           mpz_set_si(m_f_popaval[i][j], 0);
        }
    }
    mpz_init(soma_pesos);
    mpz_set_si(soma_pesos, 0);
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
    for(i = 0; i < 1; i++){ //for gerecao
        for(j = 0; j < TAMPOP; j++){ //for cromosso
            for(i_ = 0; i_ < LIN; i_++){
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
    for(j = 0; j < TAMPOP; j++){
        for(k = 0; k < TAMCROMO; k++){
            posicao * troca_aux = m_i_pop[i][j][k];
            int troca = rand() % TAMCROMO;
            m_i_pop[i][j][k] = m_i_pop[i][j][troca];
            m_i_pop[i][j][troca] = troca_aux;
        }
    }
}


void dis(posicao * inicio, posicao * atual, mpz_t* result_dis)
{
    mpz_t sum;
    mpz_init(sum);
    unsigned long int lin = sqrt(pow((inicio->linha - atual->linha), 2));
    unsigned long int col = sqrt(pow((inicio->col - atual->col), 2));
     /*printf("\nG.%d, (p1.dado= %d, %d,%d) , (p2.dado= %d, %d.%d) .TT= %d\n",
                    i_geraativa, inicio->dado, inicio->linha, inicio->col,
                    atual->dado, atual->linha, atual->col, lin + col);
                    //exit(111);*/

    mpz_set_ui(sum, (pow(lin + col), 2));
    mpz_set(*result_dis, sum);
    mpz_clear(sum);
}

void avaliapop(void) {
    int j, k;
    mpz_t peso, soma_pesos_aux;
    mpz_init(peso);
    mpz_init(soma_pesos_aux);
    mpz_set_si(soma_pesos, 0);
    mpz_set_si(peso, 0);
    mpz_set_si(soma_pesos_aux, 0);
    for(j = 0; j < TAMPOP; j++) {
         for(k = 0;  k < TAMCROMO; k++){
            posicao * pos = m_i_pop[i_geraativa][j][k]; //cromo-init
            if(k == 0){
                dis(&inicio, m_i_pop[i_geraativa][j][0], &peso); //atribui nota
                mpz_set(m_f_popaval[i_geraativa][j], peso);
                //m_f_popaval[i_geraativa][j] += 0;
            }
            else {
                dis(m_i_pop[i_geraativa][j][k - 1], m_i_pop[i_geraativa][j][k], &peso);//funcao de custo
                mpz_add(m_f_popaval[i_geraativa][j], peso, m_f_popaval[i_geraativa][j]);
                dis(&final, pos, &peso); //atribui peso do final.
                mpz_add(m_f_popaval[i_geraativa][j], peso, m_f_popaval[i_geraativa][j]);
            }
                /*Para reavaliacoes
             if(k != 0 && pos->dado == final.dado) {
                int k1;
                mpz_set_si(peso, 0);
                for(k1 = 0; k1 <= k ; k1++){
                    dis(&inicio, m_i_pop[i_geraativa][j][k1], &peso);
                    mpz_add(peso, peso, peso);
                }

                m_f_popaval[i_geraativa][j] += peso_;
                m_f_popaval[i_geraativa][j] += pow(reavalia(k, j), 2);
               //m_f_popaval[i_geraativa][j] += pow(sqrt(pow(pow(k, 2) - m_f_popaval[i_geraativa][j], 2)), 2);
                break;
            }*/

        }
        //m_f_popaval[i_geraativa][j] += pow(reavalia_final(j, TAMCROMO), 2);
        indice_notas[j] = &(m_i_pop[i_geraativa][j][0]); //guarda o endereco do cromosso
        mpz_set(soma_pesos_aux, m_f_popaval[i_geraativa][j]); //conteudo da nota
        mpz_add(soma_pesos, soma_pesos, soma_pesos_aux)
        mpz_set_si(soma_pesos_aux, 0);
        //printf("[Cromo %d, dado= %d] peso= %llu\n\n", j + 1, (*indice_notas[j])->dado, m_f_popaval[i_geraativa][j]);
    }

    printf("\npesos totais %llu\n", soma_pesos);

    ordenar_cromo(m_f_popaval);
    //teste_gera(soma_pesos); //debug para ver se funciona ordenacao.
    roleta(soma_pesos);
    teste_roleta();
    exit(255);

    //reavalia();
    //teste_gera(soma_pesos);

}


unsigned long long int reavalia(int _final, int j)
{
    unsigned long long int peso = 0;
    int k;
    peso = (m_i_pop[i_geraativa][j][0]->dado != inicio.dado) ? rand() % m_f_popaval[i_geraativa][j] + 1 : peso;
    for(k = 0; k < _final; k++){
        if(dis(m_i_pop[i_geraativa][j][k], m_i_pop[i_geraativa][j][k + 1]) != 1)
            peso += rand() % m_f_popaval[i_geraativa][j] + 1;
    }
    return peso;
}


void ordenar_cromo(unsigned long long int v[][TAMPOP])
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

void bests_cromo()
{
    int i, j, i_g, j_c, best = m_f_popaval[0][0];
    for(i = 0; i < QTGERA; i++){
        for(j = 0; j < TAMCROMO; j++){
            printf("G.%d.%d Nota= %d\n", i + 1, j + 1, m_f_popaval[i][j]);
            if(m_f_popaval[i][j] < best) {
                    best = m_f_popaval[i][j];
                    i_g = i;
                    j_c = j;
            }
        }
        printf("\n");
    }
    printf("\nBest Cromo Nota= %d, nota[%d][%d]\n", best, i_g, j_c);
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
    return NULL;
}

void roleta()
{
    int k;
    for(k = 0; k < TAMPOP; k++) {
        fx_roleta[k].porc = m_f_popaval[i_geraativa][k] / soma_pesos;;
        fx_roleta[k].inf = k == 0 ? 0 : fx_roleta[k - 1].sup ;
        fx_roleta[k].sup = fx_roleta[k].inf + (fx_roleta[k].porc);
        fx_roleta[k].p = indice_notas[(TAMPOP - 1) - k];
        //printf("dado=[%d]\n", fx_roleta[i].p->dado);
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
        printf("porc= %g, lim_inf= %g, lim_sup= %g\n\n", fx_roleta[i].porc, fx_roleta[i].inf, fx_roleta[i].sup);
    }
}


void teste_gera()
{
    printf("\n\n***TESTA COMO FICOU***\n\n");
    printf("\ntotal lf = %llu\n", soma_pesos);
    int j, k, i;
    for(j = 0; j < TAMPOP; j++) {
        pos_cromo(indice_notas[j], i_geraativa, &i);
        for(k = 0; k < TAMCROMO; k++) {
            posicao * pos = m_i_pop[i_geraativa][i][k];
            printf("%d.%d [%d]\n", i + 1, k + 1, pos->dado);
        }
        printf("Total= %d, pct = %g\n\n", m_f_popaval[i_geraativa][j], (double)m_f_popaval[i_geraativa][j] / soma_pesos);
    }
}

void debug(posicao p1, posicao p2)
{
    printf("[%d(%d, %d)], [%d(%d, %d)], dist = %d", p1.dado, p1.linha, p1.col, p2.dado, p2.linha, p2.col, dis(&p1, &p2));
}

posicao** selecionapais()
{
    int j, k;
    double n = rand() / (double)RAND_MAX;
    for(int i = 0; i < TAMPOP; i++) {
        if(n > fx_roleta[i].inf && n <= fx_roleta[i].sup) {
            #ifdef INSTALL_DEBUG
            printf("TESTE SLC PAIS\n\nnRandon = %d\n", n);
            pos_cromo(fx_roleta[i].p, 0, &j);
            for(k = 0; k < TAMCROMO; k++) {
                posicao * pos = m_i_pop[0][j][k];
                printf("%d.%d [%d]\n", j + 1, k + 1, pos->dado);
            }
            //exit(3);
            #endif
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
        do
        {
            while(!((i_pai1_ = selecionapais()) != NULL));//go do
            while(!((i_pai2_ = selecionapais()) != NULL));//go do

            do
            {
                if(i_pai2_ == i_pai1_)
                    while(!((i_pai2_ = selecionapais()) != NULL));//go do
                else {
                        break;
                }

            }while(!(i_pai2_ != i_pai1_));
            pos_cromo(i_pai1_, i_geraativa - 1, &j1);//go do
            pos_cromo(i_pai2_, i_geraativa - 1, &j2);// go do

            if(i_pai1_ == i_pai2_){

                printf("\nBUUUUUG SAFAAADO IGUAL\n");
                printf("\nBUUUUUG SAFAAADO IGUAL, dado1= %d, dado2= %d\n", (*i_pai1_)->dado, (*i_pai2_)->dado);
            }
            if(i_pai1_ == NULL || i_pai2_ == NULL ){
                    printf("\nBUUUUUG SAFAAADO NULL");
                    printf("\nBUUUUUG SAFAAADO NULL, dado1= %d, dado2= %d\n", (*i_pai1_)->dado, (*i_pai2_)->dado);
            }
        //debug_pais(i_pai1, i_pai2);// go to pegar bug
        }while(!cruzapais(i_pai1_, i_pai2_, &j1, &j2));
		mutapais(_i_novapop);
		mutapais(_i_novapop + 1);
		//debug_muta(i_pai1);
		_i_novapop+=2;
	}
	//mostrapop();
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

void debug_pais(posicao** p1, posicao** p2)
{
    printf("\n***DEBUG PAIS***\n\n");
    int k, j, l;
    pos_cromo(p1, 0, &j);
    for(l = 0; l < 2; l++) {
        for(k = 0; k < TAMCROMO; k++) {
            posicao *pos = m_i_pop[0][j][k];
            printf("%d.%d [%d]\n", j + 1, k + 1, pos->dado);
        }
        printf("\n\n");
        pos_cromo(p2, 0, &j);
    }
}

bool cruzapais(posicao** pai_1, posicao** pai_2, int* j1, int* j2) {
	int static id_cruz = 0;
	if(((double)rand() / RAND_MAX)<=TAXACRUZ) {
        int j_pai, k, l_mae;
        int pt_corte_1, pt_corte_2;
        pt_corte_1 = rand() % (TAMCROMO); //go to/ rand + 1.:BUG
        pt_corte_2 = pt_corte_1 + rand() % (TAMCROMO  - pt_corte_1);
        //printf("pt_corte 1= %d, pt_cprte 2 =%d\n", pt_corte_1, pt_corte_2);
        pos_cromo(pai_1, i_geraativa - 1, &j_pai);//go to
        pos_cromo(pai_2, i_geraativa - 1, &l_mae);//go to
        for(k = 0; k < TAMCROMO; k++) {
            if(k >= pt_corte_1 && k < pt_corte_2){
                m_i_pop[i_geraativa][id_cruz + 1][k] = m_i_pop[i_geraativa - 1][j_pai][k]; //filho mae
                m_i_pop[i_geraativa][id_cruz][k] = m_i_pop[i_geraativa - 1][l_mae][k]; //filho pai
            }
            else {
                m_i_pop[i_geraativa][id_cruz][k] = m_i_pop[i_geraativa - 1][j_pai][k];
                m_i_pop[i_geraativa][id_cruz + 1][k] = m_i_pop[i_geraativa - 1][l_mae][k];
            }
        }
        //debug_cruzamento(id_cruz, id_cruz + 1);
        verifica_repeticoes(j_pai, id_cruz, pt_corte_1, pt_corte_2);
        verifica_repeticoes(l_mae, id_cruz + 1, pt_corte_1, pt_corte_2);
        *j1 = id_cruz;
        *j2 = id_cruz + 1;
        //debug_cruzamento(id_cruz, id_cruz + 1);
        #ifdef INSTALL_DEBUG

        teste_unit_cromo(&m_i_pop[i_geraativa - 1][j_pai][0], &m_i_pop[i_geraativa - 1][l_mae][0],
                         &m_i_pop[i_geraativa][id_cruz][0], &m_i_pop[i_geraativa][id_cruz + 1][0],
                         pt_corte_1);
        #endif // INSTALL_DEBUG
        id_cruz  += 2;
        id_cruz = (id_cruz == TAMPOP) ? 0 : id_cruz;

        return true;
    }
    return false;

}

void verifica_repeticoes(int j_pai, int j_filho, int pt_corte_1, int pt_corte_2)
{
    int k, k1, w = 0;
    bool cked_1, cked_2;
    int jpai = j_pai, jfilho = j_filho;
    do{
        cked_1 = true;
        cked_2 = true;
        for(k = TAMCROMO - 1; k >= pt_corte_2; k--){
            for(k1 = pt_corte_1; k1 < pt_corte_2; k1++){
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

void teste_unit_cromo(posicao** p1, posicao** p2, posicao** f1, posicao** f2, int corte)
{
    int j1, j2, j3, j4, k;
    pos_cromo(p1, i_geraativa - 1, &j1); //pai
    pos_cromo(p2, i_geraativa - 1, &j2); //mae
    pos_cromo(f1, i_geraativa, &j3); //filho_pai
    pos_cromo(f2, i_geraativa, &j4); //filho_mae
    for(k = 0; k < TAMCROMO; k++) {
       if(k < corte) {
            posicao * pai = m_i_pop[i_geraativa - 1][j1][k];
            posicao * filho_pai = m_i_pop[i_geraativa][j3][k];
            posicao * mae = m_i_pop[i_geraativa - 1][j2][k];
            posicao * filho_mae = m_i_pop[i_geraativa][j4][k];
            if(!(pai->dado == filho_pai->dado))
            {
                exit(555);
            }
            if(!(mae->dado == filho_mae->dado))
            {
                    exit(555);
            }
        }
        else {
            posicao * pai = m_i_pop[i_geraativa - 1][j1][k];
            posicao * filho_pai = m_i_pop[i_geraativa][j3][k];
            posicao * mae = m_i_pop[i_geraativa - 1][j2][k];
            posicao * filho_mae = m_i_pop[i_geraativa][j4][k];
            if(!(pai->dado == filho_mae->dado)){
                    exit(555);
            }
            if(!(mae->dado == filho_pai->dado)) {
                exit(555);
            }
        }
    }
}
//final 15
void debug_cruzamento(int j, int l)
{
    printf("\n\n***DEBUG CRUZAMENTO***\n\n");
    int k;
    for(k = 0; k < TAMCROMO; k++){
        printf("G.%d, %d.%d dado= %d\n", i_geraativa, j + 1, k + 1, m_i_pop[i_geraativa][j][k]->dado);
    }
    printf("\n");
    for(k = 0; k < TAMCROMO; k++){
        printf("G.%d, %d.%d dado= %d\n", i_geraativa, l + 1, k + 1, m_i_pop[i_geraativa][l][k]->dado);
    }
}

void debug_muta(posicao** p1)
{
    printf("\n\nDEBUGA MUTACAO\n\n");
    int j, k;
    pos_cromo(p1, i_geraativa - 1, &j);
    for(k = 0; k < TAMCROMO; k++){
        printf("G.%d, %d.%d dado= %d\n", i_geraativa, j + 1, k + 1, m_i_pop[i_geraativa][j][k]->dado);
    }
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

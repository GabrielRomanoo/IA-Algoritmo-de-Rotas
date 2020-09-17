/*
 * AlgoritmoGenetico.c
 *
 * Programa que simula um algoritmo genético.
 *
 * Felipe Ferreira Nascimento (Ciência da Computação)
 * Gabriel Romano Godoi Pereira (Ciência da Computação)
 * Jaime Mathias de Lara Bueno (Ciência da Computação)
 * Marcus Vinicius de Souza Olimpio da Silva (Ciência da Computação)
 * Willy Pestana Filho (Ciência da Computação)
 *
 * Disciplina: Inteligência Artificial II
 *
 * Professor: Marcio Luiz Piva
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "rotas.h"

int main(void)
{
srand(time(NULL));
	init_mapa();
	print_mapa();
	criapop();
	mostrapop();

	while(TRUE) {
		avaliapop();
		reproduzpop();
		//mostrapop();
		if(checaparada()) break;
	}
	mostrapop();
    //bests_cromo();
	return 0;
}

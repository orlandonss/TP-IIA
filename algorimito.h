#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "funcao.h" // Necessário para reconhecer as estruturas p_dados e p_solucao

/* * --- Algoritmos de Pesquisa Local --- 
 */

// Algoritmo Trepa Colinas (Hill Climbing)
// Recebe os dados, um ponteiro para guardar a melhor solução encontrada e o nº de iterações
void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes);


/* * --- Futuros Algoritmos (Para implementar nas próximas etapas) --- 
 */

// void algoritmo_evolutivo(p_dados *d, p_solucao *melhor_global, ...);
// void algoritmo_hibrido(p_dados *d, p_solucao *melhor_global, ...);

#endif
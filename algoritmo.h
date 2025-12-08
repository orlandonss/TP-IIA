#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "funcao.h"

// --- ALGORITMOS ---

// Trepa Colinas (Hill Climbing)
// vizinhanca: 1=Troca Simples, 2=Troca Dupla
void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes, int vizinhanca);

// Algoritmo Evolutivo
// sel: 1=Torneio, 2=Roleta
// cross: 1=Uniao Aleatoria, 2=Preserva Comum
// mut: 1=Troca, 2=Reconstrucao
void algoritmo_evolutivo(p_dados *d, p_solucao *melhor_global, int num_iteracoes, int sel, int cross, int mut);

// Hibrido
// tipo: 1=Sequencial (EA -> HC), 2=Memetico (EA com HC interno)
void algoritmo_hibrido(p_dados *d, p_solucao *melhor_global, int num_iteracoes, int tipo);

// Auxiliares expostas (para uso no Hibrido)
void vizinho_troca1(p_solucao *atual, p_solucao *vizinho, p_dados *d);

#endif
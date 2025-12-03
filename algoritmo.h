#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "funcao.h"

// Parâmetros para as threads
typedef struct
{
    int id;
    p_dados *d;
    p_solucao *solucao_final; // Onde guardar o resultado desta thread
    int num_iteracoes;
    int algoritmo_escolhido; // 1-Trepa Colinas, 2-Evolutivo, 3-Hibrido
} t_params;

// Algoritmos
void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes);
void algoritmo_evolutivo(p_dados *d, p_solucao *melhor_global, int num_iteracoes);
void algoritmo_hibrido(p_dados *d, p_solucao *melhor_global, int num_iteracoes);

// Auxiliares
void gerar_vizinho(p_solucao *atual, p_solucao *vizinho, p_dados *d);

#endif
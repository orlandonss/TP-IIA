#ifndef FUNCAO_H
#define FUNCAO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_C 500 // Maximo de candidaturas

// Estrutura para armazenar os dados do problema
typedef struct
{
    int num_candidaturas;             // C
    int num_locais;                   // m
    double matriz_dist[MAX_C][MAX_C]; // Matriz de adjacencias
} p_dados;

// Estrutura para a solucao
typedef struct
{
    int vetor[MAX_C]; // Array de tamanho m com os indices escolhidos
    double custo;     // Valor da Distancia Media
} p_solucao;

// Prototipos
void carregar_dados(char *nome_ficheiro, p_dados *d);
void gerar_solucao_inicial(p_solucao *s, p_dados *d);
double calcular_fitness(p_solucao *s, p_dados *d);
void mostrar_solucao(p_solucao *s, p_dados *d);
void escrever_solucao(p_solucao *s, p_dados *d); // Nova funcao para log
int solucao_contem(p_solucao *s, int id_local, int m);
void copiar_solucao(p_solucao *dest, p_solucao *src, p_dados *d);

#endif
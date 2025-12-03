#include "funcao.h"
#include "algoritmo.h"
#include <stdlib.h>
#include <stdio.h>

#define TAM_POPULACAO 20
#define TAXA_MUTACAO 0.3

// --- FUNÇÕES AUXILIARES ---

// Gera um vizinho: Troca um elemento selecionado por um nao selecionado
void gerar_vizinho(p_solucao *atual, p_solucao *vizinho, p_dados *d)
{
    *vizinho = *atual;
    // Remove um aleatório
    int pos_sair = rand() % d->num_locais;
    // Escolhe um para entrar que não esteja na solução
    int id_entrar;
    do
    {
        id_entrar = rand() % d->num_candidaturas;
    } while (solucao_contem(vizinho, id_entrar, d->num_locais));

    vizinho->vetor[pos_sair] = id_entrar;
    vizinho->custo = calcular_fitness(vizinho, d);
}

// Torneio Binário para Seleção
void torneio(p_solucao *pop, p_solucao *pai, int tam_pop)
{
    int a = rand() % tam_pop;
    int b = rand() % tam_pop;
    if (pop[a].custo > pop[b].custo)
        *pai = pop[a];
    else
        *pai = pop[b];
}

// --- ALGORITMOS ---

// 1. Trepa Colinas (Hill Climbing)
void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes)
{
    p_solucao atual, vizinho;
    gerar_solucao_inicial(&atual, d);
    *melhor_global = atual;

    for (int i = 0; i < num_iteracoes; i++)
    {
        gerar_vizinho(&atual, &vizinho, d);
        if (vizinho.custo >= atual.custo)
        {
            atual = vizinho;
            if (atual.custo > melhor_global->custo)
                *melhor_global = atual;
        }
    }
}

// 2. Algoritmo Evolutivo (Básico)
void algoritmo_evolutivo(p_dados *d, p_solucao *melhor_global, int num_iteracoes)
{
    p_solucao populacao[TAM_POPULACAO];
    p_solucao nova_geracao[TAM_POPULACAO];

    // Inicializa população
    for (int i = 0; i < TAM_POPULACAO; i++)
    {
        gerar_solucao_inicial(&populacao[i], d);
    }

    // Define o melhor inicial
    *melhor_global = populacao[0];
    for (int i = 1; i < TAM_POPULACAO; i++)
    {
        if (populacao[i].custo > melhor_global->custo)
            *melhor_global = populacao[i];
    }

    // Ciclo de gerações
    for (int it = 0; it < num_iteracoes; it++)
    {
        // Elitismo: Mantém o melhor
        nova_geracao[0] = *melhor_global;

        for (int i = 1; i < TAM_POPULACAO; i++)
        {
            p_solucao pai;
            torneio(populacao, &pai, TAM_POPULACAO);

            // Mutação (Neste problema, mutação = gerar vizinho/troca)
            // Como é difícil fazer crossover mantendo m elementos únicos sem reparação complexa,
            // usaremos uma abordagem focada em mutação forte para este exemplo simples.
            if (((double)rand() / RAND_MAX) < TAXA_MUTACAO)
            {
                gerar_vizinho(&pai, &nova_geracao[i], d);
            }
            else
            {
                nova_geracao[i] = pai;
            }
        }

        // Atualiza população e melhor global
        for (int i = 0; i < TAM_POPULACAO; i++)
        {
            populacao[i] = nova_geracao[i];
            if (populacao[i].custo > melhor_global->custo)
                *melhor_global = populacao[i];
        }
    }
}

// 3. Algoritmo Híbrido (Evolutivo + Pesquisa Local no final)
void algoritmo_hibrido(p_dados *d, p_solucao *melhor_global, int num_iteracoes)
{
    // Divide iterações: 90% para Evolutivo, 10% para refinamento Local
    int it_evol = num_iteracoes * 0.9;
    int it_local = num_iteracoes - it_evol;

    // Fase 1: Exploração Global
    algoritmo_evolutivo(d, melhor_global, it_evol);

    // Fase 2: Intensificação (Refina a melhor solução encontrada)
    // Usamos o Trepa Colinas começando da melhor solução do EA
    p_solucao refinada;
    // Nota: O trepa colinas original gera aleatório no inicio.
    // Vamos adaptar ligeiramente a logica chamando a logica interna aqui:

    p_solucao atual = *melhor_global, vizinho;

    for (int i = 0; i < it_local; i++)
    {
        gerar_vizinho(&atual, &vizinho, d);
        if (vizinho.custo >= atual.custo)
        {
            atual = vizinho;
            if (atual.custo > melhor_global->custo)
                *melhor_global = atual;
        }
    }
}
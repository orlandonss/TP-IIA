#include "funcao.h"
#include "algoritmo.h"
#include <stdlib.h>
#include <stdio.h>

#define TAM_POPULACAO 100
#define TAXA_MUTACAO 0.01
#define PROB_CROSSOVER 0.7
// --- FUNÇÕES AUXILIARES DE VIZINHANÇA ---

void vizinho_troca1(p_solucao *atual, p_solucao *vizinho, p_dados *d)
{
    copiar_solucao(vizinho, atual, d);
    int pos_sair = rand() % d->num_locais;
    int id_entrar;
    do
    {
        id_entrar = rand() % d->num_candidaturas;
    } while (solucao_contem(vizinho, id_entrar, d->num_locais));
    vizinho->vetor[pos_sair] = id_entrar;
    vizinho->custo = calcular_fitness(vizinho, d);
}

void vizinho_troca2(p_solucao *atual, p_solucao *vizinho, p_dados *d)
{
    copiar_solucao(vizinho, atual, d);
    int pos1 = rand() % d->num_locais;
    int cand1;
    do
    {
        cand1 = rand() % d->num_candidaturas;
    } while (solucao_contem(vizinho, cand1, d->num_locais));
    vizinho->vetor[pos1] = cand1;

    int pos2 = rand() % d->num_locais;
    while (pos1 == pos2)
        pos2 = rand() % d->num_locais;

    int cand2;
    do
    {
        cand2 = rand() % d->num_candidaturas;
    } while (solucao_contem(vizinho, cand2, d->num_locais));
    vizinho->vetor[pos2] = cand2;

    vizinho->custo = calcular_fitness(vizinho, d);
}

// --- SELEÇÃO ---

void torneio(p_solucao *pop, p_solucao *pai)
{
    int a = rand() % TAM_POPULACAO;
    int b = rand() % TAM_POPULACAO;
    if (pop[a].custo > pop[b].custo)
        *pai = pop[a];
    else
        *pai = pop[b];
}

void roleta(p_solucao *pop, p_solucao *pai)
{
    double soma = 0.0;
    for (int i = 0; i < TAM_POPULACAO; i++)
        soma += pop[i].custo;
    double r = ((double)rand() / RAND_MAX) * soma;
    double p = 0.0;
    *pai = pop[TAM_POPULACAO - 1];
    for (int i = 0; i < TAM_POPULACAO; i++)
    {
        p += pop[i].custo;
        if (p >= r)
        {
            *pai = pop[i];
            return;
        }
    }
}

// --- CROSSOVER ---

int uniao_vetores(int *v1, int *v2, int m, int *temp)
{
    int count = 0;
    for (int i = 0; i < m; i++)
        temp[count++] = v1[i];
    for (int i = 0; i < m; i++)
    {
        int existe = 0;
        for (int j = 0; j < count; j++)
        {
            if (temp[j] == v2[i])
            {
                existe = 1;
                break;
            }
        }
        if (!existe)
            temp[count++] = v2[i];
    }
    return count;
}

// Crossover 1: União Aleatória (Junta tudo e escolhe m aleatorios)
void crossover_uniao(p_solucao *p1, p_solucao *p2, p_solucao *filho, p_dados *d)
{
    int temp[MAX_C * 2]; // Para segurar a uniao
    int tamanho_uniao = uniao_vetores(p1->vetor, p2->vetor, d->num_locais, temp);

    // Embaralha temp
    for (int i = 0; i < tamanho_uniao; i++)
    {
        int r = rand() % tamanho_uniao;
        int aux = temp[i];
        temp[i] = temp[r];
        temp[r] = aux;
    }

    // Pega os primeiros m
    for (int i = 0; i < d->num_locais; i++)
        filho->vetor[i] = temp[i];

    filho->custo = calcular_fitness(filho, d);
}

// Crossover 2: Preserva Comum (Mantem os iguais, preenche o resto aleatorio da uniao)
void crossover_comum(p_solucao *p1, p_solucao *p2, p_solucao *filho, p_dados *d)
{
    int selecionados = 0;
    int temp_uniao[MAX_C * 2];
    int count_u = 0;

    // 1. Identificar comuns e adicionar ao filho
    for (int i = 0; i < d->num_locais; i++)
    {
        for (int j = 0; j < d->num_locais; j++)
        {
            if (p1->vetor[i] == p2->vetor[j])
            {
                filho->vetor[selecionados++] = p1->vetor[i];
                break;
            }
        }
    }

    // 2. Criar lista de candidatos (Uniao - Comuns) para preencher o resto
    count_u = uniao_vetores(p1->vetor, p2->vetor, d->num_locais, temp_uniao);

    // 3. Preencher o resto

    while (selecionados < d->num_locais)
    {
        int idx_temp = rand() % count_u;
        int cand = temp_uniao[idx_temp];
        // Verifica se ja esta no filho (ja foi adicionado como comum)
        int existe = 0;
        for (int k = 0; k < selecionados; k++)
        {
            if (filho->vetor[k] == cand)
            {
                existe = 1;
                break;
            }
        }
        if (!existe)
        {
            filho->vetor[selecionados++] = cand;
        }
    }
    filho->custo = calcular_fitness(filho, d);
}

// --- ALGORITMOS ---

void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes, int vizinhanca)
{
    p_solucao atual, vizinho;
    gerar_solucao_inicial(&atual, d);
    copiar_solucao(melhor_global, &atual, d);

    for (int i = 0; i < num_iteracoes; i++)
    {
        if (vizinhanca == 1)
            vizinho_troca1(&atual, &vizinho, d);
        else
            vizinho_troca2(&atual, &vizinho, d);

        if (vizinho.custo >= atual.custo)
        {
            copiar_solucao(&atual, &vizinho, d);
            if (atual.custo > melhor_global->custo)
                copiar_solucao(melhor_global, &atual, d);
        }

        // Log compacto
        if (i % 50 == 0)
        {
            printf("HC It %4d | Melhor: %.4f\n", i, melhor_global->custo);
        }
    }
}

void algoritmo_evolutivo(p_dados *d, p_solucao *melhor_global, int num_iteracoes, int sel, int cross, int mut)
{
    p_solucao populacao[TAM_POPULACAO];
    p_solucao nova_geracao[TAM_POPULACAO];

    // Inicializa
    for (int i = 0; i < TAM_POPULACAO; i++)
        gerar_solucao_inicial(&populacao[i], d);

    *melhor_global = populacao[0];
    for (int i = 1; i < TAM_POPULACAO; i++)
        if (populacao[i].custo > melhor_global->custo)
            *melhor_global = populacao[i];

    for (int it = 0; it < num_iteracoes; it++)
    {
        nova_geracao[0] = *melhor_global; // Elitismo (ocupa índice 0)

        // Ciclo avança de 2 em 2 para gerar pares de filhos
        for (int i = 1; i < TAM_POPULACAO; i += 2)
        {
            p_solucao pai1, pai2, filho1, filho2;

            // 1. Seleção (2 Pais)
            if (sel == 1)
            {
                torneio(populacao, &pai1);
                torneio(populacao, &pai2);
            }
            else
            {
                roleta(populacao, &pai1);
                roleta(populacao, &pai2);
            }

            // 2. Crossover (Gera 2 Filhos)
            if (((double)rand() / RAND_MAX) < PROB_CROSSOVER)
            {
                if (cross == 1)
                {
                    crossover_uniao(&pai1, &pai2, &filho1, d);
                    crossover_uniao(&pai1, &pai2, &filho2, d); // 2º filho (aleatoriedade diferente)
                }
                else
                {
                    crossover_comum(&pai1, &pai2, &filho1, d);
                    crossover_comum(&pai1, &pai2, &filho2, d);
                }
            }
            else
            {
                // Sem crossover, os filhos são clones dos pais
                filho1 = pai1;
                filho2 = pai2;
            }

            // 3. Mutação (Independente para cada filho)
            // Filho 1
            if (((double)rand() / RAND_MAX) < TAXA_MUTACAO)
            {
                p_solucao m;
                if (mut == 1)
                    vizinho_troca1(&filho1, &m, d);
                else
                    vizinho_troca2(&filho1, &m, d);
                filho1 = m;
            }

            // Filho 2
            if (((double)rand() / RAND_MAX) < TAXA_MUTACAO)
            {
                p_solucao m;
                if (mut == 1)
                    vizinho_troca1(&filho2, &m, d);
                else
                    vizinho_troca2(&filho2, &m, d);
                filho2 = m;
            }

            // 4. Inserção na nova geração
            nova_geracao[i] = filho1;

            // VERIFICAÇÃO DE SEGURANÇA: Garante que não escreve fora do array
            // Se i=99 e TAM=100, i+1=100 (invalido). Esta verificação impede o crash.
            if (i + 1 < TAM_POPULACAO)
                nova_geracao[i + 1] = filho2;
        }

        // Atualiza população
        for (int i = 0; i < TAM_POPULACAO; i++)
        {
            populacao[i] = nova_geracao[i];
            if (populacao[i].custo > melhor_global->custo)
                *melhor_global = populacao[i];
        }

        // Log periódico
        if (it % 50 == 0)
            printf("EA Gen %d | Melhor: %.4f\n", it, melhor_global->custo);
    }
}

// Implementação das DUAS abordagens híbridas
void algoritmo_hibrido(p_dados *d, p_solucao *melhor_global, int num_iteracoes, int tipo)
{
    if (tipo == 1)
    {
        // --- HIBRIDO 1: SEQUENCIAL (EA + Refinamento HC) ---
        // Usa 90% das iterações para Evolutivo e 10% para Trepa Colinas na melhor solução
        printf(">>> Hibrido 1: Evolutivo -> Refinamento Final (HC)\n");

        int it_ea = (int)(num_iteracoes * 0.9);
        int it_hc = num_iteracoes - it_ea;

        // 1. Fase Global (Evolutivo)
        algoritmo_evolutivo(d, melhor_global, it_ea, 1, 1, 1);
        printf("Fim Fase Evolutiva. Melhor: %.4f. Iniciando Refinamento...\n", melhor_global->custo);

        // 2. Fase Local (Trepa Colinas apenas na melhor solução)
        p_solucao atual = *melhor_global;
        p_solucao vizinho;

        for (int i = 0; i < it_hc; i++)
        {
            vizinho_troca1(&atual, &vizinho, d); // Usa vizinhança simples para ajuste fino
            if (vizinho.custo > atual.custo)
            {
                atual = vizinho;
            }
        }

        // Se a refinada for melhor, atualiza
        if (atual.custo > melhor_global->custo)
            *melhor_global = atual;
    }
    else
    {
        // --- HIBRIDO 2: MEMÉTICO (EA com Pesquisa Local intervalada) ---
        // A cada 20 gerações, aplica HC em alguns indivíduos da população
        printf(">>> Hibrido 2: Memetico (HC dentro do Ciclo Evolutivo)\n");

        p_solucao populacao[TAM_POPULACAO];
        p_solucao nova_geracao[TAM_POPULACAO];

        // Inicializa
        for (int i = 0; i < TAM_POPULACAO; i++)
            gerar_solucao_inicial(&populacao[i], d);
        *melhor_global = populacao[0];

        for (int it = 0; it < num_iteracoes; it++)
        {
            // --- Passo Normal do Evolutivo ---
            nova_geracao[0] = *melhor_global; // Elitismo
            for (int i = 1; i < TAM_POPULACAO; i++)
            {
                p_solucao pai1, pai2, filho;
                torneio(populacao, &pai1);
                torneio(populacao, &pai2);

                if (((double)rand() / RAND_MAX) < PROB_CROSSOVER)
                    crossover_uniao(&pai1, &pai2, &filho, d);
                else
                    filho = pai1;

                if (((double)rand() / RAND_MAX) < TAXA_MUTACAO)
                {
                    p_solucao m;
                    vizinho_troca1(&filho, &m, d);
                    filho = m;
                }
                nova_geracao[i] = filho;
            }

            // --- PASSO EXTRA HIBRIDO: Otimização Local (Lamarckian) ---
            // A cada 20 gerações, melhora 5 individuos aleatorios com Trepa Colinas
            if (it > 0 && it % 20 == 0)
            {
                // printf(" [Hibrido] A aplicar otimizacao local na geracao %d...\n", it);
                for (int k = 0; k < 5; k++)
                {
                    int idx = rand() % TAM_POPULACAO; // Escolhe aleatorio
                    p_solucao *indiv = &nova_geracao[idx];
                    p_solucao v;
                    // Faz 50 passos de HC neste individuo
                    for (int step = 0; step < 50; step++)
                    {
                        vizinho_troca1(indiv, &v, d);
                        if (v.custo > indiv->custo)
                            *indiv = v;
                    }
                }
            }

            // Atualiza População
            for (int i = 0; i < TAM_POPULACAO; i++)
            {
                populacao[i] = nova_geracao[i];
                if (populacao[i].custo > melhor_global->custo)
                    *melhor_global = populacao[i];
            }

            if (it % 50 == 0)
                printf("Hyb2 Gen %3d | Melhor: %.4f\n", it, melhor_global->custo);
        }
    }
}
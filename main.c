#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // Necessário para threads
#include <time.h>
#include "funcao.h"
#include "algoritmo.h"

#define NUM_THREADS 4 // Número de threads para execução paralela

// Função executada por cada thread
void *rotina_thread(void *arg)
{
    t_params *p = (t_params *)arg;

    // Seed diferente para cada thread baseada no ID e tempo
    unsigned int seed = time(NULL) + p->id * 100;
    srand(seed);

    switch (p->algoritmo_escolhido)
    {
    case 1:
        trepa_colinas(p->d, p->solucao_final, p->num_iteracoes);
        break;
    case 2:
        algoritmo_evolutivo(p->d, p->solucao_final, p->num_iteracoes);
        break;
    case 3:
        algoritmo_hibrido(p->d, p->solucao_final, p->num_iteracoes);
        break;
    default:
        trepa_colinas(p->d, p->solucao_final, p->num_iteracoes);
    }
    pthread_exit(NULL);
}

// Função para escolher a melhor solução entre todas as threads
void obter_melhor_resultado(p_solucao lista_solucoes[], int qtd, p_solucao *melhor_de_todas)
{
    *melhor_de_todas = lista_solucoes[0];
    for (int i = 1; i < qtd; i++)
    {
        if (lista_solucoes[i].custo > melhor_de_todas->custo)
        {
            *melhor_de_todas = lista_solucoes[i];
        }
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    p_dados dados;
    p_solucao melhor_solucao;
    char nome_ficheiro[100];
    int iteracoes = 1000;

    printf("\n--- Otimizacao Turistica ---\n");

    // 1. Seleção do Ficheiro via ARGV ou Default
    if (argc >= 2)
    {
        strcpy(nome_ficheiro, argv[1]);
    }
    else
    {
        printf("Ficheiro nao especificado nos argumentos.\n");
        printf("A usar 'tourism_5.txt' por defeito.\n");
        printf("Uso: ./main <nome_ficheiro>\n");
        strcpy(nome_ficheiro, "tourism_5.txt");
    }

    carregar_dados(nome_ficheiro, &dados);
    printf("Dados carregados: %d Candidaturas | %d Locais a selecionar\n",
           dados.num_candidaturas, dados.num_locais);

    // 2. Menu de Seleção
    int opcao_alg = 0;
    int modo_exec = 0;

    printf("\nEscolha o Algoritmo:\n");
    printf("1 - Trepa Colinas (Hill Climbing)\n");
    printf("2 - Algoritmo Evolutivo\n");
    printf("3 - Hibrido\n");
    printf("Opcao: ");
    if (scanf("%d", &opcao_alg) != 1)
        opcao_alg = 1;

    printf("\nModo de Execucao:\n");
    printf("1 - Simples (1 execucao na main thread)\n");
    printf("2 - Multithreaded (%d execucoes paralelas - Mais Rapido)\n", NUM_THREADS);
    printf("Opcao: ");
    if (scanf("%d", &modo_exec) != 1)
        modo_exec = 1;

    printf("\nNumero de Iteracoes (ex: 1000): ");
    scanf("%d", &iteracoes);

    // 3. Execução
    if (modo_exec == 1)
    {
        // Modo Simples
        printf("\nA executar modo simples...\n");
        if (opcao_alg == 1)
            trepa_colinas(&dados, &melhor_solucao, iteracoes);
        else if (opcao_alg == 2)
            algoritmo_evolutivo(&dados, &melhor_solucao, iteracoes);
        else
            algoritmo_hibrido(&dados, &melhor_solucao, iteracoes);
    }
    else
    {
        // Modo Multithread
        printf("\nA iniciar %d threads...\n", NUM_THREADS);
        pthread_t threads[NUM_THREADS];
        t_params params[NUM_THREADS];
        p_solucao solucoes_threads[NUM_THREADS];

        for (int i = 0; i < NUM_THREADS; i++)
        {
            params[i].id = i;
            params[i].d = &dados;
            params[i].num_iteracoes = iteracoes;
            params[i].algoritmo_escolhido = opcao_alg;
            params[i].solucao_final = &solucoes_threads[i];

            if (pthread_create(&threads[i], NULL, rotina_thread, (void *)&params[i]) != 0)
            {
                perror("Erro ao criar thread");
                exit(1);
            }
        }

        // Aguarda threads terminarem
        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
            printf("Thread %d terminou com custo: %.4f\n", i, solucoes_threads[i].custo);
        }

        // 4. Escolhe o Melhor Valor
        obter_melhor_resultado(solucoes_threads, NUM_THREADS, &melhor_solucao);
        printf("\nMelhor solucao entre as threads selecionada!\n");
    }

    printf("\n--- Resultado Final ---\n");
    mostrar_solucao(&melhor_solucao);

    return 0;
}
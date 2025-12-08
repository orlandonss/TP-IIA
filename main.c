#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcao.h"
#include "algoritmo.h"

#define NUM_RUNS 10

int main(int argc, char *argv[])
{
    srand(time(NULL));
    p_dados dados;
    p_solucao melhor_run, melhor_absoluta;
    char nome_ficheiro[100];
    int iteracoes = 1000;
    int runs = NUM_RUNS;
    double mbf = 0.0;
    int run_onde_ocorreu_melhor = -1;
    int opcao = 0;

    melhor_absoluta.custo = -1.0;

    printf("\n--- Otimizacao Turistica ---\n");

    if (argc >= 2)
        strcpy(nome_ficheiro, argv[1]);
    else
        strcpy(nome_ficheiro, "tourism_5.txt");

    carregar_dados(nome_ficheiro, &dados);
    printf("Dados carregados: %d Candidaturas, %d Locais\n", dados.num_candidaturas, dados.num_locais);

    printf("\nEscolha o Algoritmo:\n");
    printf("1 - Trepa Colinas (Vizinhanca 1)\n");
    printf("2 - Trepa Colinas (Vizinhanca 2)\n");
    printf("3 - Evolutivo (Torneio + Uniao)\n");
    printf("4 - Evolutivo (Roleta + Comum)\n");
    printf("5 - Hibrido 1 (Sequencial: EA -> HC)\n");
    printf("6 - Hibrido 2 (Memetico: HC durante EA)\n");
    printf("Opcao: ");
    if (scanf("%d", &opcao) != 1)
        opcao = 3;

    printf("Numero de Iteracoes: ");
    scanf("%d", &iteracoes);

    printf("Numero de Repeticoes (Runs): ");
    char buf[10];
    scanf("%s", buf);
    runs = atoi(buf);
    if (runs <= 0)
        runs = 10;

    printf("\nIniciando %d runs...\n", runs);

    for (int r = 0; r < runs; r++)
    {
        printf("\n--- Run %d ---\n", r + 1);

        switch (opcao)
        {
        case 1:
            trepa_colinas(&dados, &melhor_run, iteracoes, 1);
            break;
        case 2:
            trepa_colinas(&dados, &melhor_run, iteracoes, 2);
            break;
        case 3:
            algoritmo_evolutivo(&dados, &melhor_run, iteracoes, 1, 1, 1);
            break;
        case 4:
            algoritmo_evolutivo(&dados, &melhor_run, iteracoes, 2, 2, 2);
            break;
        case 5:
            algoritmo_hibrido(&dados, &melhor_run, iteracoes, 1);
            break;
        case 6:
            algoritmo_hibrido(&dados, &melhor_run, iteracoes, 2);
            break;
        default:
            algoritmo_evolutivo(&dados, &melhor_run, iteracoes, 1, 1, 1);
        }

        printf("FIM Run %d: ", r + 1);
        mostrar_solucao(&melhor_run, &dados);

        mbf += melhor_run.custo;

        if (melhor_run.custo > melhor_absoluta.custo)
        {
            copiar_solucao(&melhor_absoluta, &melhor_run, &dados);
            run_onde_ocorreu_melhor = r + 1;
        }
    }

    printf("\n=========================================\n");
    printf("ESTATISTICAS FINAIS (%d Runs)\n", runs);
    printf("=========================================\n");
    printf("MBF: %.4f\n", mbf / runs);
    printf("MELHOR GLOBAL (Run %d):\n", run_onde_ocorreu_melhor);
    mostrar_solucao(&melhor_absoluta, &dados);
    printf("=========================================\n");

    return 0;
}
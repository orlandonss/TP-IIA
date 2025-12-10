#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcao.h"
#include "algoritmo.h"

// Variável global para controlar logs
int modo_silencioso = 0;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    // CORREÇÃO DE MEMÓRIA (Mantida)
    static p_dados dados;

    p_solucao melhor_run, melhor_absoluta;
    char nome_ficheiro[100];
    int iteracoes = 1000;
    int runs = 10;
    int opcao = 1;
    double mbf = 0.0;

    melhor_absoluta.custo = -1.0;

    // --- MODO BATCH ---
    if (argc == 5)
    {
        modo_silencioso = 1;
        strcpy(nome_ficheiro, argv[1]);
        opcao = atoi(argv[2]);
        iteracoes = atoi(argv[3]);
        runs = atoi(argv[4]);
        carregar_dados(nome_ficheiro, &dados);
    }
    else
    {
        // --- MODO INTERATIVO ---
        printf("\n--- Otimizacao Turistica ---\n");

        // Leitura do Ficheiro
        if (argc >= 2)
            strcpy(nome_ficheiro, argv[1]);
        else
        {
            printf("Nome do Ficheiro (enter para 'tourism_5.txt'): ");
            char buffer[100];
            if (fgets(buffer, 100, stdin))
            {
                buffer[strcspn(buffer, "\n")] = 0;
                if (strlen(buffer) == 0)
                    strcpy(nome_ficheiro, "tourism_5.txt");
                else
                    strcpy(nome_ficheiro, buffer);
            }
        }

        carregar_dados(nome_ficheiro, &dados);
        printf("Dados carregados: %d Candidaturas | %d Locais\n",
               dados.num_candidaturas, dados.num_locais);

        // Menu Visual
        printf("\nEscolha o Algoritmo:\n");
        printf("1 - Trepa Colinas (Vizinhanca 1: Troca Simples)\n");
        printf("2 - Trepa Colinas (Vizinhanca 2: Troca Dupla)\n");
        printf("3 - Evolutivo (Torneio + Crossover Uniao)\n");
        printf("4 - Evolutivo (Roleta + Crossover Comum)\n");
        printf("5 - Hibrido 1 (Sequencial: Evolutivo -> Trepa Colinas)\n");
        printf("6 - Hibrido 2 (Memetico: Trepa Colinas durante Evolucao)\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1)
            opcao = 3;

        printf("\nNumero de Iteracoes: ");
        scanf("%d", &iteracoes);
        printf("Numero de Repeticoes (Runs): ");
        scanf("%d", &runs);
    }

    if (!modo_silencioso)
        printf("\nIniciando %d runs de %d iteracoes...\n", runs, iteracoes);

    // --- CICLO DE EXECUÇÃO ---
    for (int r = 0; r < runs; r++)
    {
        if (!modo_silencioso)
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

        if (!modo_silencioso)
            printf("FIM Run %d: %.4f\n", r + 1, melhor_run.custo);

        mbf += melhor_run.custo;

        if (melhor_run.custo > melhor_absoluta.custo)
        {
            copiar_solucao(&melhor_absoluta, &melhor_run, &dados);
        }
    }

    double mbf_final = mbf / runs;

    printf("\n=========================================\n");
    printf("MBF: %.4f | Melhor Global: %.4f\n", mbf_final, melhor_absoluta.custo);
    printf("Solucao Global: ");
    mostrar_solucao(&melhor_absoluta, &dados);
    printf("=========================================\n");

    // --- GRAVAR NO CSV (COM NOMES EXPLÍCITOS) ---
    FILE *f_out = fopen("resumo_resultados.csv", "a");
    if (f_out)
    {
        char nome_alg[50];

        // Mapeamento dos IDs para Nomes Reais
        switch (opcao)
        {
        case 1:
            strcpy(nome_alg, "TrepaColinas_Swap1");
            break;
        case 2:
            strcpy(nome_alg, "TrepaColinas_Swap2");
            break;
        case 3:
            strcpy(nome_alg, "Evolutivo_Torneio");
            break;
        case 4:
            strcpy(nome_alg, "Evolutivo_Roleta");
            break;
        case 5:
            strcpy(nome_alg, "Hibrido_Sequencial");
            break;
        case 6:
            strcpy(nome_alg, "Hibrido_Memetico");
            break;
        default:
            strcpy(nome_alg, "Desconhecido");
        }

        // Cria a string descritiva final: Ex: "Evolutivo_Torneio_1000_Iter"
        char desc_completa[100];
        sprintf(desc_completa, "%s_%d_Iter", nome_alg, iteracoes);

        fprintf(f_out, "%s;%s;%.4f;%.4f\n",
                nome_ficheiro, desc_completa, mbf_final, melhor_absoluta.custo);

        fclose(f_out);
    }

    return 0;
}
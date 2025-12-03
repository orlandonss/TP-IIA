#include "funcao.h"
#include "algorimito.h"

// Deve criar um algoritmo.h com o prototipo:
// void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes);

int main()
{
    srand(time(NULL));
    p_dados dados;
    p_solucao melhor_solucao;

    // Define ficheiro a carregar (ex: grafo_20.txt que enviaste)
    char *ficheiro = "tourism_5.txt";

    printf("--- Trabalho Pratico IA: Otimizacao Turistica ---\n");
    printf("A carregar %s...\n", ficheiro);
    carregar_dados(ficheiro, &dados);

    printf("Candidaturas (C): %d | A selecionar (m): %d\n",
           dados.num_candidaturas, dados.num_locais);

    // Teste Simples: 1 Execução do Trepa Colinas
    printf("\nIniciando Trepa Colinas (1000 iteracoes)...\n");

    trepa_colinas(&dados, &melhor_solucao, 1000);

    printf("\n--- Resultado Final ---\n");
    mostrar_solucao(&melhor_solucao);

    // Exemplo de como farias o estudo experimental (varias runs)
    printf("\n--- Estudo Experimental (10 runs) ---\n");
    double soma = 0;
    for (int i = 0; i < 10; i++)
    {
        p_solucao s;
        trepa_colinas(&dados, &s, 1000);
        printf("Run %2d: %.4f\n", i + 1, s.custo);
        soma += s.custo;
    }
    printf("Media: %.4f\n", soma / 10.0);

    return 0;
}
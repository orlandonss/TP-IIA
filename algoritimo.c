#include "funcao.h"
#include "algorimito.h"

// Gera um vizinho: Troca um elemento selecionado por um nao selecionado
void gerar_vizinho(p_solucao *atual, p_solucao *vizinho, p_dados *d)
{
    // 1. Copia a solucao atual
    *vizinho = *atual;

    // 2. Escolhe uma posicao aleatoria no vetor solucao para remover (0 a m-1)
    int pos_sair = rand() % d->num_locais;
    int id_antigo = vizinho->vetor[pos_sair];

    // 3. Escolhe um candidato que NAO esteja na solucao para entrar
    int id_entrar;
    do
    {
        id_entrar = rand() % d->num_candidaturas;
    } while (solucao_contem(vizinho, id_entrar, d->num_locais));

    // 4. Realiza a troca
    vizinho->vetor[pos_sair] = id_entrar;

    // 5. Avalia o novo vizinho
    vizinho->custo = calcular_fitness(vizinho, d);
}

// Algoritmo Trepa Colinas (Maximizaçao)
void trepa_colinas(p_dados *d, p_solucao *melhor_global, int num_iteracoes)
{
    p_solucao atual, vizinho;

    // Gera solucao inicial aleatoria
    gerar_solucao_inicial(&atual, d);
    *melhor_global = atual;

    for (int i = 0; i < num_iteracoes; i++)
    {
        gerar_vizinho(&atual, &vizinho, d);

        // Como queremos MAXIMIZAR a distancia media:
        // Se o vizinho for melhor ou igual, aceitamos (para caminhar em plateaus)
        if (vizinho.custo >= atual.custo)
        {
            atual = vizinho;

            // Atualiza o melhor global encontrado
            if (atual.custo > melhor_global->custo)
            {
                *melhor_global = atual;
                // printf("Iter %d: Nova melhor solucao: %.4f\n", i, melhor_global->custo);
            }
        }
    }
}
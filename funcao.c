#include "funcao.h"

void carregar_dados(char *nome_ficheiro, p_dados *d)
{
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f)
    {
        printf("Erro ao abrir ficheiro: %s\n", nome_ficheiro);
        exit(1);
    }

    if (fscanf(f, "%d %d", &d->num_candidaturas, &d->num_locais) != 2)
    {
        fscanf(f, "%*[^\n]");
        fscanf(f, "%d %d", &d->num_candidaturas, &d->num_locais);
    }

    // Inicializar matriz a 0
    for (int i = 0; i < MAX_C; i++)
        for (int j = 0; j < MAX_C; j++)
            d->matriz_dist[i][j] = 0.0;

    char buf1[20], buf2[20];
    double dist;

    // Leitura das distancias
    while (fscanf(f, "%s %s %lf", buf1, buf2, &dist) == 3)
    {
        // Converte "e1" para índice 0, "e2" para 1...
        int i = (buf1[0] == 'e' || buf1[0] == 'E') ? atoi(buf1 + 1) - 1 : atoi(buf1) - 1;
        int j = (buf2[0] == 'e' || buf2[0] == 'E') ? atoi(buf2 + 1) - 1 : atoi(buf2) - 1;

        if (i >= 0 && i < d->num_candidaturas && j >= 0 && j < d->num_candidaturas)
        {
            d->matriz_dist[i][j] = dist;
            d->matriz_dist[j][i] = dist;
        }
    }
    fclose(f);
}

int solucao_contem(p_solucao *s, int id_local, int m)
{
    for (int i = 0; i < m; i++)
        if (s->vetor[i] == id_local)
            return 1;
    return 0;
}

void gerar_solucao_inicial(p_solucao *s, p_dados *d)
{
    int selecionados = 0;
    for (int i = 0; i < d->num_locais; i++)
        s->vetor[i] = -1;

    while (selecionados < d->num_locais)
    {
        int r = rand() % d->num_candidaturas;
        int existe = 0;
        for (int k = 0; k < selecionados; k++)
        {
            if (s->vetor[k] == r)
            {
                existe = 1;
                break;
            }
        }

        if (!existe)
        {
            s->vetor[selecionados] = r;
            selecionados++;
        }
    }
    s->custo = calcular_fitness(s, d);
}

double calcular_fitness(p_solucao *s, p_dados *d)
{
    double soma = 0.0;
    int m = d->num_locais;

    if (m < 2)
        return 0.0;

    for (int i = 0; i < m - 1; i++)
    {
        for (int j = i + 1; j < m; j++)
        {
            int p1 = s->vetor[i];
            int p2 = s->vetor[j];
            soma += d->matriz_dist[p1][p2];
        }
    }
    return (1.0 / m) * soma;
}

// Mostra a solução final formatada: {e1, e2, e4} (sem ordenar)
void mostrar_solucao(p_solucao *s, p_dados *d)
{
    printf("Fitness: %.4f | Solucao: {", s->custo);
    for (int i = 0; i < d->num_locais; i++)
    {
        // Adiciona o prefixo 'e' e soma 1 ao índice
        printf("e%d", s->vetor[i] + 1);

        // Coloca vírgula se não for o último
        if (i < d->num_locais - 1)
            printf(", ");
    }
    printf("}\n");
}

// Log compacto também formatado
void escrever_solucao(p_solucao *s, p_dados *d)
{
    printf("[Fit: %.2f] {", s->custo);

    int max_print = (d->num_locais > 10) ? 10 : d->num_locais;

    for (int i = 0; i < max_print; i++)
    {
        printf("e%d", s->vetor[i] + 1);
        if (i < max_print - 1)
            printf(", ");
    }

    if (d->num_locais > max_print)
        printf(", ...");
    printf("}");
}

void copiar_solucao(p_solucao *dest, p_solucao *src, p_dados *d)
{
    dest->custo = src->custo;
    for (int i = 0; i < d->num_locais; i++)
        dest->vetor[i] = src->vetor[i];
}
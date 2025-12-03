#include "funcao.h"

void carregar_dados(char *nome_ficheiro, p_dados *d)
{
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f)
    {
        printf("Erro ao abrir ficheiro: %s\n", nome_ficheiro);
        exit(1);
    }

    // Leitura da primeira linha: C e m [cite: 40]
    if (fscanf(f, "%d %d", &d->num_candidaturas, &d->num_locais) != 2)
    {
        // Tenta ler formato alternativo caso o ficheiro tenha cabecalho extra
        fscanf(f, "%*[^\n]");
        fscanf(f, "%d %d", &d->num_candidaturas, &d->num_locais);
    }

    // Inicializar matriz a 0
    for (int i = 0; i < MAX_C; i++)
        for (int j = 0; j < MAX_C; j++)
            d->matriz_dist[i][j] = 0.0;

    char buf1[20], buf2[20];
    double dist;

    // Leitura das distancias (ex: e1 e2 50.2)
    while (fscanf(f, "%s %s %lf", buf1, buf2, &dist) == 3)
    {
        // Converte "e1" para indice 0, "e2" para 1, etc.
        // Assume-se que o formato comeÃ§a sempre com 'e' ou sao inteiros
        int i, j;
        if (buf1[0] == 'e' || buf1[0] == 'E')
            i = atoi(buf1 + 1) - 1;
        else
            i = atoi(buf1) - 1;

        if (buf2[0] == 'e' || buf2[0] == 'E')
            j = atoi(buf2 + 1) - 1;
        else
            j = atoi(buf2) - 1;

        if (i >= 0 && i < d->num_candidaturas && j >= 0 && j < d->num_candidaturas)
        {
            d->matriz_dist[i][j] = dist;
            d->matriz_dist[j][i] = dist; // Matriz simetrica
        }
    }
    fclose(f);
}

// Verifica se um ID ja esta no vetor solucao
int solucao_contem(p_solucao *s, int id_local, int m)
{
    for (int i = 0; i < m; i++)
    {
        if (s->vetor[i] == id_local)
            return 1;
    }
    return 0;
}

void gerar_solucao_inicial(p_solucao *s, p_dados *d)
{
    // Escolhe m locais aleatorios e unicos
    int selecionados = 0;
    while (selecionados < d->num_locais)
    {
        int r = rand() % d->num_candidaturas;
        if (!solucao_contem(s, r, selecionados))
        {
            s->vetor[selecionados] = r;
            selecionados++;
        }
    }
    s->custo = calcular_fitness(s, d);
}

// Formula: DM(P) = (1/m) * Sum(Sum(dist(ei, ej)))
double calcular_fitness(p_solucao *s, p_dados *d)
{
    double soma = 0.0;
    int m = d->num_locais;

    for (int i = 0; i < m - 1; i++)
    {
        for (int j = i + 1; j < m; j++)
        {
            // Soma a distancia entre o ponto na posicao i e o ponto na posicao j do vetor solucao
            int p1 = s->vetor[i];
            int p2 = s->vetor[j];
            soma += d->matriz_dist[p1][p2];
        }
    }
    return (1.0 / m) * soma;
}

void mostrar_solucao(p_solucao *s)
{
    printf("Fitness: %.4f | Locais: ", s->custo);
    // Mostra apenas os primeiros 10 para nao encher o ecra em instancias grandes
    for (int i = 0; i < 5 && i < MAX_C; i++)
    {
        printf("%d ", s->vetor[i] + 1); // +1 para bater certo com e1, e2...
    }
    printf("...\n");
}
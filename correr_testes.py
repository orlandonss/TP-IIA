import subprocess
import os
import sys

# CONFIGURAÇÃO
# Se estiver no Windows e compilou como 'main.exe', mantenha assim.
# Se for Linux/Mac, use "./main"
EXECUTAVEL = "main.exe" if os.name == 'nt' else "./main"
ARQUIVO_SAIDA = "resumo_resultados.csv"

# LISTA DE FICHEIROS PARA TESTE
ficheiros = ["tourism_5.txt","tourism_20.txt","tourism_50.txt","tourism_100.txt","tourism_250.txt", "tourism_500.txt"] 
# (Adicione os outros ficheiros aqui: tourism_5.txt, tourism_20.txt, etc)

# LISTA DE ALGORITMOS (1 a 6)
algoritmos = [1, 2, 3, 4, 5, 6]

# LISTA DE ITERAÇÕES
iteracoes_lista = [100, 1000, 5000, 50000] 

# NÚMERO DE RUNS
RUNS = 10

def main():
    # Verifica se o executável existe
    if not os.path.exists(EXECUTAVEL) and not os.path.exists(EXECUTAVEL.strip("./")):
        print(f"ERRO: Executável '{EXECUTAVEL}' não encontrado!")
        print("Compile o código C primeiro: gcc -o main main.c funcao.c algoritmo.c")
        input("Pressione ENTER para sair...")
        return

    # Inicializa CSV com cabeçalho simples
    print(f"A preparar {ARQUIVO_SAIDA}...")
    try:
        with open(ARQUIVO_SAIDA, "w") as f:
            f.write("Ficheiro;Configuracao;MBF;Melhor_Run\n")
    except PermissionError:
        print(f"ERRO: O ficheiro '{ARQUIVO_SAIDA}' está aberto no Excel!")
        print("Feche o Excel e tente novamente.")
        input("Pressione ENTER para sair...")
        return

    total = len(ficheiros) * len(algoritmos) * len(iteracoes_lista)
    count = 0

    print(f"--- INICIANDO BENCHMARK ({total} testes) ---")

    for fich in ficheiros:
        # Verifica se o ficheiro de texto existe
        if not os.path.exists(fich):
            print(f"AVISO: Ficheiro de dados '{fich}' não encontrado na pasta.")
            continue
            
        for alg in algoritmos:
            for it in iteracoes_lista:
                count += 1
                print(f"[{count}/{total}] A processar: {fich} | Alg {alg} | Iter {it}...")
                
                # Chama o programa C
                cmd = [EXECUTAVEL, fich, str(alg), str(it), str(RUNS)]
                
                # Executa e captura erros
                retorno = subprocess.run(cmd)
                
                if retorno.returncode != 0:
                    print(f"ERRO FATAL: O programa C crashou ou retornou erro (Código {retorno.returncode}).")
                    print("Verifique se aplicou o 'static p_dados dados' no main.c!")
                    input("Pressione ENTER para abortar...")
                    return

    print(f"\n--- CONCLUÍDO! ---")
    print(f"Os resultados foram gravados em '{ARQUIVO_SAIDA}'.")
    input("Pressione ENTER para fechar esta janela...")

if __name__ == "__main__":
    main()
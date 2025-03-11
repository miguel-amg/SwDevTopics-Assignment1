import os
import sys
import pandas as pd

def calcular_media_colunas(caminho_csv, input_dir, output_dir):
    try:
        df = pd.read_csv(caminho_csv)
        
        # Se a coluna 'Package' existir, remove os valores máximo e mínimo
        if 'Package' in df.columns:
            max_package = df['Package'].max()
            min_package = df['Package'].min()
            removidos = df[(df['Package'] == max_package) | (df['Package'] == min_package)]
            df = df[(df['Package'] != max_package) & (df['Package'] != min_package)]
            print(f"Linhas removidas em {os.path.basename(caminho_csv)}:")
            print(removidos)
        
        medias = df.mean(numeric_only=True)
        
        # Obtém o caminho relativo do arquivo em relação ao diretório de entrada
        rel_dir = os.path.dirname(os.path.relpath(caminho_csv, input_dir))
        
        # Cria a mesma estrutura de pastas dentro do diretório 'medians'
        output_subdir = os.path.join(output_dir, rel_dir)
        if not os.path.exists(output_subdir):
            os.makedirs(output_subdir)
        
        # Define o nome do arquivo de saída
        base_name = os.path.basename(caminho_csv)
        out_name = base_name.replace(".csv", "_median.csv")
        out_path = os.path.join(output_subdir, out_name)
        
        medias.to_csv(out_path, header=["Média"])
        print(f"Médias guardadas em: {out_path}\n")
    except Exception as e:
        print(f"Erro ao processar {caminho_csv}: {e}")

def main(input_dir):
    # Diretório de saída: 'medians' dentro do diretório de entrada
    output_dir = os.path.join(input_dir, "medians")
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Percorre recursivamente todos os arquivos
    for root, dirs, files in os.walk(input_dir):
        # Ignora o diretório de saída para evitar processar arquivos já gerados
        if os.path.abspath(root).startswith(os.path.abspath(output_dir)):
            continue
        for file in files:
            if file.endswith(".csv"):
                caminho_csv = os.path.join(root, file)
                calcular_media_colunas(caminho_csv, input_dir, output_dir)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python script.py <diretorio_de_entrada>")
        sys.exit(1)
    input_dir = sys.argv[1]
    main(input_dir)

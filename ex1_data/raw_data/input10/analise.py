import pandas as pd

def calcular_media_colunas(nome_ficheiro):
    try:
        # Carregar o ficheiro CSV
        df = pd.read_csv(nome_ficheiro)
        
        # Verificar se a coluna 'Package' existe
        if 'Package' in df.columns:
            # Encontrar os valores máximo e mínimo da coluna 'Package'
            max_package = df['Package'].max()
            min_package = df['Package'].min()
            
            # Filtrar as linhas removendo o máximo e o mínimo
            removidos = df[(df['Package'] == max_package) | (df['Package'] == min_package)]
            df = df[(df['Package'] != max_package) & (df['Package'] != min_package)]
            
            # Print das linhas removidas
            print("Linhas removidas:")
            print(removidos)
        
        # Calcular a média de todas as colunas numéricas
        medias = df.mean(numeric_only=True)
        
        # Criar nome do ficheiro de saída
        nome_saida = nome_ficheiro.replace(".csv", "_median.csv")
        
        # Guardar as médias num novo ficheiro CSV
        medias.to_csv(nome_saida, header=["Média"])
        
        print(f"Médias das colunas guardadas em {nome_saida}")
    except Exception as e:
        print(f"Erro ao processar o ficheiro: {e}")

# Exemplo de uso
if __name__ == "__main__":
    nome_ficheiro = "linearO2.csv"  # Substituir pelo nome correto do ficheiro
    calcular_media_colunas(nome_ficheiro)

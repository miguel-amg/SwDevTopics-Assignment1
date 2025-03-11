import os
import sys
import pandas as pd

def extract_metrics_from_csv(file_path):
    try:
        df = pd.read_csv(file_path)
        col_metric = df.columns[0]
        col_value = df.columns[1]
        
        pkg_vals = df.loc[df[col_metric] == "Package", col_value].values
        time_vals = df.loc[df[col_metric] == "Time", col_value].values
        
        if len(pkg_vals) == 0 or len(time_vals) == 0:
            return None
        return float(pkg_vals[0]), float(time_vals[0])
    except Exception as e:
        print(f"Erro ao processar {file_path}: {e}")
        return None

def main(input_dir, output_csv):
    records = []
    
    for root, dirs, files in os.walk(input_dir):
        for file in files:
            if file.endswith(".csv"):
                file_path = os.path.join(root, file)
                metrics = extract_metrics_from_csv(file_path)
                if metrics is not None:
                    package_val, time_val = metrics
                    records.append({
                        "File": file,
                        "Package": package_val,
                        "Time": time_val
                    })
    
    if records:
        df_out = pd.DataFrame(records)
        # Ordena pelo nome do arquivo
        df_out.sort_values(by="File", inplace=True)
        df_out.to_csv(output_csv, index=False)
        print(f"Resultados salvos em: {output_csv}")
    else:
        print("Nenhum arquivo CSV válido foi encontrado.")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso: python extract_metrics.py <diretorio_de_entrada> <arquivo_saida.csv>")
        sys.exit(1)
    
    input_directory = sys.argv[1]
    output_file = sys.argv[2]
    main(input_directory, output_file)

import sys
import pandas as pd

def transform_filename(filename):
    # Exemplo: Shell_IPD_SO_median.csv -> Shell Int Pequeno Desorganizado Sem Otimizacoes
    parts = filename.split('_')
    if len(parts) < 3:
        return filename
    algoritmo = parts[0]
    tos = parts[1]
    opt = parts[2]
    tipo = "Int" if tos[0].upper() == "I" else "Float" if tos[0].upper() == "F" else tos[0]
    tamanho = "Pequeno" if tos[1].upper() == "P" else "Grande" if tos[1].upper() == "G" else tos[1]
    ordem = "Organizado" if tos[2].upper() == "O" else "Desorganizado" if tos[2].upper() == "D" else tos[2]
    opt_str = "Otimizado" if opt.upper() == "O2" else "Sem Otimizacoes" if opt.upper() == "SO" else opt
    return f"{algoritmo} {tipo} {tamanho} {ordem} {opt_str}"

def process_csv(input_csv, output_csv):
    # Lê o CSV sem cabeçalho e define nomes
    df = pd.read_csv(input_csv, header=None, names=["Filename", "Package", "Time"])
    # Transforma o nome e ordena pelo Package
    df["Filename"] = df["Filename"].apply(transform_filename)
    df.sort_values(by="Package", inplace=True)
    df.to_csv(output_csv, index=False)
    print(f"Arquivo criado: {output_csv}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso: python transform_names.py <input_csv> <output_csv>")
        sys.exit(1)
    process_csv(sys.argv[1], sys.argv[2])

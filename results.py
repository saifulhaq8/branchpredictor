import os
import numpy as np
import pandas as pd
import re
import sys


#string = "cadical-low-60K-1186B.champsimtrace.xz-filename-no-no-no-no-lru-1core.txt"

folder = 'results_10M'

def get_row_name(x):
    split_string = x.split(".")
    filename = split_string[0]
    return filename

def get_column_name(x): 
        try:
            return x.split(".")[2][3:-22]
        except:
            print(x)

rows = [get_row_name(f) for f in os.listdir(folder)]
rows = list(set(rows))

columns = [get_column_name(f) for f in os.listdir(folder)]
columns = list(set(columns))


result_dir = 'results'

try:
    os.mkdir(result_dir)
except:
    pass

df_mpki = pd.DataFrame(np.random.randn(len(rows),len(columns)), index=rows, columns=columns)
df_accuracy = pd.DataFrame(np.random.randn(len(rows),len(columns)), index=rows, columns=columns)

for file in os.listdir(folder):
    f = open(os.path.join(folder,file),'r')
    row = get_row_name(file)
    column = get_column_name(file)
    text = f.read()
    try:
        df_mpki.loc[row,column] = float(re.compile(r'MPKI: \d+\.?\d*').search(text).group().split()[-1])
        df_accuracy.loc[row,column] = float(re.compile("Branch Prediction Accuracy: \d+\.?\d*").search(text).group().split()[-1])
    except:
        print(file)

df_mpki.to_csv(os.path.join(result_dir, "mpki.csv"))
df_accuracy.to_csv(os.path.join(result_dir, "accuracy.csv"))

print(df_mpki)
print(df_accuracy)

# for file in os.listdir('result_10M'):
#     f = open(os.path.join(folder,file),'r')
#     file_name = file[:10]
#     text = f.read()
#     df_mpki.loc[file_name,folder] = float(re.compile(r'MPKI: \d+\.?\d*').search(text).group().split()[-1])
#     df_accuracy.loc[file_name,folder] = float(re.compile("Branch Prediction Accuracy: \d+\.?\d*").search(text).group().split()[-1])
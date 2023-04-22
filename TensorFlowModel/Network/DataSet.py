import pandas as pd
import os


interval = 3 # how much row will be skipped
merge_row_number = 3 # how much data will be in one sample

dir = os.getcwd() + "\\Data\\"
file_list = os.listdir(dir)

print(file_list)

def prepareAllFiles(interval: int, merge_row_number: int):
    merged = pd.DataFrame()
    for x in file_list:
        to_merged = prepareFileData(x, merge_row_number, interval)
        merged = pd.concat([merged, to_merged])
    merged = merged.sample(frac=1)
    merged.to_csv("AllData.csv", index=False)

def prepareFileData(file: str , merge_row_number: int, interval: int) -> pd.DataFrame:
    df = pd.read_csv(dir + file, on_bad_lines='skip')
    to_merge = pd.DataFrame()
    for i in range (0,interval):
        to_merge = pd.concat([to_merge,df[i::interval]])

    if(to_merge.values.shape[0]%merge_row_number!=0):
        to_merge = to_merge[0 : len(to_merge)-to_merge.values.shape[0]%merge_row_number]
       
    merged_array = to_merge.values.reshape(int(to_merge.values.size/(merge_row_number * 7)), merge_row_number * 7)
    merged = pd.DataFrame(merged_array, columns=prepColumns(merge_row_number))
    merged = merged.loc[:,~merged.columns.duplicated()].copy()

    return merged

def prepColumns(merge_row_number):
    columns = []
    for i in range(0, merge_row_number):
        columns.append(f"A{i*3+1}")
        columns.append(f"A{i*3+2}")
        columns.append(f"A{i*3+3}")
        columns.append(f"G{i*3+1}")
        columns.append(f"G{i*3+2}")
        columns.append(f"G{i*3+3}")
        columns.append(f"Activity")
    return columns

prepareAllFiles(interval, merge_row_number)
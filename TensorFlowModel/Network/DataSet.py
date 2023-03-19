import pandas as pd
import os

dir = os.getcwd() + "\\Data\\"

file_list = os.listdir(dir)

def prepareAllFiles():
    merged = pd.DataFrame()
    for x in file_list:
        to_merged = prepareFileData(x, 3)
        merged = pd.concat([merged, to_merged])
    merged.to_csv("AllData.csv", index=False)

def prepareFileData(file: str , merge_row_number: int) -> pd.DataFrame:
    df = pd.read_csv(dir + file)
    to_merge = pd.DataFrame()
    for i in range (0,merge_row_number):
        to_merge = pd.concat([to_merge,df[i::merge_row_number]])

    if(to_merge.values.shape[0]%3!=0):
        to_merge = to_merge[0 : len(to_merge)-to_merge.values.shape[0]%merge_row_number]
       
    merged_array = to_merge.values.reshape(int(to_merge.values.size/(merge_row_number * 7)), merge_row_number * 7)
    merged = pd.DataFrame(merged_array, columns=['A1', 'A2', 'A3', 'G1', 'G2', 'G3', 'Activity', 'A4', 'A5', 'A6', 'G4', 'G5', 'G6', 'Activity', 'A7', 'A8', 'A9', 'G7', 'G8', 'G9', 'Activity'])
    merged = merged.loc[:,~merged.columns.duplicated()].copy()

    return merged

prepareAllFiles()
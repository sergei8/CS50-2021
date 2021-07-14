import sys
import csv
import re


def get_params():
    """get and return files names from args"""
    if len(sys.argv) != 3:
        return None
    else:
        return (sys.argv[1], sys.argv[2])


def read_database(file_name):
    """read file and return STR counts"""
    try:
        with open(f"./databases/{file_name}") as f:
            return list(csv.DictReader(f))
    except FileNotFoundError:
        return None


def read_dna_file(file_name):
    """read DNA sequences into memory"""
    try:
        with open(f"./databases/{file_name}") as f:
            return f.read()
    except FileNotFoundError:
        return None

def probe_STR(STR, sample):
    """count and return number of repeated `str` in the `sample`"""
    x = '.'.join(sample.split(STR)) 
    y = re.split("[A-Z]", x) 
    return len(max(y))
    
def main():
    """identifies a person based on their DNA"""
    
    # get params from CLI
    args = get_params()
    if args == None:
        print ("Usage: python dna.py data.csv sequence.txt")
        return
    else:
        data_file_name, seq_file_name = args
        
    # read data file into dict `data`
    data = read_database(data_file_name)
    if data == None:
        print (f"File {data_file_name} not found")
        return
    
    # read sequences file into memory
    seq = read_dna_file(seq_file_name)
    if seq == None:
        print (f"File {seq_file_name} not found")
        return

    
    
    # find similarity
    for STR_record in data:
        pass
     
    
if __name__ == "__main__":
    main()

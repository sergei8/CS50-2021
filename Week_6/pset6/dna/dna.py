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
        with open(file_name) as f:
            return list(csv.DictReader(f))
    except FileNotFoundError:
        return None


def read_dna_file(file_name):
    """read DNA sequences into memory"""
    try:
        with open(file_name) as f:
            return f.read()
    except FileNotFoundError:
        return None


def probe_STR(STR, sample):
    """count and return number of repeated `str` in the `sample`"""
    x = '.'.join(sample.split(STR))
    y = re.split("[A-Z]", x)
    return len(max(y))


def build_DNA_model(STR_names, sample):
    """build abd return dict with key=STR_names, values=longest count of the STR"""
    model = {str_name: probe_STR(str_name, sample) for str_name in STR_names}
    return model


def convert_to_int(data):
    """convert counts of STR onto numbers"""
    converted_list = []
    for dict_item in data:
        converted_list.append(
            {x: int(y) if y.isnumeric() else y for (x, y) in dict_item.items()})
    return converted_list


def main():
    """identifies a person based on their DNA"""

    # get params from CLI
    args = get_params()
    if args == None:
        print("Usage: python dna.py data.csv sequence.txt")
        return
    else:
        data_file_name, seq_file_name = args

    # read data file into list of dict `data`
    data = read_database(data_file_name)
    if data == None:
        print(f"File {data_file_name} not found")
        return

    # convert numbers to int in `data`
    data = convert_to_int(data)

    # read sequences file into memory
    seq = read_dna_file(seq_file_name)
    if seq == None:
        print(f"File {seq_file_name} not found")
        return

    # set names of STRs without person `name`
    STR_names = list(filter(lambda x: x != "name", data[0].keys()))

    # build DNA model from input sequence
    model = build_DNA_model(STR_names, seq)

    # find sample owner
    for STR_record in data:
        if all(dna in STR_record.items() for dna in model.items()):
            print(STR_record["name"])
            return

    print("No match")


if __name__ == "__main__":
    main()

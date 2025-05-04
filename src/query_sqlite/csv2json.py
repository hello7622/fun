import os
import sys
import csv
import json

def find_csv_files(directory, recursive=True):
    """
    Find all CSV files in the specified directory.
    If recursive is True, searches subdirectories as well.
    """
    csv_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.csv'):
                csv_files.append(os.path.join(root, file))
        if not recursive:
            break
    return csv_files

def csv_to_json(csv_file_path):
    """
    Convert a CSV file to a JSON file in the same directory.
    The JSON file will have the same name as the CSV file but with a .json extension.
    """
    json_file_path = csv_file_path.replace('.csv', '.json')
    try:
        with open(csv_file_path, mode='r', encoding='utf-8') as csv_file:
            reader = csv.DictReader(csv_file)
            rows = list(reader)

        with open(json_file_path, mode='w', encoding='utf-8') as json_file:
            json.dump(rows, json_file, indent=4, ensure_ascii=False)

        print(f"转换成功：{csv_file_path} -> {json_file_path}")
    except Exception as e:
        print(f"文件处理出错 {csv_file_path}: {e}")

def process_directories(directories):
    """
    Process each directory provided, converting all CSV files to JSON files.
    """
    for directory in directories:
        if not os.path.exists(directory):
            print(f"目录不存在：{directory}")
            continue

        if not os.path.isdir(directory):
            print(f"目录无效：{directory}")
            continue

        csv_files = find_csv_files(directory)
        for csv_file in csv_files:
            csv_to_json(csv_file)

if __name__ == "__main__":
    # Accept directories as command-line arguments
    if len(sys.argv) < 2:
        print("使用方法：python csv_to_json_converter.py <directory1> <directory2> ...")
        sys.exit(1)

    directories = sys.argv[1:]
    process_directories(directories)

#!/bin/bash

# 数据库文件
DB_FILE=$1

# 检查数据库文件是否存在
if [ ! -f "$DB_FILE" ]; then
  echo "数据库文件 $DB_FILE 不存在！"
  exit 1
fi

# 创建输出目录
OUTPUT_DIR="${DB_FILE%%.*}"  # 提取数据库名作为目录名
mkdir -p "$OUTPUT_DIR"

# 查询所有表名
TABLES=$(sqlite3 "$DB_FILE" "SELECT name FROM sqlite_master WHERE type='table';")

# 遍历每个表
for TABLE in $TABLES; do
  echo "处理表：$TABLE"

  # 创建表的目录
  TABLE_DIR="$OUTPUT_DIR/$TABLE"
  mkdir -p "$TABLE_DIR"

  # 获取表结构并保存到 table_info 文件
  sqlite3 "$DB_FILE" "PRAGMA table_info($TABLE);" > "$TABLE_DIR/table_info"

  # 获取表数据并保存到 data.csv 文件
  # 获取列名
  COLUMNS=$(sqlite3 "$DB_FILE" "PRAGMA table_info($TABLE);" | awk -F'|' '{print $2}' | paste -sd "," -)
  echo "$COLUMNS" > "$TABLE_DIR/data.csv"  # 写入列名

  # 获取表中的数据
  sqlite3 "$DB_FILE" "SELECT * FROM $TABLE;" | sed 's/|/,/g' >> "$TABLE_DIR/data.csv"  # 写入行数据

  # 调用 Python 脚本将 CSV 转换为 JSON
  if [ -f "csv2json.py" ]; then
    python3 csv2json.py "$TABLE_DIR"  # 调用 Python 脚本
  else
    echo "警告：未找到 csv2json.py 脚本，跳过 CSV 到 JSON 的转换！"
  fi
done

echo "处理完成！数据已导出到 $OUTPUT_DIR 目录中。"
echo "若直接打开data.csv时出现乱码，可以先打开excel，然后依次选择"
echo "数据 → 获取和转换数据 → 从文本/CSV"

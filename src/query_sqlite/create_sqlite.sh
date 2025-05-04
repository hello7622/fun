#!/bin/bash

# 输入的目录名
INPUT_DIR=$1

# 检查输入目录是否存在
if [ ! -d "$INPUT_DIR" ]; then
  echo "目录 $INPUT_DIR 不存在！"
  exit 1
fi

# 根据目录名生成数据库名
DB_NAME="${INPUT_DIR%%/}.db"

# 删除已存在的同名数据库以避免冲突
if [ -f "$DB_NAME" ]; then
  echo "警告：数据库 $DB_NAME 已存在，将被覆盖！"
  rm -f "$DB_NAME"
fi

# 遍历每个子目录（作为表名）
for TABLE_DIR in "$INPUT_DIR"/*; do
  if [ -d "$TABLE_DIR" ]; then
    TABLE_NAME=$(basename "$TABLE_DIR")
    echo "处理表：$TABLE_NAME"

    # 检查 table_info 和 data.csv 是否存在
    TABLE_INFO_FILE="$TABLE_DIR/table_info"
    DATA_FILE="$TABLE_DIR/data.csv"

    if [ ! -f "$TABLE_INFO_FILE" ]; then
      echo "跳过表 $TABLE_NAME：缺少 table_info 文件"
      continue
    fi

    if [ ! -f "$DATA_FILE" ]; then
      echo "跳过表 $TABLE_NAME：缺少 data.csv 文件"
      continue
    fi

    # 从 table_info 文件中生成 CREATE TABLE 语句
    CREATE_TABLE_SQL="CREATE TABLE $TABLE_NAME ("
    while IFS='|' read -r cid name type notnull dflt_value pk; do
      CREATE_TABLE_SQL+="$name $type, "
    done < "$TABLE_INFO_FILE"
    # 移除最后的逗号并补上括号
    CREATE_TABLE_SQL="${CREATE_TABLE_SQL%, })"

    # 执行创建表的 SQL
    sqlite3 "$DB_NAME" "$CREATE_TABLE_SQL"

    # 从 data.csv 中读取列名（第一行）并生成 INSERT 语句
    HEADER=$(head -n 1 "$DATA_FILE")
    INSERT_SQL="INSERT INTO $TABLE_NAME ($HEADER) VALUES "

    # 读取数据行并生成 VALUES 部分
    while IFS=',' read -r -a ROW; do
      VALUES="("
      for VALUE in "${ROW[@]}"; do
        VALUES+="'$VALUE', "
      done
      VALUES="${VALUES%, }), "
      INSERT_SQL+="$VALUES"
    done < <(tail -n +2 "$DATA_FILE") # 跳过第一行（列名）

    # 移除最后的逗号并执行插入
    INSERT_SQL="${INSERT_SQL%, }"
    sqlite3 "$DB_NAME" "$INSERT_SQL"
  fi
done

echo "数据库 $DB_NAME 创建完成！"

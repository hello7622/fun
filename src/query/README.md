# SQLite 数据库导出与转换工具

## 简介

本工具是一个用于从 SQLite 数据库中导出表结构和数据的 Shell 脚本，并配合 Python 脚本将数据从 CSV 格式转换为 JSON 格式。脚本会根据数据库文件生成相应的目录结构，方便对数据库表的数据进行管理和查看。

---

## 目录结构

运行脚本后，会按照以下目录结构生成导出数据：

```
<数据库名>/
  ├── <表名>/
       ├── table_info      # 表结构信息
       ├── data.csv        # 表数据（CSV 格式）
       ├── data.json       # 表数据（JSON 格式，需 Python 脚本支持）
```

---

## 使用说明

### 1. 运行环境

**依赖：**
- SQLite3
- Python 3
- Python 脚本依赖：`csv` 和 `json` 模块（Python 标准库）

**操作系统支持：**
- Linux / macOS
- Windows (需支持 Bash 环境)

---

### 2. 使用方法

#### （1）准备工作

- 将 `csv2json.py` 放置在与 Shell 脚本相同的目录下。
- 确保数据库文件和脚本在同一目录或提供正确的数据库文件路径。

#### （2）运行 Shell 脚本

```bash
bash export_sqlite.sh <数据库文件>
```

**示例：**
```bash
bash export_sqlite.sh my_database.db
```

脚本会在当前目录生成一个与数据库文件同名的目录，包含导出的表数据。

#### （3）运行 Python 脚本（可选）

如果 Shell 脚本运行时未找到 `csv2json.py`，可以单独运行 Python 脚本来转换 CSV 为 JSON：

```bash
python3 csv2json.py <目录1> <目录2> ...
```

**示例：**
```bash
python3 csv2json.py my_database
```

---

## 输出结果说明

### 表结构 (`table_info`)

- 包含表的字段定义及元信息。
- 通过 `PRAGMA table_info(<表名>)` 获取。

### 表数据 (`data.csv`)

- 表的所有数据，以 CSV 格式存储。
- 第一行为列名，数据行以逗号分隔。

### 表数据 (`data.json`)

- 表的所有数据，以 JSON 格式存储。
- 每一行数据为 JSON 对象，整体为 JSON 数组。

---

## 注意事项

1. **数据库文件检查**：脚本会检查数据库文件是否存在，若不存在则停止运行。
2. **输出目录**：若输出目录已存在，脚本会覆盖其中的文件，请谨慎操作。
3. **编码问题**：如直接打开 `data.csv` 出现乱码，建议使用 Excel 按以下步骤导入：
   - 数据 → 获取和转换数据 → 从文本/CSV
4. **Python 脚本缺失**：如果未找到 `csv2json.py`，会跳过 JSON 转换步骤。

---

## 示例输出

运行脚本后，假设数据库文件为 `example.db`，生成的结构可能如下：

```
example/
  ├── users/
  │    ├── table_info
  │    ├── data.csv
  │    ├── data.json
  ├── orders/
  │    ├── table_info
  │    ├── data.csv
  │    ├── data.json
```

---

## 常见问题

### 1. `csv2json.py` 脚本未找到
**解决办法：** 确保 `csv2json.py` 脚本与主 Shell 脚本位于同一目录，或者修改脚本路径调用。

### 2. Python 脚本转换失败
**可能原因：**
- CSV 数据有编码问题。
- CSV 文件格式不规范。

**解决办法：**
- 检查 CSV 文件是否正确生成。
- 确保 Python 环境安装正常。

### 3. 表数据量较大时运行缓慢
**优化建议：**
- 可按需修改脚本，将数据分批导出。
- 使用更高效的数据库管理工具。

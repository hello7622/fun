# sepa.py说明

该项目是一个基于 Python 的图像处理工具，可以对图片进行颜色聚类，并将不同颜色分组分离为独立的图层（图片）。它使用 `K-Means` 算法来对图片的颜色进行聚类。

## 功能

1. **颜色聚类**：将图片中的颜色分为若干类。
2. **图层分离**：根据颜色聚类结果，将每一类颜色分离成单独的图层（图片）。
3. **批量处理**：支持同时处理多张图片。

## 环境要求

- Python >= 3.7
- 安装以下依赖库：
  ```bash
  pip install numpy pillow scikit-learn
  ```

## 使用说明

### 基本命令

运行以下命令以执行图层分离：
```bash
python sepa.py -n <聚类数> -p <图片路径1> <图片路径2> ...
```

### 参数说明

- `-n, --n_cluster`：指定聚类数，即生成的图层（图片）数。默认为 5。
- `-p, --paths`：指定一个或多个图片路径。

### 示例

假设当前目录下有一张图片 `example.png`，运行以下命令进行分离：
```bash
python sepa.py -n 5 -p example.png
```

程序会在同级目录下生成一个名为 `example` 的文件夹，其中包含 5 张分离后的图片：
```
example/
├── example_cluster_1.png
├── example_cluster_2.png
├── example_cluster_3.png
├── example_cluster_4.png
├── example_cluster_5.png
```

如果有多张图片，可以用以下命令：
```bash
python script_name.py -n 5 -p image1.png image2.png image3.png
```

每张图片都会在它的输出目录下生成分离后的图层。

## 工作原理

1. **加载图片**：使用 `Pillow` (`PIL`) 加载图片并转换为 NumPy 数组。
2. **颜色聚类**：使用 `scikit-learn` 的 `KMeans` 对图片的颜色进行聚类，得到颜色中心和对应的像素标签。
3. **图层分离**：将每个聚类结果分离为单独的图片并保存。

## 注意事项

1. **图片格式**：仅支持`png`图片格式。
2. **输出目录**：每张图片的分离结果会保存在与该图片同名的文件夹中。
3. **K-Means 算法**：初始的随机种子可能导致不同运行间结果略有差异。

## 批量处理示例

如果需要处理多张图片，可以一次性传入多个图片路径：
```bash
python script_name.py -n 3 -p img1.png img2.png img3.png
```
程序会为每张图片生成一个对应的输出目录，其中包含分离后的图层。

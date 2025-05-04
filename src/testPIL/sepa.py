import sys
from PIL import Image
import numpy as np
from sklearn.cluster import KMeans
import os, argparse

# 1. 加载图片
def load_image(image_path):
    """加载图片并转换为 NumPy 数组"""
    img = Image.open(image_path)
    img_array = np.array(img)
    return img, img_array

# 2. 颜色聚类
def cluster_colors(img_array, n_clusters=5):
    """对图片的颜色进行聚类"""
    # 将图片数组重塑为 (height * width, channels)
    height, width, channels = img_array.shape
    pixels = img_array.reshape(-1, channels)

    # 使用 K-Means 进行颜色聚类
    kmeans = KMeans(n_clusters=n_clusters, random_state=42)
    labels = kmeans.fit_predict(pixels)

    # 获取聚类中心（代表颜色）
    colors = kmeans.cluster_centers_.astype(int)
    return labels, colors, height, width

# 3. 分离图片
def separate_images(img_array, labels, colors, height, width, output_dir, base_name):
    """根据聚类结果分离图片"""
    separated_images = []
    for i in range(len(colors)):
        # 创建一个空白图像（黑色背景）
        new_image = np.zeros_like(img_array)

        # 将属于当前颜色组的像素保留，其余设为黑色
        mask = labels == i
        new_image[mask.reshape(height, width)] = img_array[mask.reshape(height, width)]

        # 将 NumPy 数组转换回 Pillow 图像
        separated_image = Image.fromarray(new_image)

        # 保存分离后的图片
        output_path = os.path.join(output_dir, f"{base_name}_cluster_{i + 1}.png")
        separated_image.save(output_path)
        separated_images.append(output_path)
    return separated_images

# 4. 处理单张图片
def process_image(image_path, output_dir, n_clusters=5):
    """处理单张图片并返回结果图片路径"""
    # 加载图片
    img, img_array = load_image(image_path)

    # 颜色聚类
    labels, colors, height, width = cluster_colors(img_array, n_clusters)

    # 获取图片的基本名称（不含路径和扩展名）
    base_name = os.path.splitext(os.path.basename(image_path))[0]

    # 分离图片
    result_paths = separate_images(img_array, labels, colors, height, width, output_dir, base_name)

    return result_paths

# 主函数
def main():
    
    # 创建 ArgumentParser 对象
    parser = argparse.ArgumentParser(description="图层分离程序")
    parser.add_argument("-n", "--n_cluster", type=int, default=5, help="聚类数（生成图片数）")
    parser.add_argument("-p", "--paths", nargs="*", help="图片路径")
    args = parser.parse_args()
    if not args.paths:
        parser.print_help()
        sys.exit(1)

    # 遍历所有传入的图片路径
    for image_path in args.paths:
        
        # 输出目录
        output_dir = image_path[:-4]
        os.makedirs(output_dir, exist_ok=True)

        # 处理图片
        result_paths = process_image(image_path, output_dir, args.n_cluster)

        # 打印结果信息
        print(f'"{image_path}" -> {output_dir}/')

# 运行
if __name__ == "__main__":
    main()

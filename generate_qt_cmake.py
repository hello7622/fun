#!/usr/bin/env python3
"""
generate_qt_cmake.py -p=<process name>

生成一个 Qt + CMake 项目模板。
- 必填参数：-p 或 --process，指定项目名（也是生成的目录名）
- 若目录已存在，则在其中补充缺失文件
- 若文件已存在，则跳过
- 完成后输出生成/跳过的文件清单
"""

import argparse
import os
import sys

# ---------- 文件内容模板 ----------

FILES = {
    # 相对路径: 文件内容
    "app/MainWindow.h": '''#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
};
''',

    "app/MainWindow.cpp": '''#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
{
    setWindowTitle(tr("MainWindow"));
    resize(800, 600);
}
''',

    "translations/zh_CN.ts": '''<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1">
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../app/MainWindow.cpp" line="6"/>
        <source>MainWindow</source>
        <translation type="finished">主窗口</translation>
    </message>
</context>
</TS>
''',

    "build.py": '''#!/usr/bin/env python3
import os
import subprocess

# 切换到脚本所在目录
os.chdir(os.path.dirname(os.path.abspath(__file__)))

BUILD_DIR_NAME= "build"
BUILD_TYPE= "Release"

# 第一步：cmake 配置
subprocess.run(
    ["cmake", "-B", BUILD_DIR_NAME, f"-DCMAKE_BUILD_TYPE={BUILD_TYPE}"],
    check=True
)

# 第二步：编译
subprocess.run(
    ["cmake", "--build", BUILD_DIR_NAME],
    check=True
)
''',

    "CMakeLists.txt": '''cmake_minimum_required(VERSION 3.16)

get_filename_component(CURRENT_FOLDER_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${CURRENT_FOLDER_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT DEFINED ENV{LOCAL_QT5_DIR})
    message(FATAL_ERROR "请设置 LOCAL_QT5_DIR\\n例如：export LOCAL_QT5_DIR=\\"/opt/homebrew/opt/qt@5\\"\\n若设置后仍然报错，请删除build目录后重新编译")
endif()
set(CMAKE_PREFIX_PATH "$ENV{LOCAL_QT5_DIR}")

find_package(Qt5 COMPONENTS LinguistTools Widgets REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} 
    main.cpp
    resources.qrc
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/zh_CN.ts
)

qt5_create_translation(QM_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TS_FILES}
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/translations
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QM_FILES}
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/translations/
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${QM_FILES}
    app/MainWindow.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    Qt5::Widgets
)
''',

    "main.cpp": '''#include <QApplication>
#include <QTranslator>
#include "app/MainWindow.h"

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);

    QString locale = QLocale::system().name();   // 例如 "zh_CN"、"en_US"
    QString qmPath = QCoreApplication::applicationDirPath() + "/translations/" + locale + ".qm";

    QTranslator translator;
    if (translator.load(qmPath)) 
    {
        app.installTranslator(&translator);
    }

    MainWindow window;
    window.show();
    return app.exec();
}
''',

    "resources.qrc": '''<RCC>
  <qresource prefix="/icons">
    <file alias="L.svg">resources/icons/L.svg</file>
  </qresource>
</RCC>
''',

    "resources/icons/L.svg": '''<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 200 240" width="200" height="240">
  <defs>
    <linearGradient id="gothic" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" stop-color="#2C2C2C"/>
      <stop offset="100%" stop-color="#0A0A0A"/>
    </linearGradient>
  </defs>
  <path d="
    M 30 20
    L 95 20
    L 95 25
    L 75 35
    L 75 185
    L 130 185
    L 155 160
    L 165 175
    L 130 220
    L 30 220
    L 30 215
    L 50 205
    L 50 35
    L 30 25
    Z
  " fill="url(#gothic)"/>
  <path d="M 30 20 L 20 30 L 30 35 Z" fill="#0A0A0A"/>
  <path d="M 95 20 L 105 30 L 95 35 Z" fill="#0A0A0A"/>
</svg>
''',

    ".gitignore": "build/\n",
}


# ---------- 主逻辑 ----------

def generate(project_dir: str):
    """在 project_dir 下生成所有模板文件，返回 (生成列表, 跳过列表)。"""
    created = []
    skipped = []

    for rel_path, content in FILES.items():
        abs_path = os.path.join(project_dir, rel_path)
        os.makedirs(os.path.dirname(abs_path), exist_ok=True)

        if os.path.exists(abs_path):
            skipped.append(rel_path)
            continue

        with open(abs_path, "w", encoding="utf-8") as f:
            f.write(content)
        created.append(rel_path)

    return created, skipped


def main():
    parser = argparse.ArgumentParser(
        description="生成 Qt + CMake 项目模板"
    )
    parser.add_argument(
        "-p", "--process",
        required=True,
        help="项目名（也是生成的目录名）"
    )
    args = parser.parse_args()

    project_name = args.process
    project_dir = project_name  # 在当前工作目录下创建

    os.makedirs(project_dir, exist_ok=True)
    created, skipped = generate(project_dir)

    # ---------- 输出报告 ----------
    print(f"生成了{project_name}目录，目录下生成文件")
    for path in created:
        print(path)
    print(f"共计{len(created)}个文件")

    if skipped:
        print("下列文件的生成被跳过（已存在）")
        for path in skipped:
            print(path)
        print(f"共计{len(skipped)}个文件")


if __name__ == "__main__":
    main()

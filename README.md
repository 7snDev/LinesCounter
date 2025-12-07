# LinesCounter

A lightweight and fast C++ tool for counting lines of code across entire projects — including nested folders, multiple file types, and large codebases.  
Designed to be simple, accurate, and configurable using an ignore file.

---

## 📌 Overview

**LinesCounter** scans your project recursively and calculates:

- 📊 **Total lines** across the whole project  
- 📝 **Line count per file extension** (e.g., `.cpp`, `.py`, `.js` …)  
- 📂 **Directory- and file-level ignoring** via `.ignorefiles`  
- 🎯 **Wildcard support** (e.g., `*.exe`, `*.log`)  
- ⚡ **Multithreaded processing** for maximum speed  
- 🪶 Fully cross-platform (Windows / Linux)  
- 🔓 MIT Licensed  

Perfect for developers who want quick insight into their project’s size and code distribution.

---

## 🚀 How It Works

Simply run the executable inside your project folder.  
LinesCounter will:

1. Load ignored paths from `.ignorefiles` (if it exists)  
2. Scan all files and subfolders recursively  
3. Count lines grouped by extension  
4. Print total lines + percentages for each type  

Example output:

```
Total Lines : 15243
%.cpp : 62.4 | 9510 line
%.h   : 23.1 | 3518 line
%.py  : 14.4 | 2215 line
```

---

## 🧩 Ignoring Files & Folders

Create a file named:

```
.ignorefiles
```

Place it in your project root.

You can ignore:

### 🔹 Specific files
```
build.log
readme_old.txt
```

### 🔹 Entire folders
```
build
node_modules
dist
```

### 🔹 Extensions (wildcards)
```
*.exe
*.png
*.obj
```

Wildcards follow the simple `*.ext` pattern.

---

## 🛠️ Compile & Run

### Linux / macOS
```
g++ main.cpp -std=c++17 -pthread -o LinesCounter
./LinesCounter
```

### Windows (MinGW)
```
g++ main.cpp -std=c++17 -pthread -o LinesCounter.exe
LinesCounter.exe
```

Requires C++17 or higher.

---

## 📁 Project Structure

```
LinesCounter/
│── main.cpp
└── .ignorefiles   (optional)
```

---

## 📜 License

This project is released under the **MIT License**.  
Feel free to use, modify, and distribute it.

---

## 👨‍💻 Author

Made with ⚡ in C++ by **7sndev**  
GitHub: [@7sndev](https://github.com/7sndev)  
Repo: [7sndev/LinesCounter](https://github.com/7sndev/LinesCounter)

# 🧩 Sudoku Assistant Qt

> An interactive Qt-based assistant that helps users solve Sudoku grids — without solving it for them.

---

## 📋 Description

Sudoku Assistant is a desktop application built with **Qt (C++)** that provides real-time assistance to users solving Sudoku puzzles. Instead of solving the grid automatically, the application guides the user by dynamically filtering possible values, highlighting conflicts, and providing optional visual hints.

This project follows a strict **Model-View-Controller (MVC)** architecture with the **Observer pattern** implemented via Qt's signal/slot mechanism.

---

## ✨ Features

### Core
- 🎯 9×9 interactive Sudoku grid with dropdown menus per cell
- 🔄 Real-time candidate filtering — dropdowns only show valid remaining values
- 🔒 Fixed cells (initial grid values) are locked and visually distinct
- 📂 Load predefined grids from `.txt` files

### Visual Assistance (togglable)
- 🔴 Cells with **no possible values** highlighted in red (contradiction)
- 🟡 Cells with **only one possible value** highlighted in yellow (*naked single*)
- 🟠 Cells with **two possible values** highlighted in orange
- 🔵 Highlights the row, column, and 3×3 block of the selected cell

### Advanced Features
- 📝 **Candidate mode** — display small candidate numbers inside each cell (Sudoku pro style)
- ↩️ **Undo/Redo** history (Ctrl+Z / Ctrl+Y)
- ⏱️ Move counter and chronometer
- 🌍 **Internationalization** — French 🇫🇷 and English 🇬🇧 (via Qt Linguist)
- 💡 Tooltips and status bar with contextual messages

---

## 🏗️ Architecture

This project follows the **MVC pattern**:

```
sudoku-assistant-qt/
├── src/
│   ├── Model/
│   │   ├── SudokuModel.h/cpp       # Grid data, candidates, state
│   │   └── SudokuSolver.h/cpp      # Candidate calculation, contradiction detection
│   ├── View/
│   │   ├── MainWindow.h/cpp        # Main window, menus, toolbar
│   │   ├── SudokuGridView.h/cpp    # Grid widget (9x9 cells)
│   │   └── CellWidget.h/cpp        # Individual cell (QComboBox + styling)
│   └── Controller/
│       └── SudokuController.h/cpp  # Connects Model ↔ View
├── resources/
│   ├── grilles/                    # Predefined Sudoku grids (.txt)
│   └── i18n/                       # Translation files (.ts / .qm)
├── docs/                           # Report and documentation
└── sudoku-assistant.pro            # Qt project file
```

**Observer Pattern** is implemented natively through Qt signals and slots:
- The **Model** emits signals when data changes (`cellChanged`, `candidatesUpdated`, `contradictionDetected`)
- The **View** connects to these signals and updates automatically
- The **Controller** handles user input and calls Model methods

---

## ⌨️ Keyboard Shortcuts

| Action | Shortcut |
|---|---|
| Load grid | Ctrl+O |
| New grid | Ctrl+N |
| Undo | Ctrl+Z |
| Redo | Ctrl+Y |
| Toggle hints | Ctrl+H |
| Quit | Ctrl+Q |

---

## 🚀 Build Instructions

### Prerequisites
- Qt 6.x (or Qt 5.15+)
- C++17 compiler (GCC, Clang, or MSVC)
- CMake or qmake

### Build with qmake
```bash
git clone https://github.com/anaskartaoui/sudoku-assistant-qt.git
cd sudoku-assistant-qt
qmake sudoku-assistant.pro
make
./sudoku-assistant
```

### Build with Qt Creator
1. Open `sudoku-assistant.pro` in Qt Creator
2. Configure the kit (Qt version + compiler)
3. Click **Build** (Ctrl+B) then **Run** (Ctrl+R)

---

## 📁 Grid File Format

Grids are stored as plain `.txt` files using `0` for empty cells:

```
0 0 3 0 2 0 6 0 0
9 0 0 3 0 5 0 0 1
0 0 1 8 0 6 4 0 0
0 0 8 1 0 2 9 0 0
7 0 0 0 0 0 0 0 8
0 0 6 7 0 8 2 0 0
0 0 2 6 0 9 5 0 0
8 0 0 2 0 3 0 0 9
0 0 5 0 1 0 3 0 0
```

---

## 🖼️ Screenshots

> *(Coming soon — will be added after UI implementation)*

---

## 👤 Author

- **Anas KARTAOUI** — Master student in Computer Science (AI and Cybersecurity)
- Project for Qt Programming course — Université de Caen Normandie

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

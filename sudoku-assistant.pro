QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET   = sudoku-assistant
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/Model/SudokuModel.cpp \
    src/Model/SudokuSolver.cpp \
    src/View/MainWindow.cpp \
    src/View/SudokuGridView.cpp \
    src/View/CellWidget.cpp \
    src/View/NumPad.cpp \
    src/Controller/SudokuController.cpp

HEADERS += \
    src/Model/SudokuModel.h \
    src/Model/SudokuSolver.h \
    src/View/MainWindow.h \
    src/View/SudokuGridView.h \
    src/View/CellWidget.h \
    src/View/NumPad.h \
    src/Controller/SudokuController.h

TRANSLATIONS += \
    resources/i18n/sudoku_fr.ts \
    resources/i18n/sudoku_en.ts

RESOURCES += \
    resources/resources.qrc

DESTDIR     = build/
OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
RCC_DIR     = build/rcc
UI_DIR      = build/ui

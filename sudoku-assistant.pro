QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET   = sudoku-assistant
TEMPLATE = app

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

SOURCES += \
    src/main.cpp \
    src/Model/SudokuModel.cpp \
    src/Model/SudokuSolver.cpp \
    src/View/MainWindow.cpp \
    src/View/SudokuGridView.cpp \
    src/View/CellWidget.cpp \
    src/Controller/SudokuController.cpp

# ------------------------------------------------------------------------------
# Headers
# ------------------------------------------------------------------------------

HEADERS += \
    src/Model/SudokuModel.h \
    src/Model/SudokuSolver.h \
    src/View/MainWindow.h \
    src/View/SudokuGridView.h \
    src/View/CellWidget.h \
    src/Controller/SudokuController.h

# ------------------------------------------------------------------------------
# Internationalisation
# ------------------------------------------------------------------------------

TRANSLATIONS += \
    resources/i18n/sudoku_fr.ts \
    resources/i18n/sudoku_en.ts

# ------------------------------------------------------------------------------
# Resources
# ------------------------------------------------------------------------------

RESOURCES += \
    resources/resources.qrc

# ------------------------------------------------------------------------------
# Output directories
# ------------------------------------------------------------------------------

DESTDIR     = build/
OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
RCC_DIR     = build/rcc
UI_DIR      = build/ui

# ------------------------------------------------------------------------------
# Warnings
# ------------------------------------------------------------------------------

# Disable "unused parameter" warnings during early development
QMAKE_CXXFLAGS += -Wno-unused-parameter
#include "SudokuController.h"
#include "../Model/SudokuModel.h"

#include <QFile>
#include <QTextStream>

SudokuController::SudokuController(QObject *parent)
    : QObject(parent), m_hintsEnabled(true)
{
    m_model = new SudokuModel(this);
}

void SudokuController::loadDefaultGrid()
{
    static const int defaultGrid[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    m_model->loadGrid(defaultGrid);
    m_undoStack.clear();
    m_redoStack.clear();
}

void SudokuController::loadGridFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int grid[9][9] = {};
    QTextStream in(&file);
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            in >> grid[r][c];

    m_model->loadGrid(grid);
    m_undoStack.clear();
    m_redoStack.clear();
}

void SudokuController::setCellValue(int row, int col, int value)
{
    int oldValue = m_model->getValue(row, col);
    m_undoStack.push({{row, col}, oldValue});
    m_redoStack.clear();
    m_model->setValue(row, col, value);
}

void SudokuController::undo()
{
    if (m_undoStack.isEmpty()) return;
    CellState state = m_undoStack.pop();
    int row = state.first.first;
    int col = state.first.second;
    int oldValue = state.second;
    m_redoStack.push({{row, col}, m_model->getValue(row, col)});
    m_model->setValue(row, col, oldValue);
}

void SudokuController::redo()
{
    if (m_redoStack.isEmpty()) return;
    CellState state = m_redoStack.pop();
    int row = state.first.first;
    int col = state.first.second;
    int redoValue = state.second;
    m_undoStack.push({{row, col}, m_model->getValue(row, col)});
    m_model->setValue(row, col, redoValue);
}

void SudokuController::toggleHints()
{
    m_hintsEnabled = !m_hintsEnabled;
}

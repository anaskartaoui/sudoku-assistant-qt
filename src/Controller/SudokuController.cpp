#include "SudokuController.h"
#include "../Model/SudokuModel.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

SudokuController::SudokuController(QObject *parent)
    : QObject(parent), m_hintsEnabled(true)
{
    m_model = new SudokuModel(this);
}

void SudokuController::loadDefaultGrid()
{
    loadRandomGrid("Easy");
}

void SudokuController::loadRandomGrid(const QString &difficulty)
{
    QString path = QString(":/grilles/%1.txt").arg(difficulty);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    int count = 0;
    in >> count;
    if (count <= 0) return;

    int index = QRandomGenerator::global()->bounded(count);

    for (int i = 0; i <= index; ++i) {
        QString line;
        do { line = in.readLine(); } while (line.trimmed().isEmpty());
        if (i == index) {
            int grid[9][9] = {};
            for (int r = 0; r < 9; ++r)
                for (int c = 0; c < 9; ++c)
                    grid[r][c] = line[r * 9 + c].digitValue();
            m_model->loadGrid(grid);
        }
    }

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

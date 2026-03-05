#include "SudokuController.h"
#include "../Model/SudokuModel.h"
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>

SudokuController::SudokuController(QObject *parent)
    : QObject(parent)
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

    int N = in.readLine().trimmed().toInt();
    if (N < 1) return;

    int chosen = QRandomGenerator::global()->bounded(N) + 1;

    QString line;
    for (int i = 0; i < chosen; ++i)
        line = in.readLine();

    if (line.length() != 81) return;

    m_model->clearGrid();
    int grid[9][9] = {};
    for (int i = 0; i < 81; ++i)
        grid[i / 9][i % 9] = line[i].digitValue();
    m_model->loadGrid(grid);

    m_undoStack.clear();
    m_redoStack.clear();
}

void SudokuController::loadGridFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    int N = in.readLine().trimmed().toInt();
    if (N < 1) return;

    QString line = in.readLine();
    if (line.length() != 81) return;

    m_model->clearGrid();
    int grid[9][9] = {};
    for (int i = 0; i < 81; ++i)
        grid[i / 9][i % 9] = line[i].digitValue();
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
    m_model->setHintsEnabled(!m_model->hintsEnabled());
    m_model->updateAllCandidates();
}

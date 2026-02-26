#include "SudokuModel.h"

SudokuModel::SudokuModel(QObject *parent)
    : QObject(parent)
{
    clearGrid();
}

int SudokuModel::getValue(int row, int col) const
{
    return m_grid[row][col];
}

void SudokuModel::setValue(int row, int col, int value)
{
    if (m_fixed[row][col]) return;

    m_grid[row][col] = value;
    updateAllCandidates();

    emit cellChanged(row, col);

    if (isSolved())
        emit gridSolved();
}

bool SudokuModel::isFixed(int row, int col) const
{
    return m_fixed[row][col];
}

void SudokuModel::setFixed(int row, int col, bool fixed)
{
    m_fixed[row][col] = fixed;
}

QSet<int> SudokuModel::getCandidates(int row, int col) const
{
    return m_candidates[row][col];
}

void SudokuModel::updateAllCandidates()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            computeCandidates(r, c);

    emit candidatesUpdated();

    // Check for contradictions
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (m_grid[r][c] == 0 && m_candidates[r][c].isEmpty())
                emit contradictionDetected(r, c);
}

void SudokuModel::computeCandidates(int row, int col)
{
    if (m_grid[row][col] != 0) {
        m_candidates[row][col].clear();
        return;
    }

    QSet<int> used;

    // Row
    for (int c = 0; c < 9; ++c)
        if (m_grid[row][c] != 0)
            used.insert(m_grid[row][c]);

    // Column
    for (int r = 0; r < 9; ++r)
        if (m_grid[r][col] != 0)
            used.insert(m_grid[r][col]);

    // 3x3 block
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int r = blockRow; r < blockRow + 3; ++r)
        for (int c = blockCol; c < blockCol + 3; ++c)
            if (m_grid[r][c] != 0)
                used.insert(m_grid[r][c]);

    // Candidates = {1..9} - used
    m_candidates[row][col].clear();
    for (int v = 1; v <= 9; ++v)
        if (!used.contains(v))
            m_candidates[row][col].insert(v);
}

void SudokuModel::loadGrid(const int grid[9][9])
{
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            m_grid[r][c]  = grid[r][c];
            m_fixed[r][c] = (grid[r][c] != 0);
        }
    }
    updateAllCandidates();
}

void SudokuModel::clearGrid()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c) {
            m_grid[r][c]  = 0;
            m_fixed[r][c] = false;
            m_candidates[r][c].clear();
        }
}

bool SudokuModel::isSolved() const
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (m_grid[r][c] == 0) return false;
    return true;
}

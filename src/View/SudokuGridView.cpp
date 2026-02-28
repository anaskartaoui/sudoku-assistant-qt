#include "SudokuGridView.h"
#include "../Model/SudokuModel.h"
#include <QGridLayout>

SudokuGridView::SudokuGridView(SudokuModel *model, QWidget *parent)
    : QWidget(parent), m_model(model),
    m_selectedRow(-1), m_selectedCol(-1)
{
    setupGrid();
    connect(m_model, &SudokuModel::candidatesUpdated,
            this, &SudokuGridView::onCandidatesUpdated);
    connect(m_model, &SudokuModel::contradictionDetected,
            this, &SudokuGridView::onContradictionDetected);
}

void SudokuGridView::setupGrid()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(0);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            m_cells[r][c] = new CellWidget(r, c, this);
            m_cells[r][c]->setBlockBorders(r, c);
            layout->addWidget(m_cells[r][c], r, c);
            connect(m_cells[r][c], &CellWidget::cellClicked,
                    this, &SudokuGridView::onCellClicked);
        }
    }

    setLayout(layout);
    setFixedSize(sizeHint());
}

void SudokuGridView::applyValue(int value)
{
    if (m_selectedRow == -1 || m_selectedCol == -1) return;
    emit valueChanged(m_selectedRow, m_selectedCol, value);
    m_cells[m_selectedRow][m_selectedCol]->setValue(value);
}

void SudokuGridView::onCellClicked(int row, int col)
{
    clearHighlight();
    clearSelection();
    m_selectedRow = row;
    m_selectedCol = col;
    m_cells[row][col]->setSelected(true);
    applyHighlight(row, col);
    emit cellSelected(row, col);
}

void SudokuGridView::clearSelection()
{
    if (m_selectedRow != -1 && m_selectedCol != -1)
        m_cells[m_selectedRow][m_selectedCol]->setSelected(false);
    m_selectedRow = -1;
    m_selectedCol = -1;
}

void SudokuGridView::clearHighlight()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            m_cells[r][c]->setHighlighted(false);
}

void SudokuGridView::applyHighlight(int row, int col)
{
    // Ligne et colonne
    for (int i = 0; i < 9; ++i) {
        if (i != col) m_cells[row][i]->setHighlighted(true);
        if (i != row) m_cells[i][col]->setHighlighted(true);
    }

    // Bloc 3x3
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int r = blockRow; r < blockRow + 3; ++r)
        for (int c = blockCol; c < blockCol + 3; ++c)
            if (r != row || c != col)
                m_cells[r][c]->setHighlighted(true);
}

void SudokuGridView::refreshCell(int row, int col)
{
    CellWidget *cell     = m_cells[row][col];
    int value            = m_model->getValue(row, col);
    bool fixed           = m_model->isFixed(row, col);
    QSet<int> candidates = m_model->getCandidates(row, col);
    cell->setFixed(fixed);
    cell->setValue(value);
    cell->setCandidates(candidates);
}

void SudokuGridView::resetView()
{
    clearHighlight();
    clearSelection();
}

void SudokuGridView::onCandidatesUpdated()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            refreshCell(r, c);
}

void SudokuGridView::onContradictionDetected(int row, int col)
{
    m_cells[row][col]->setContradiction(true);
}

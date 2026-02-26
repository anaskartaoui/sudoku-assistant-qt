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
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(2);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int layoutRow = r + (r / 3);
            int layoutCol = c + (c / 3);

            m_cells[r][c] = new CellWidget(r, c, this);
            layout->addWidget(m_cells[r][c], layoutRow, layoutCol);

            connect(m_cells[r][c], &CellWidget::cellClicked,
                    this, &SudokuGridView::onCellClicked);
        }
    }

    // Largeur/hauteur des séparateurs de blocs
    layout->setColumnMinimumWidth(3, 8);
    layout->setColumnMinimumWidth(7, 8);
    layout->setRowMinimumHeight(3, 8);
    layout->setRowMinimumHeight(7, 8);

    setLayout(layout);
}

void SudokuGridView::applyValue(int value)
{
    if (m_selectedRow == -1 || m_selectedCol == -1) return;

    emit valueChanged(m_selectedRow, m_selectedCol, value);
    m_cells[m_selectedRow][m_selectedCol]->setValue(value);
}

void SudokuGridView::onCellClicked(int row, int col)
{
    clearSelection();
    m_selectedRow = row;
    m_selectedCol = col;
    m_cells[row][col]->setSelected(true);
    emit cellSelected(row, col);
}

void SudokuGridView::clearSelection()
{
    if (m_selectedRow != -1 && m_selectedCol != -1)
        m_cells[m_selectedRow][m_selectedCol]->setSelected(false);
    m_selectedRow = -1;
    m_selectedCol = -1;
}

void SudokuGridView::refreshCell(int row, int col)
{
    CellWidget *cell = m_cells[row][col];
    int value        = m_model->getValue(row, col);
    bool fixed       = m_model->isFixed(row, col);
    QSet<int> candidates = m_model->getCandidates(row, col);

    cell->setFixed(fixed);
    cell->setValue(value);
    cell->setCandidates(candidates);
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


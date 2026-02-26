#include "SudokuGridView.h"
#include "../Model/SudokuModel.h"

#include <QGridLayout>

SudokuGridView::SudokuGridView(SudokuModel *model, QWidget *parent)
    : QWidget(parent), m_model(model)
{
    setupGrid();

    connect(m_model, &SudokuModel::cellChanged,
            this, &SudokuGridView::onCellChanged);
    connect(m_model, &SudokuModel::candidatesUpdated,
            this, &SudokuGridView::onCandidatesUpdated);
    connect(m_model, &SudokuModel::contradictionDetected,
            this, &SudokuGridView::onContradictionDetected);
}

void SudokuGridView::setupGrid()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(1);
    layout->setContentsMargins(10, 10, 10, 10);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            m_cells[r][c] = new CellWidget(r, c, this);
            layout->addWidget(m_cells[r][c], r, c);

            // Forward cell signal to controller
            connect(m_cells[r][c], &CellWidget::valueSelected,
                    this, &SudokuGridView::cellValueSelected);
        }
    }
    setLayout(layout);
}

void SudokuGridView::refreshCell(int row, int col)
{
    CellWidget *cell = m_cells[row][col];
    int value        = m_model->getValue(row, col);
    bool fixed       = m_model->isFixed(row, col);
    QSet<int> candidates = m_model->getCandidates(row, col);

    cell->setFixed(fixed);
    cell->setValue(value);
    if (!fixed)
        cell->setCandidates(candidates);
}

void SudokuGridView::onCellChanged(int row, int col)
{
    refreshCell(row, col);
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

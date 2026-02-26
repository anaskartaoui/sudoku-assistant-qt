#ifndef SUDOKUGRIDVIEW_H
#define SUDOKUGRIDVIEW_H

#include <QWidget>
#include "CellWidget.h"

class SudokuModel;

/**
 * @brief The SudokuGridView class
 * Displays the 9x9 Sudoku grid using CellWidget instances.
 * Connects to the model's signals to refresh the UI automatically.
 */
class SudokuGridView : public QWidget
{
    Q_OBJECT

public:
    explicit SudokuGridView(SudokuModel *model, QWidget *parent = nullptr);

public slots:
    void onCellChanged(int row, int col);
    void onCandidatesUpdated();
    void onContradictionDetected(int row, int col);

signals:
    void cellValueSelected(int row, int col, int value);

private:
    void setupGrid();
    void refreshCell(int row, int col);

    SudokuModel *m_model;
    CellWidget  *m_cells[9][9];
};

#endif // SUDOKUGRIDVIEW_H

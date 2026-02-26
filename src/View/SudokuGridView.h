#ifndef SUDOKUGRIDVIEW_H
#define SUDOKUGRIDVIEW_H

#include <QWidget>
#include "CellWidget.h"

class SudokuModel;

/**
 * @brief The SudokuGridView class
 * Displays the 9x9 Sudoku grid using CellWidget instances.
 * Manages cell selection and connects to model signals.
 */
class SudokuGridView : public QWidget
{
    Q_OBJECT

public:
    explicit SudokuGridView(SudokuModel *model, QWidget *parent = nullptr);

    // Called by MainWindow when user clicks a number on the numpad
    void applyValue(int value);

signals:
    void cellSelected(int row, int col);
    void valueChanged(int row, int col, int value);

public slots:
    void onCandidatesUpdated();
    void onContradictionDetected(int row, int col);

private slots:
    void onCellClicked(int row, int col);

private:
    void setupGrid();
    void refreshCell(int row, int col);
    void clearSelection();

    SudokuModel *m_model;
    CellWidget  *m_cells[9][9];
    int          m_selectedRow;
    int          m_selectedCol;

};

#endif // SUDOKUGRIDVIEW_H

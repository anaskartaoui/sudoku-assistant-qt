#ifndef SUDOKUGRIDVIEW_H
#define SUDOKUGRIDVIEW_H
#include <QWidget>
#include <QLabel>
#include "CellWidget.h"
class SudokuModel;

class SudokuGridView : public QWidget
{
    Q_OBJECT
public:
    explicit SudokuGridView(SudokuModel *model, QWidget *parent = nullptr);
    void applyValue(int value);
    void resetView();
    void setPaused(bool paused);
signals:
    void cellSelected(int row, int col);
    void valueChanged(int row, int col, int value);
    void pauseToggled();
public slots:
    void onCandidatesUpdated();
    void onContradictionDetected(int row, int col);
private slots:
    void onCellClicked(int row, int col);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    void setupGrid();
    void refreshCell(int row, int col);
    void clearSelection();
    void applyHighlight(int row, int col);
    void clearHighlight();
    void selectCell(int row, int col);
    SudokuModel *m_model;
    CellWidget  *m_cells[9][9];
    int          m_selectedRow;
    int          m_selectedCol;
    QLabel      *m_pauseOverlay;
};
#endif // SUDOKUGRIDVIEW_H

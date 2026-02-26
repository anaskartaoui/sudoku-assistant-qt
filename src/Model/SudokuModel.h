#ifndef SUDOKUMODEL_H
#define SUDOKUMODEL_H

#include <QObject>
#include <QSet>

/**
 * @brief The SudokuModel class
 * Holds the grid data and computes candidates for each cell.
 * Emits signals when the state changes (Observer pattern via Qt signals/slots).
 */
class SudokuModel : public QObject
{
    Q_OBJECT

public:
    explicit SudokuModel(QObject *parent = nullptr);

    // Grid access
    int  getValue(int row, int col) const;
    void setValue(int row, int col, int value);
    bool isFixed(int row, int col) const;
    void setFixed(int row, int col, bool fixed);

    // Candidates
    QSet<int> getCandidates(int row, int col) const;
    void updateAllCandidates();

    // Grid management
    void loadGrid(const int grid[9][9]);
    void clearGrid();
    bool isSolved() const;

signals:
    void cellChanged(int row, int col);
    void candidatesUpdated();
    void contradictionDetected(int row, int col);
    void gridSolved();

private:
    int  m_grid[9][9];
    bool m_fixed[9][9];
    QSet<int> m_candidates[9][9];

    void computeCandidates(int row, int col);
};

#endif // SUDOKUMODEL_H

#ifndef SUDOKUCONTROLLER_H
#define SUDOKUCONTROLLER_H

#include <QObject>
#include <QStack>
#include <QPair>

class SudokuModel;

/**
 * @brief The SudokuController class
 * Bridges the Model and View.
 * Handles user actions: cell input, undo/redo, grid loading, hints toggle.
 */
class SudokuController : public QObject
{
    Q_OBJECT

public:
    explicit SudokuController(QObject *parent = nullptr);

    SudokuModel *model() const { return m_model; }

    void loadDefaultGrid();
    void loadGridFromFile(const QString &path);
    void toggleHints();

public slots:
    void setCellValue(int row, int col, int value);
    void undo();
    void redo();

private:
    SudokuModel *m_model;
    bool m_hintsEnabled;

    using CellState = QPair<QPair<int,int>, int>;
    QStack<CellState> m_undoStack;
    QStack<CellState> m_redoStack;
};

#endif // SUDOKUCONTROLLER_H

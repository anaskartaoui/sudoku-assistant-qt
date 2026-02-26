#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QSet>

class QComboBox;

/**
 * @brief The CellWidget class
 * Represents a single cell in the Sudoku grid.
 * Shows a QComboBox with only valid candidate values.
 * Fixed cells display their value as a label (not editable).
 */
class CellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellWidget(int row, int col, QWidget *parent = nullptr);

    void setValue(int value);
    void setCandidates(const QSet<int> &candidates);
    void setFixed(bool fixed);
    void setContradiction(bool contradiction);
    void setNakedSingle(bool single);
    void setSelected(bool selected);

signals:
    void valueSelected(int row, int col, int value);

private slots:
    void onComboChanged(int index);

private:
    int        m_row;
    int        m_col;
    bool       m_fixed;
    bool       m_contradiction;
    bool       m_nakedSingle;
    bool       m_selected;
    QComboBox *m_combo;

    void updateStyle();
};

#endif // CELLWIDGET_H

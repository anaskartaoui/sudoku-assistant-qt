#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSet>

/**
 * @brief The CellWidget class
 * Represents a single cell in the Sudoku grid.
 * Displays a number as a QLabel. Clickable to select.
 */
class CellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellWidget(int row, int col, QWidget *parent = nullptr);

    void setValue(int value);
    int  getValue() const { return m_value; }
    void setCandidates(const QSet<int> &candidates);
    void setFixed(bool fixed);
    void setSelected(bool selected);
    void setContradiction(bool contradiction);
    void setNakedSingle(bool nakedSingle);

signals:
    void cellClicked(int row, int col);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int     m_row;
    int     m_col;
    int     m_value;
    bool    m_fixed;
    bool    m_selected;
    bool    m_contradiction;
    bool    m_nakedSingle;
    QLabel *m_label;

    void updateStyle();
};

#endif // CELLWIDGET_H

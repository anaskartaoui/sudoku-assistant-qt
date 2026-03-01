#ifndef CELLWIDGET_H
#define CELLWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QSet>
#include <QGridLayout>

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
    void setBlockBorders(int row, int col);
    void setHighlighted(bool highlighted);
signals:
    void cellClicked(int row, int col);
protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    int       m_row, m_col, m_value;
    bool      m_fixed, m_selected, m_contradiction, m_nakedSingle, m_highlighted;
    QSet<int> m_candidates;
    QLabel      *m_label;
    QLabel      *m_pencilLabels[9];
    QGridLayout *m_pencilLayout;
    QWidget     *m_pencilWidget;
    void updateStyle();
    void updatePencilMarks();
};
#endif // CELLWIDGET_H

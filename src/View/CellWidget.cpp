#include "CellWidget.h"
#include <QMouseEvent>
#include <QVBoxLayout>

CellWidget::CellWidget(int row, int col, QWidget *parent)
    : QWidget(parent), m_row(row), m_col(col),
    m_value(0), m_fixed(false), m_selected(false),
    m_contradiction(false), m_nakedSingle(false)
{
    setFixedSize(55, 55);
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setFixedSize(55, 55);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_label);
    setLayout(layout);
    updateStyle();
}

void CellWidget::setValue(int value)
{
    m_value = value;
    m_label->setText(value != 0 ? QString::number(value) : "");
    updateStyle();
}

void CellWidget::setCandidates(const QSet<int> &candidates)
{
    m_nakedSingle = (candidates.size() == 1);
    updateStyle();
}

void CellWidget::setFixed(bool fixed)
{
    m_fixed = fixed;
    updateStyle();
}

void CellWidget::setSelected(bool selected)
{
    m_selected = selected;
    updateStyle();
}

void CellWidget::setContradiction(bool contradiction)
{
    m_contradiction = contradiction;
    updateStyle();
}

void CellWidget::setNakedSingle(bool nakedSingle)
{
    m_nakedSingle = nakedSingle;
    updateStyle();
}

void CellWidget::setBlockBorders(int row, int col)
{
    m_row = row;
    m_col = col;
    updateStyle();
}

void CellWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!m_fixed)
        emit cellClicked(m_row, m_col);
}

void CellWidget::updateStyle()
{
    QString bgColor, textColor;

    if (m_fixed) {
        bgColor   = "#EAF0F6";
        textColor = "#2C3E50";
    } else if (m_selected) {
        bgColor   = "#C8D8F0";
        textColor = "#1A2A3A";
    } else if (m_contradiction) {
        bgColor   = "#FADBD8";
        textColor = "#C0392B";
    } else if (m_nakedSingle) {
        bgColor   = "#D6EAF8";
        textColor = "#1F618D";
    } else {
        bgColor   = "#FFFFFF";
        textColor = "#5B8CCC";
    }

    int top    = (m_row % 3 == 0) ? 3 : 1;
    int left   = (m_col % 3 == 0) ? 3 : 1;
    int bottom = (m_row == 8)     ? 3 : 0;
    int right  = (m_col == 8)     ? 3 : 0;

    QString fontSize   = m_fixed ? "22px" : "18px";
    QString fontWeight = "bold";

    m_label->setStyleSheet(QString(
                               "QLabel {"
                               "  background-color: %1;"
                               "  color: %2;"
                               "  font-size: %3;"
                               "  font-weight: %4;"
                               "  border-top:    %5px solid #7A9AB5;"
                               "  border-left:   %6px solid #7A9AB5;"
                               "  border-bottom: %7px solid #7A9AB5;"
                               "  border-right:  %8px solid #7A9AB5;"
                               "}"
                               ).arg(bgColor, textColor, fontSize, fontWeight)
                               .arg(top).arg(left).arg(bottom).arg(right));
}

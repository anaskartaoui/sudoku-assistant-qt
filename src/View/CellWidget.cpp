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

void CellWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!m_fixed)
        emit cellClicked(m_row, m_col);
}

void CellWidget::updateStyle()
{
    QString bgColor, textColor, fontWeight;
    fontWeight = "bold";

    if (m_fixed) {
        bgColor   = "#D4C4A8";  // plus foncé
        textColor = "#2C2C2C";  // presque noir
    } else if (m_selected) {
        bgColor   = "#DD7D59";
        textColor = "#FFFFFF";
    } else if (m_contradiction) {
        bgColor   = "#E74C3C";
        textColor = "#FFFFFF";
    } else if (m_nakedSingle) {
        bgColor   = "#F39C12";
        textColor = "#FFFFFF";
    } else {
        bgColor   = "#FDF8E2";  // crème clair
        textColor = "#DD7D59";
    }

    QString fontSize = m_fixed ? "22px" : "18px";

    m_label->setStyleSheet(QString(
                               "QLabel {"
                               "  background-color: %1;"
                               "  color: %2;"
                               "  font-size: %3;"
                               "  font-weight: %4;"
                               "  border-radius: 4px;"
                               "  border: 2px;"
                               "}"
                               ).arg(bgColor, textColor, fontSize, fontWeight));
}

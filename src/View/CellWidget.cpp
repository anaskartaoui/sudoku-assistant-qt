#include "CellWidget.h"
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QGridLayout>

CellWidget::CellWidget(int row, int col, QWidget *parent)
    : QWidget(parent), m_row(row), m_col(col),
    m_value(0), m_fixed(false), m_selected(false),
    m_contradiction(false), m_nakedSingle(false), m_highlighted(false)
{
    setFixedSize(55, 55);

    // Label principal (chiffre)
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setGeometry(0, 0, 55, 55);

    // Widget pencil marks (grille 3x3 de petits chiffres)
    m_pencilWidget = new QWidget(this);
    m_pencilWidget->setObjectName("pencilWidget");
    m_pencilWidget->setGeometry(0, 0, 55, 55);
    m_pencilLayout = new QGridLayout(m_pencilWidget);
    m_pencilLayout->setContentsMargins(2, 2, 2, 2);
    m_pencilLayout->setSpacing(0);

    for (int i = 0; i < 9; ++i) {
        m_pencilLabels[i] = new QLabel(m_pencilWidget);
        m_pencilLabels[i]->setAlignment(Qt::AlignCenter);
        m_pencilLabels[i]->setStyleSheet(
            "QLabel { color: #2C3E50; font-size: 9px; font-weight: bold; background: transparent; }"
            );
        m_pencilLayout->addWidget(m_pencilLabels[i], i / 3, i % 3);
    }

    m_pencilWidget->hide();
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
    m_candidates  = candidates;
    m_nakedSingle = (candidates.size() == 1);
    updateStyle();
    updatePencilMarks();
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
    updatePencilMarks();
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

void CellWidget::setHighlighted(bool highlighted)
{
    m_highlighted = highlighted;
    updateStyle();
}

void CellWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!m_fixed)
        emit cellClicked(m_row, m_col);
}

void CellWidget::updatePencilMarks()
{
    // Affiche pencil marks uniquement si cellule vide et sélectionnée
    if (m_value == 0 && m_selected && !m_fixed) {
        for (int i = 0; i < 9; ++i) {
            int num = i + 1;
            m_pencilLabels[i]->setText(
                m_candidates.contains(num) ? QString::number(num) : ""
                );
        }
        m_pencilWidget->show();
        m_label->hide();
    } else {
        m_pencilWidget->hide();
        m_label->show();
    }
}

void CellWidget::updateStyle()
{
    QString bgColor, textColor;

    if (m_selected) {
        bgColor   = "#B8D4ED";
        textColor = "#1A2A3A";
    } else if (m_contradiction) {
        bgColor   = "#FADBD8";
        textColor = "#C0392B";
    } else if (m_nakedSingle) {
        bgColor   = "#D5F5E3";
        textColor = "#1E8449";
    } else if (m_highlighted) {
        bgColor   = "#EAF2FB";
        textColor = m_fixed ? "#2C3E50" : "#5B8CCC";
    } else if (m_fixed) {
        bgColor   = "#EAF0F6";
        textColor = "#2C3E50";
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

    QString borderStyle = QString(
                              "border-top:    %1px solid #7A9AB5;"
                              "border-left:   %2px solid #7A9AB5;"
                              "border-bottom: %3px solid #7A9AB5;"
                              "border-right:  %4px solid #7A9AB5;"
                              ).arg(top).arg(left).arg(bottom).arg(right);

    // Bordures + fond sur le label
    m_label->setStyleSheet(QString(
                               "QLabel {"
                               "  background-color: %1;"
                               "  color: %2;"
                               "  font-size: %3;"
                               "  font-weight: %4;"
                               "  %5"
                               "}"
                               ).arg(bgColor, textColor, fontSize, fontWeight, borderStyle));

    // Même bordures + fond sur le pencilWidget
    m_pencilWidget->setStyleSheet(QString(
                                      "QWidget#pencilWidget {"
                                      "  background-color: %1;"
                                      "  %2"
                                      "}"
                                      ).arg(bgColor, borderStyle));
}

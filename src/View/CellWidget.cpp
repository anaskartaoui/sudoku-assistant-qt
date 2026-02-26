#include "CellWidget.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <QList>
#include <algorithm>

CellWidget::CellWidget(int row, int col, QWidget *parent)
    : QWidget(parent), m_row(row), m_col(col),
      m_fixed(false), m_contradiction(false),
      m_nakedSingle(false), m_selected(false)
{
    m_combo = new QComboBox(this);
    m_combo->setMinimumSize(50, 50);
    m_combo->setMaximumSize(60, 60);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->addWidget(m_combo);
    setLayout(layout);

    connect(m_combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CellWidget::onComboChanged);

    updateStyle();
}

void CellWidget::setValue(int value)
{
    m_combo->blockSignals(true);
    m_combo->clear();
    if (value != 0)
        m_combo->addItem(QString::number(value));
    m_combo->blockSignals(false);
    updateStyle();
}

void CellWidget::setCandidates(const QSet<int> &candidates)
{
    if (m_fixed) return;

    m_combo->blockSignals(true);
    m_combo->clear();

    // Empty option first
    m_combo->addItem("", 0);

    // Sort candidates
    QList<int> sorted = candidates.values();
    std::sort(sorted.begin(), sorted.end());
    for (int v : sorted)
        m_combo->addItem(QString::number(v), v);

    m_nakedSingle = (candidates.size() == 1);
    m_combo->blockSignals(false);
    updateStyle();
}

void CellWidget::setFixed(bool fixed)
{
    m_fixed = fixed;
    m_combo->setEnabled(!fixed);
    updateStyle();
}

void CellWidget::setContradiction(bool contradiction)
{
    m_contradiction = contradiction;
    updateStyle();
}

void CellWidget::setNakedSingle(bool single)
{
    m_nakedSingle = single;
    updateStyle();
}

void CellWidget::setSelected(bool selected)
{
    m_selected = selected;
    updateStyle();
}

void CellWidget::onComboChanged(int index)
{
    if (index < 0) return;
    int value = m_combo->itemData(index).toInt();
    emit valueSelected(m_row, m_col, value);
}

void CellWidget::updateStyle()
{
    QString style;

    if (m_fixed) {
        style = "background-color: #d0d0d0; font-weight: bold; font-size: 16px;";
    } else if (m_contradiction) {
        style = "background-color: #ff6b6b;";
    } else if (m_nakedSingle) {
        style = "background-color: #fff176;";
    } else if (m_selected) {
        style = "background-color: #bbdefb;";
    } else {
        style = "background-color: white;";
    }

    m_combo->setStyleSheet(style);
}

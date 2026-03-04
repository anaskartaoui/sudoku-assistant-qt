#include "SudokuGridView.h"
#include "../Model/SudokuModel.h"
#include <QGridLayout>
#include <QPainter>
#include <QKeyEvent>

SudokuGridView::SudokuGridView(SudokuModel *model, QWidget *parent)
    : QWidget(parent), m_model(model),
    m_selectedRow(-1), m_selectedCol(-1)
{
    setupGrid();
    setFocusPolicy(Qt::StrongFocus);

    m_pauseOverlay = new QLabel(this);
    m_pauseOverlay->setAlignment(Qt::AlignCenter);
    QPixmap px(":/images/pause.png");
    QPixmap colored(px.size());
    colored.fill(Qt::transparent);
    QPainter painter(&colored);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, px);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(colored.rect(), QColor("#7A9AB5"));
    painter.end();

    m_pauseOverlay->setPixmap(
        colored.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    m_pauseOverlay->setStyleSheet(
        "QLabel { background-color: rgba(221, 230, 240, 220); }"
        );
    m_pauseOverlay->setGeometry(0, 0, width(), height());
    m_pauseOverlay->hide();

    connect(m_model, &SudokuModel::candidatesUpdated,
            this, &SudokuGridView::onCandidatesUpdated);
    connect(m_model, &SudokuModel::contradictionDetected,
            this, &SudokuGridView::onContradictionDetected);
}

void SudokuGridView::setupGrid()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(0);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            m_cells[r][c] = new CellWidget(r, c, this);
            m_cells[r][c]->setBlockBorders(r, c);
            layout->addWidget(m_cells[r][c], r, c);
            connect(m_cells[r][c], &CellWidget::cellClicked,
                    this, &SudokuGridView::onCellClicked);
        }
    }

    setLayout(layout);
    setFixedSize(sizeHint());
}

void SudokuGridView::selectCell(int row, int col)
{
    if (row < 0 || row > 8 || col < 0 || col > 8) return;
    clearHighlight();
    clearSelection();
    m_selectedRow = row;
    m_selectedCol = col;
    m_cells[row][col]->setSelected(true);
    applyHighlight(row, col);
    emit cellSelected(row, col);
}

void SudokuGridView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if (m_selectedRow > 0) selectCell(m_selectedRow - 1, m_selectedCol);
        break;
    case Qt::Key_Down:
        if (m_selectedRow < 8) selectCell(m_selectedRow + 1, m_selectedCol);
        break;
    case Qt::Key_Left:
        if (m_selectedCol > 0) selectCell(m_selectedRow, m_selectedCol - 1);
        break;
    case Qt::Key_Right:
        if (m_selectedCol < 8) selectCell(m_selectedRow, m_selectedCol + 1);
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        applyValue(0);
        break;
    case Qt::Key_Space:
        emit pauseToggled();
        break;
    case Qt::Key_Escape:
        clearHighlight();
        clearSelection();
        break;
    case Qt::Key_1: case Qt::Key_2: case Qt::Key_3:
    case Qt::Key_4: case Qt::Key_5: case Qt::Key_6:
    case Qt::Key_7: case Qt::Key_8: case Qt::Key_9:
        applyValue(event->key() - Qt::Key_0);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void SudokuGridView::setPaused(bool paused)
{
    if (paused) {
        m_pauseOverlay->setGeometry(0, 0, width(), height());
        m_pauseOverlay->show();
        m_pauseOverlay->raise();
    } else {
        m_pauseOverlay->hide();
    }
}

void SudokuGridView::applyValue(int value)
{
    if (m_selectedRow == -1 || m_selectedCol == -1) return;
    emit valueChanged(m_selectedRow, m_selectedCol, value);
    m_cells[m_selectedRow][m_selectedCol]->setValue(value);
}

void SudokuGridView::onCellClicked(int row, int col)
{
    selectCell(row, col);
    setFocus();
}

void SudokuGridView::clearSelection()
{
    if (m_selectedRow != -1 && m_selectedCol != -1)
        m_cells[m_selectedRow][m_selectedCol]->setSelected(false);
    m_selectedRow = -1;
    m_selectedCol = -1;
}

void SudokuGridView::clearHighlight()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            m_cells[r][c]->setHighlighted(false);
}

void SudokuGridView::applyHighlight(int row, int col)
{
    for (int i = 0; i < 9; ++i) {
        if (i != col) m_cells[row][i]->setHighlighted(true);
        if (i != row) m_cells[i][col]->setHighlighted(true);
    }
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int r = blockRow; r < blockRow + 3; ++r)
        for (int c = blockCol; c < blockCol + 3; ++c)
            if (r != row || c != col)
                m_cells[r][c]->setHighlighted(true);
}

void SudokuGridView::refreshCell(int row, int col)
{
    CellWidget *cell     = m_cells[row][col];
    int value            = m_model->getValue(row, col);
    bool fixed           = m_model->isFixed(row, col);
    QSet<int> candidates = m_model->getCandidates(row, col);
    cell->setNakedSingle(false);
    cell->setFixed(fixed);
    cell->setValue(value);
    cell->setCandidates(candidates);
}

void SudokuGridView::resetView()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c) {
            m_cells[r][c]->setContradiction(false);
            m_cells[r][c]->setHighlighted(false);
            m_cells[r][c]->setSelected(false);
            refreshCell(r, c);
        }
    m_selectedRow = -1;
    m_selectedCol = -1;
    m_pauseOverlay->hide();
}

void SudokuGridView::onCandidatesUpdated()
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c) {
            m_cells[r][c]->setContradiction(false);
            refreshCell(r, c);
        }
    if (m_selectedRow != -1 && m_selectedCol != -1) {
        m_cells[m_selectedRow][m_selectedCol]->setSelected(true);
        applyHighlight(m_selectedRow, m_selectedCol);
    }
}

void SudokuGridView::onContradictionDetected(int row, int col)
{
    m_cells[row][col]->setContradiction(true);
}

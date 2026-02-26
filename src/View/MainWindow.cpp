#include "MainWindow.h"
#include "SudokuGridView.h"
#include "NumPad.h"
#include "../Controller/SudokuController.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Sudoku Assistant"));
    resize(700, 950);
    setMinimumSize(700, 950);


    // Fond teal
    setStyleSheet("QMainWindow { background-color: #2D7D7D; }");

    m_controller = new SudokuController(this);

    setupCentralWidget();
    setupMenus();
    setupToolbar();
    setupStatusBar();

    // Connexions
    connect(m_gridView, &SudokuGridView::valueChanged,
            m_controller, &SudokuController::setCellValue);
    connect(m_numPad, &NumPad::numberClicked,
            this, &MainWindow::onNumberClicked);

    m_controller->loadDefaultGrid();
}

MainWindow::~MainWindow() {}

void MainWindow::setupCentralWidget()
{
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #2D7D7D;");

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(80, 30, 30, 30);
    layout->setSpacing(20);
    layout->setAlignment(Qt::AlignCenter);

    // Plateau bois vintage autour de la grille
    QWidget *board = new QWidget(this);
    board->setStyleSheet(
        "QWidget {"
        "  background-color: #B08A5A;"
        "  border-radius: 18px;"
        "  border: None;"
        "}"
        );

    QVBoxLayout *boardLayout = new QVBoxLayout(board);
    boardLayout->setContentsMargins(12, 12, 12, 12);

    m_gridView = new SudokuGridView(m_controller->model(), this);
    m_gridView->setStyleSheet(
        "SudokuGridView {"
        "  background-color: #FDF8E2;"
        "  border-radius: 8px;"
        "}"
        );

    boardLayout->addWidget(m_gridView);
    board->setLayout(boardLayout);
    board->setFixedSize(581, 581);

    m_numPad = new NumPad(this);

    layout->addWidget(board, 0, Qt::AlignCenter);
    layout->addWidget(m_numPad, 0, Qt::AlignCenter);

    central->setLayout(layout);
    setCentralWidget(central);
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Fichier"));

    QAction *newAction = fileMenu->addAction(tr("&Nouvelle grille"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setToolTip(tr("Charger une nouvelle grille"));
    connect(newAction, &QAction::triggered, this, &MainWindow::onNewGrid);

    QAction *loadAction = fileMenu->addAction(tr("&Ouvrir grille..."));
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setToolTip(tr("Ouvrir une grille depuis un fichier"));
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadGrid);

    fileMenu->addSeparator();

    QAction *quitAction = fileMenu->addAction(tr("&Quitter"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu(tr("&Édition"));

    QAction *undoAction = editMenu->addAction(tr("&Annuler"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);

    QAction *redoAction = editMenu->addAction(tr("&Rétablir"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Aide"));

    QAction *hintsAction = helpMenu->addAction(tr("Activer/désactiver les &indices"));
    hintsAction->setShortcut(tr("Ctrl+H"));
    hintsAction->setCheckable(true);
    hintsAction->setChecked(true);
    connect(hintsAction, &QAction::triggered, this, &MainWindow::onToggleHints);
}

void MainWindow::setupToolbar()
{
    QToolBar *toolbar = addToolBar(tr("Barre d'outils"));
    toolbar->setMovable(false);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Prêt — Sélectionnez une case puis un chiffre."));
    statusBar()->setStyleSheet("background-color: #1f5f5f; color: white;");
}

void MainWindow::onNewGrid()
{
    m_controller->loadDefaultGrid();
    statusBar()->showMessage(tr("Nouvelle grille chargée."));
}

void MainWindow::onLoadGrid()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        tr("Ouvrir une grille Sudoku"),
        "",
        tr("Fichiers Sudoku (*.txt *.sdk);;Tous les fichiers (*)")
        );
    if (!path.isEmpty()) {
        m_controller->loadGridFromFile(path);
        statusBar()->showMessage(tr("Grille chargée depuis %1").arg(path));
    }
}

void MainWindow::onUndo()
{
    m_controller->undo();
    statusBar()->showMessage(tr("Annulation effectuée."));
}

void MainWindow::onRedo()
{
    m_controller->redo();
    statusBar()->showMessage(tr("Rétablissement effectué."));
}

void MainWindow::onToggleHints()
{
    m_controller->toggleHints();
}

void MainWindow::onNumberClicked(int value)
{
    m_gridView->applyValue(value);
}

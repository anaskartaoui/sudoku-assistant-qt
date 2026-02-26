#include "MainWindow.h"
#include "SudokuGridView.h"
#include "../Controller/SudokuController.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Sudoku Assistant"));
    resize(600, 650);

    // Controller
    m_controller = new SudokuController(this);

    // Central widget
    m_gridView = new SudokuGridView(m_controller->model(), this);
    setCentralWidget(m_gridView);

    // Connect grid → controller
    connect(m_gridView, &SudokuGridView::cellValueSelected,
            m_controller, &SudokuController::setCellValue);

    setupMenus();
    setupToolbar();
    setupStatusBar();

    // Load default grid on startup
    m_controller->loadDefaultGrid();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenus()
{
    // File menu
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

    // Edit menu
    QMenu *editMenu = menuBar()->addMenu(tr("&Édition"));

    QAction *undoAction = editMenu->addAction(tr("&Annuler"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);

    QAction *redoAction = editMenu->addAction(tr("&Rétablir"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    // Help menu
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
    statusBar()->showMessage(tr("Prêt — Chargez une grille pour commencer."));
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

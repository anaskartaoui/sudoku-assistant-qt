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
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Sudoku Assistant"));
    resize(1280, 800);

    setStyleSheet("QMainWindow { background-color: #DDE6F0; }");

    m_controller = new SudokuController(this);

    setupCentralWidget();
    setupMenus();
    setupToolbar();
    setupStatusBar();

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
    central->setStyleSheet("background-color: #DDE6F0;");

    QVBoxLayout *vLayout = new QVBoxLayout(central);
    vLayout->setContentsMargins(0, 20, 0, 20);
    vLayout->setSpacing(16);
    vLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    setupDifficultyBar();
    m_gridView = new SudokuGridView(m_controller->model(), this);
    m_numPad   = new NumPad(this);

    vLayout->addWidget(m_difficultyBar, 0, Qt::AlignHCenter);
    vLayout->addWidget(m_gridView,      0, Qt::AlignHCenter);
    vLayout->addWidget(m_numPad,        0, Qt::AlignHCenter);

    central->setLayout(vLayout);
    setCentralWidget(central);
}

void MainWindow::setupDifficultyBar()
{
    m_difficultyBar = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(m_difficultyBar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QButtonGroup *group = new QButtonGroup(this);

    QString btnStyle =
        "QPushButton {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: 1px solid #B0BEC5;"
        "  border-radius: 14px;"
        "  padding: 6px 20px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #C8D8F0; }"
        "QPushButton:checked {"
        "  background-color: #7A9AB5;"
        "  color: #FFFFFF;"
        "  border: 1px solid #7A9AB5;"
        "}";

    QStringList levels = { tr("Facile"), tr("Moyen"), tr("Difficile"), tr("Insane") };
    QStringList keys   = { "Easy",       "Medium",    "Hard",          "Insane"     };

    for (int i = 0; i < levels.size(); ++i) {
        QPushButton *btn = new QPushButton(levels[i], m_difficultyBar);
        btn->setStyleSheet(btnStyle);
        btn->setCheckable(true);
        btn->setProperty("difficulty", keys[i]);
        if (i == 0) btn->setChecked(true);
        group->addButton(btn);
        layout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this, keys, i]() {
            onDifficultySelected(keys[i]);
        });
    }

    m_difficultyBar->setLayout(layout);
}

void MainWindow::onDifficultySelected(const QString &difficulty)
{
    m_controller->loadRandomGrid(difficulty);
    m_gridView->resetView();
    statusBar()->showMessage(tr("Nouvelle grille chargée : %1").arg(difficulty));
}

void MainWindow::setupMenus()
{
    menuBar()->setStyleSheet(
        "QMenuBar { background-color: #DDE6F0; color: #2C3E50; }"
        "QMenuBar::item:selected { background-color: #B0C4D8; }"
        "QMenu { background-color: #FFFFFF; color: #2C3E50; border: 1px solid #B0BEC5; }"
        "QMenu::item:selected { background-color: #DDE6F0; }"
        );

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
    toolbar->setStyleSheet("QToolBar { background-color: #DDE6F0; border: none; }");
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Prêt — Sélectionnez une case puis un chiffre."));
    statusBar()->setStyleSheet(
        "QStatusBar { background-color: #DDE6F0; color: #2C3E50; font-size: 13px; padding: 4px; }"
        );
}

void MainWindow::onNewGrid()
{
    m_controller->loadDefaultGrid();
    m_gridView->resetView();
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
        m_gridView->resetView();
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

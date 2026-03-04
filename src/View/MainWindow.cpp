#include "MainWindow.h"
#include "SudokuGridView.h"
#include "NumPad.h"
#include "HelpDialog.h"
#include "VictoryDialog.h"
#include "../Controller/SudokuController.h"
#include "../Model/SudokuModel.h"

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
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_seconds(0), m_paused(false)
{
    setWindowTitle(tr("Sudoku Assistant"));
    resize(1280, 800);

    setStyleSheet(
        "QMainWindow { background-color: #DDE6F0; }"
        "QToolTip {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: solid #B0BEC5;"
        "  border-radius: 6px;"
        "  padding: 4px 8px;"
        "  font-family: Nunito;"
        "  font-size: 12px;"
        "}"
        );

    m_controller = new SudokuController(this);
    m_timer      = new QTimer(this);

    setupCentralWidget();
    setupMenus();
    setupToolbar();
    setupStatusBar();

    connect(m_gridView, &SudokuGridView::valueChanged,
            m_controller, &SudokuController::setCellValue);
    connect(m_numPad, &NumPad::numberClicked,
            this, &MainWindow::onNumberClicked);
    connect(m_timer, &QTimer::timeout,
            this, &MainWindow::onTimerTick);
    connect(m_controller->model(), &SudokuModel::gridSolved,
            this, &MainWindow::onGridSolved);

    m_controller->loadDefaultGrid();
    resetTimer();
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
    setupTimerBar();
    m_numPad = new NumPad(this);

    vLayout->addWidget(m_difficultyBar, 0, Qt::AlignHCenter);
    vLayout->addWidget(m_gridView,      0, Qt::AlignHCenter);
    vLayout->addWidget(m_timerBar,      0, Qt::AlignHCenter);
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

    QStringList levels   = { tr("Facile"), tr("Moyen"), tr("Difficile"), tr("Insane") };
    QStringList keys     = { "Easy",       "Medium",    "Hard",          "Insane"     };
    QStringList tooltips = {
        tr("Grilles simples, idéales pour débuter"),
        tr("Grilles de difficulté intermédiaire"),
        tr("Grilles complexes pour joueurs expérimentés"),
        tr("Grilles extrêmement difficiles")
    };

    for (int i = 0; i < levels.size(); ++i) {
        QPushButton *btn = new QPushButton(levels[i], m_difficultyBar);
        btn->setStyleSheet(btnStyle);
        btn->setCheckable(true);
        btn->setProperty("difficulty", keys[i]);
        btn->setToolTip(tooltips[i]);
        if (i == 0) btn->setChecked(true);
        group->addButton(btn);
        layout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this, keys, i]() {
            onDifficultySelected(keys[i]);
        });
    }

    m_difficultyBar->setLayout(layout);
}

void MainWindow::setupTimerBar()
{
    m_timerBar = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(m_timerBar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    m_timerLabel = new QLabel("00:00", m_timerBar);
    m_timerLabel->setStyleSheet(
        "QLabel {"
        "  color: #2C3E50;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        );

    m_pauseBtn = new QPushButton("⏸", m_timerBar);
    m_pauseBtn->setFixedSize(32, 32);
    m_pauseBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: 1px solid #B0BEC5;"
        "  border-radius: 16px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover { background-color: #C8D8F0; }"
        );
    m_pauseBtn->setToolTip(tr("Mettre le jeu en pause"));

    connect(m_pauseBtn, &QPushButton::clicked, this, &MainWindow::onPauseClicked);

    layout->addWidget(m_timerLabel);
    layout->addWidget(m_pauseBtn);
    m_timerBar->setLayout(layout);

    QPushButton *restartBtn = new QPushButton("↺", m_timerBar);
    restartBtn->setFixedSize(32, 32);
    restartBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: 1px solid #B0BEC5;"
        "  border-radius: 16px;"
        "  font-size: 16px;"
        "}"
        "QPushButton:hover { background-color: #C8D8F0; }"
        );
    restartBtn->setToolTip(tr("Recommencer la partie depuis le début"));
    connect(restartBtn, &QPushButton::clicked, this, &MainWindow::onNewGrid);

    layout->addWidget(m_timerLabel);
    layout->addWidget(m_pauseBtn);
    layout->addWidget(restartBtn);
}

void MainWindow::resetTimer()
{
    m_timer->stop();
    m_seconds = 0;
    m_paused  = false;
    m_timerLabel->setText("00:00");
    m_pauseBtn->setText("⏸");
    m_timer->start(1000);
}

void MainWindow::onTimerTick()
{
    ++m_seconds;
    int m = m_seconds / 60;
    int s = m_seconds % 60;
    m_timerLabel->setText(QString("%1:%2")
                              .arg(m, 2, 10, QChar('0'))
                              .arg(s, 2, 10, QChar('0')));
}

void MainWindow::onPauseClicked()
{
    m_paused = !m_paused;
    if (m_paused) {
        m_timer->stop();
        m_pauseBtn->setText("▶");
        m_gridView->setPaused(true);
    } else {
        m_timer->start(1000);
        m_pauseBtn->setText("⏸");
        m_gridView->setPaused(false);
    }
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
    quitAction->setToolTip(tr("Quitter l'application"));
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu(tr("&Édition"));

    QAction *undoAction = editMenu->addAction(tr("&Annuler"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setToolTip(tr("Annuler la dernière action"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);

    QAction *redoAction = editMenu->addAction(tr("&Rétablir"));
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setToolTip(tr("Rétablir la dernière action annulée"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Aide"));

    QAction *hintsAction = helpMenu->addAction(tr("Activer/désactiver les &indices"));
    hintsAction->setShortcut(tr("Ctrl+H"));
    hintsAction->setCheckable(true);
    hintsAction->setChecked(true);
    hintsAction->setToolTip(tr("Afficher ou masquer les indices de cases"));
    connect(hintsAction, &QAction::triggered, this, &MainWindow::onToggleHints);

    QAction *helpAction = helpMenu->addAction(tr("&Guide du joueur"));
    helpAction->setShortcut(tr("F1"));
    helpAction->setToolTip(tr("Afficher l'aide"));
    connect(helpAction, &QAction::triggered, this, &MainWindow::onShowHelp);
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
    resetTimer();
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
        resetTimer();
        statusBar()->showMessage(tr("Grille chargée depuis %1").arg(path));
    }
}

void MainWindow::onDifficultySelected(const QString &difficulty)
{
    m_controller->loadRandomGrid(difficulty);
    m_gridView->resetView();
    resetTimer();
    statusBar()->showMessage(tr("Nouvelle grille chargée : %1").arg(difficulty));
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

void MainWindow::onShowHelp()
{
    HelpDialog dialog(this);
    dialog.exec();
}

void MainWindow::onGridSolved()
{
    m_timer->stop();
    m_paused = true;
    m_pauseBtn->setText("▶");

    VictoryDialog dialog(m_seconds, this);
    connect(&dialog, &VictoryDialog::newGameRequested, this, &MainWindow::onNewGrid);
    dialog.exec();
}

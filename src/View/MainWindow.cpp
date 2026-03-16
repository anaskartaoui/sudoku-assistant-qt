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
#include <QTranslator>
#include <QCoreApplication>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_seconds(0), m_paused(false), m_currentLang("fr")
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
    connect(m_gridView, &SudokuGridView::pauseToggled,
            this, &MainWindow::onPauseClicked);

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

    QStringList keys = { "Easy", "Medium", "Hard", "Insane" };

    for (int i = 0; i < keys.size(); ++i) {
        QPushButton *btn = new QPushButton(m_difficultyBar);
        btn->setStyleSheet(btnStyle);
        btn->setCheckable(true);
        btn->setProperty("difficulty", keys[i]);
        if (i == 0) btn->setChecked(true);
        group->addButton(btn);
        layout->addWidget(btn);
        m_difficultyButtons.append(btn);
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
    connect(m_pauseBtn, &QPushButton::clicked, this, &MainWindow::onPauseClicked);

    m_restartBtn = new QPushButton("↺", m_timerBar);
    m_restartBtn->setFixedSize(32, 32);
    m_restartBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: 1px solid #B0BEC5;"
        "  border-radius: 16px;"
        "  font-size: 16px;"
        "}"
        "QPushButton:hover { background-color: #C8D8F0; }"
        );
    connect(m_restartBtn, &QPushButton::clicked, this, &MainWindow::onNewGrid);

    QString roundBtnStyle =
        "QPushButton {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: 1px solid #B0BEC5;"
        "  border-radius: 16px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #C8D8F0; }"
        "QPushButton:checked {"
        "  background-color: #7A9AB5;"
        "  color: #FFFFFF;"
        "  border: 1px solid #7A9AB5;"
        "}";

    m_hintsBtn = new QPushButton("H", m_timerBar);
    m_hintsBtn->setFixedSize(32, 32);
    m_hintsBtn->setCheckable(true);
    m_hintsBtn->setChecked(true);
    m_hintsBtn->setStyleSheet(roundBtnStyle);
    connect(m_hintsBtn, &QPushButton::clicked, this, &MainWindow::onToggleHints);

    m_helpBtn = new QPushButton("?", m_timerBar);
    m_helpBtn->setFixedSize(32, 32);
    m_helpBtn->setStyleSheet(roundBtnStyle);
    connect(m_helpBtn, &QPushButton::clicked, this, &MainWindow::onShowHelp);

    m_langBtn = new QPushButton("FR", m_timerBar);
    m_langBtn->setFixedSize(32, 32);
    m_langBtn->setStyleSheet(roundBtnStyle);
    connect(m_langBtn, &QPushButton::clicked, this, [this]() {
        onLanguageChanged(m_currentLang == "fr" ? "en" : "fr");
    });

    layout->addWidget(m_timerLabel);
    layout->addWidget(m_pauseBtn);
    layout->addWidget(m_restartBtn);
    layout->addWidget(m_hintsBtn);
    layout->addWidget(m_helpBtn);
    layout->addWidget(m_langBtn);
    m_timerBar->setLayout(layout);
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

    m_fileMenu = menuBar()->addMenu(QString());

    m_newAction = m_fileMenu->addAction(QString());
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::onNewGrid);

    m_loadAction = m_fileMenu->addAction(QString());
    m_loadAction->setShortcut(QKeySequence::Open);
    connect(m_loadAction, &QAction::triggered, this, &MainWindow::onLoadGrid);

    m_fileMenu->addSeparator();

    m_quitAction = m_fileMenu->addAction(QString());
    m_quitAction->setShortcut(QKeySequence::Quit);
    m_quitAction->setMenuRole(QAction::NoRole);
    connect(m_quitAction, &QAction::triggered, this, &QWidget::close);

    m_editMenu = menuBar()->addMenu(QString());

    m_undoAction = m_editMenu->addAction(QString());
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::onUndo);

    m_redoAction = m_editMenu->addAction(QString());
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    m_helpMenu = menuBar()->addMenu(QString());

    m_hintsAction = m_helpMenu->addAction(QString());
    m_hintsAction->setShortcut(tr("Ctrl+H"));
    m_hintsAction->setCheckable(true);
    m_hintsAction->setChecked(true);
    connect(m_hintsAction, &QAction::triggered, this, &MainWindow::onToggleHints);

    m_helpAction = m_helpMenu->addAction(QString());
    m_helpAction->setShortcut(tr("F1"));
    connect(m_helpAction, &QAction::triggered, this, &MainWindow::onShowHelp);

    m_helpMenu->addSeparator();

    m_langMenu = m_helpMenu->addMenu(QString());

    QAction *frAction = m_langMenu->addAction("Français");
    connect(frAction, &QAction::triggered, this, [this]() { onLanguageChanged("fr"); });

    QAction *enAction = m_langMenu->addAction("English");
    connect(enAction, &QAction::triggered, this, [this]() { onLanguageChanged("en"); });

    retranslateUi();
}

void MainWindow::setupToolbar()
{
    QToolBar *toolbar = addToolBar(tr("Barre d'outils"));
    toolbar->setMovable(false);
    toolbar->setStyleSheet("QToolBar { background-color: #DDE6F0; border: none; }");
}

void MainWindow::setupStatusBar()
{
    statusBar()->setStyleSheet(
        "QStatusBar { background-color: #DDE6F0; color: #2C3E50; font-size: 13px; padding: 4px; }"
        );
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Sudoku Assistant"));

    // Menus
    m_fileMenu->setTitle(tr("&Fichier"));
    m_editMenu->setTitle(tr("&Édition"));
    m_helpMenu->setTitle(tr("&Aide"));
    m_langMenu->setTitle(tr("&Langue"));

    // File actions
    m_newAction->setText(tr("&Nouvelle grille"));
    m_newAction->setToolTip(tr("Charger une nouvelle grille"));
    m_loadAction->setText(tr("&Ouvrir grille..."));
    m_loadAction->setToolTip(tr("Ouvrir une grille depuis un fichier"));
    m_quitAction->setText(tr("&Quitter"));
    m_quitAction->setToolTip(tr("Quitter l'application"));

    // Edit actions
    m_undoAction->setText(tr("&Annuler"));
    m_undoAction->setToolTip(tr("Annuler la dernière action"));
    m_redoAction->setText(tr("&Rétablir"));
    m_redoAction->setToolTip(tr("Rétablir la dernière action annulée"));

    // Help actions
    m_hintsAction->setText(tr("Activer/désactiver les &indices"));
    m_hintsAction->setToolTip(tr("Afficher ou masquer les indices de cases"));
    m_helpAction->setText(tr("&Guide du joueur"));
    m_helpAction->setToolTip(tr("Afficher l'aide"));

    // Difficulty buttons
    const QStringList labels = { tr("Facile"), tr("Moyen"), tr("Difficile"), tr("Insane") };
    const QStringList tips   = {
        tr("Grilles simples, idéales pour débuter"),
        tr("Grilles de difficulté intermédiaire"),
        tr("Grilles complexes pour joueurs expérimentés"),
        tr("Grilles extrêmement difficiles")
    };
    for (int i = 0; i < m_difficultyButtons.size(); ++i) {
        m_difficultyButtons[i]->setText(labels[i]);
        m_difficultyButtons[i]->setToolTip(tips[i]);
    }

    // Timer bar
    m_pauseBtn->setToolTip(tr("Mettre le jeu en pause"));
    m_restartBtn->setToolTip(tr("Recommencer la partie depuis le début"));
    m_hintsBtn->setToolTip(tr("Activer/désactiver les indices"));
    m_helpBtn->setToolTip(tr("Afficher l'aide"));
    m_langBtn->setText(m_currentLang.toUpper());
    m_langBtn->setToolTip(tr("Changer de langue"));

    // NumPad
    m_numPad->retranslateUi();

    // Status bar
    statusBar()->showMessage(tr("Prêt — Sélectionnez une case puis un chiffre."));
}

void MainWindow::onLanguageChanged(const QString &lang)
{
    m_currentLang = lang;
    static QTranslator translator;
    QCoreApplication::removeTranslator(&translator);
    translator.load(":/i18n/sudoku_" + lang + ".qm");
    QCoreApplication::installTranslator(&translator);
    retranslateUi();
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
    bool hintsOn = m_controller->model()->hintsEnabled();
    m_hintsBtn->setChecked(hintsOn);
    m_hintsAction->setChecked(hintsOn);
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

    VictoryDialog *dialog = new VictoryDialog(m_seconds, this);
    connect(dialog, &VictoryDialog::newGameRequested, this,  &MainWindow::onNewGrid,   Qt::QueuedConnection);
    connect(dialog, &VictoryDialog::newGameRequested, dialog, &VictoryDialog::accept);
    dialog->exec();
    dialog->deleteLater();
}

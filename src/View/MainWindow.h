#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QList>
class SudokuController;
class SudokuGridView;
class NumPad;
class QMenu;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void retranslateUi();
private slots:
    void onNewGrid();
    void onLoadGrid();
    void onUndo();
    void onRedo();
    void onToggleHints();
    void onNumberClicked(int value);
    void onDifficultySelected(const QString &difficulty);
    void onTimerTick();
    void onPauseClicked();
    void onShowHelp();
    void onGridSolved();
    void onLanguageChanged(const QString &lang);
private:
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupDifficultyBar();
    void setupTimerBar();
    void resetTimer();

    // Central widgets
    SudokuGridView      *m_gridView;
    SudokuController    *m_controller;
    NumPad              *m_numPad;
    QWidget             *m_difficultyBar;
    QWidget             *m_timerBar;
    QLabel              *m_timerLabel;
    QPushButton         *m_pauseBtn;
    QPushButton         *m_restartBtn;
    QTimer              *m_timer;
    int                  m_seconds;
    bool                 m_paused;

    // Difficulty buttons (for retranslation)
    QList<QPushButton*>  m_difficultyButtons;

    // Menus
    QMenu   *m_fileMenu;
    QMenu   *m_editMenu;
    QMenu   *m_helpMenu;
    QMenu   *m_langMenu;

    // Actions
    QAction *m_newAction;
    QAction *m_loadAction;
    QAction *m_quitAction;
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_hintsAction;
    QAction *m_helpAction;
};
#endif // MAINWINDOW_H

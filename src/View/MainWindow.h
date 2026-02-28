#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
class SudokuController;
class SudokuGridView;
class NumPad;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onNewGrid();
    void onLoadGrid();
    void onUndo();
    void onRedo();
    void onToggleHints();
    void onNumberClicked(int value);
    void onDifficultySelected(const QString &difficulty);
private:
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupDifficultyBar();
    SudokuGridView   *m_gridView;
    SudokuController *m_controller;
    NumPad           *m_numPad;
    QWidget *m_difficultyBar;
};
#endif // MAINWINDOW_H

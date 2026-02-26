#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SudokuController;
class SudokuGridView;
class NumPad;

/**
 * @brief The MainWindow class
 * Top-level window: menus, toolbar, status bar, grid and numpad.
 */
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

private:
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();
    void setupCentralWidget();

    SudokuGridView   *m_gridView;
    SudokuController *m_controller;
    NumPad           *m_numPad;
};

#endif // MAINWINDOW_H

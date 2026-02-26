#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SudokuController;
class SudokuGridView;

/**
 * @brief The MainWindow class
 * Top-level window: menus, toolbar, status bar, and central grid widget.
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

private:
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();

    SudokuGridView   *m_gridView;
    SudokuController *m_controller;
};

#endif // MAINWINDOW_H

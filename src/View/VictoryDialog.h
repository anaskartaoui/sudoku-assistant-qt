#ifndef VICTORYDIALOG_H
#define VICTORYDIALOG_H
#include <QDialog>

class VictoryDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VictoryDialog(int seconds, QWidget *parent = nullptr);

signals:
    void newGameRequested();
};
#endif // VICTORYDIALOG_H

#ifndef NUMPAD_H
#define NUMPAD_H

#include <QWidget>
#include <QPushButton>

/**
 * @brief The NumPad class
 * A numeric keypad (1-9 + clear button) displayed below the grid.
 * Emits a signal when a number is clicked.
 */
class NumPad : public QWidget
{
    Q_OBJECT

public:
    explicit NumPad(QWidget *parent = nullptr);

signals:
    void numberClicked(int value);  // 0 = clear

private:
    void setupButtons();
    QPushButton* createButton(const QString &text, int value);
};

#endif // NUMPAD_H

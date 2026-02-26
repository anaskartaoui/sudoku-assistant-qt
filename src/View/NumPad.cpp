#include "NumPad.h"
#include <QGridLayout>

NumPad::NumPad(QWidget *parent)
    : QWidget(parent)
{
    setupButtons();
}

void NumPad::setupButtons()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(20, 10, 20, 10);

    // Boutons 1-9
    int nums[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; ++i) {
        QPushButton *btn = createButton(QString::number(nums[i]), nums[i]);
        layout->addWidget(btn, i / 3, i % 3);
    }

    // Bouton effacer
    QPushButton *clearBtn = createButton("✕", 0);
    clearBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #c0392b;"
        "  color: white;"
        "  border-radius: 25px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e74c3c; }"
        "QPushButton:pressed { background-color: #a93226; }"
        );
    layout->addWidget(clearBtn, 3, 1);

    setLayout(layout);
}

QPushButton* NumPad::createButton(const QString &text, int value)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setFixedSize(50, 50);
    btn->setStyleSheet(
        "QPushButton {"
        "  background-color: #1f5f5f;"
        "  color: #FDF8E2;"
        "  border-radius: 25px;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  border: none;"
        "}"
        "QPushButton:hover { background-color: #DD7D59; }"
        "QPushButton:pressed { background-color: #b56344; }"
        );

    connect(btn, &QPushButton::clicked, this, [this, value]() {
        emit numberClicked(value);
    });

    return btn;
}

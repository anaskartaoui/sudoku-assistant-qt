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
    layout->setSpacing(10);
    layout->setContentsMargins(20, 10, 20, 10);

    for (int i = 1; i <= 5; ++i) {
        QPushButton *btn = createButton(QString::number(i), i);
        layout->addWidget(btn, 0, i - 1);
    }

    for (int i = 6; i <= 9; ++i) {
        QPushButton *btn = createButton(QString::number(i), i);
        layout->addWidget(btn, 1, i - 6);
    }

    QPushButton *clearBtn = createButton("✕", 0);
    clearBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FADBD8;"
        "  color: #C0392B;"
        "  border-radius: 25px;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  border: 1px solid #E8B4B0;"
        "}"
        "QPushButton:hover { background-color: #F1948A; color: #FFFFFF; }"
        "QPushButton:pressed { background-color: #C0392B; color: #FFFFFF; }"
        );
    layout->addWidget(clearBtn, 1, 4);

    setLayout(layout);
}

QPushButton* NumPad::createButton(const QString &text, int value)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setFixedSize(50, 50);
    btn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border-radius: 25px;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  border: 1px solid #B0BEC5;"
        "}"
        "QPushButton:hover { background-color: #C8D8F0; color: #1A2A3A; }"
        "QPushButton:pressed { background-color: #5B8CCC; color: #FFFFFF; }"
        );
    connect(btn, &QPushButton::clicked, this, [this, value]() {
        emit numberClicked(value);
    });
    return btn;
}

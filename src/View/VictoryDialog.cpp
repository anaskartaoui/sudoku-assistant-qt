#include "VictoryDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

VictoryDialog::VictoryDialog(int seconds, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Félicitations !"));
    setFixedSize(380, 240);
    setStyleSheet(
        "QDialog { background-color: #DDE6F0; }"
        "QLabel { color: #2C3E50; background-color: transparent; }"
        "QPushButton {"
        "  background-color: #7A9AB5;"
        "  color: #FFFFFF;"
        "  border-radius: 10px;"
        "  padding: 8px 24px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "  border: none;"
        "}"
        "QPushButton:hover { background-color: #6A89A7; }"
        "QPushButton#closeBtn {"
        "  background-color: #FFFFFF;"
        "  color: #2C3E50;"
        "  border: 1px solid #B0BEC5;"
        "}"
        "QPushButton#closeBtn:hover { background-color: #C8D8F0; }"
        );

    QVBoxLayout *main = new QVBoxLayout(this);
    main->setContentsMargins(30, 30, 30, 30);
    main->setSpacing(12);

    QLabel *title = new QLabel(tr("Félicitations !"), this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50; background: transparent;");
    title->setAlignment(Qt::AlignCenter);
    main->addWidget(title);

    QLabel *msg = new QLabel(tr("Vous avez résolu la grille !"), this);
    msg->setStyleSheet("font-size: 14px; color: #2C3E50; background: transparent;");
    msg->setAlignment(Qt::AlignCenter);
    main->addWidget(msg);

    int m = seconds / 60;
    int s = seconds % 60;
    QString timeStr = QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
    QLabel *timeLabel = new QLabel(tr("Temps : %1").arg(timeStr), this);
    timeLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #7A9AB5; background: transparent;");
    timeLabel->setAlignment(Qt::AlignCenter);
    main->addWidget(timeLabel);

    main->addStretch();

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(12);

    QPushButton *newGameBtn = new QPushButton(tr("Nouvelle partie"), this);
    connect(newGameBtn, &QPushButton::clicked, this, [this]() {
        emit newGameRequested();
        accept();
    });

    QPushButton *closeBtn = new QPushButton(tr("Fermer"), this);
    closeBtn->setObjectName("closeBtn");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    btnLayout->addWidget(newGameBtn);
    btnLayout->addWidget(closeBtn);
    main->addLayout(btnLayout);

    setLayout(main);
}

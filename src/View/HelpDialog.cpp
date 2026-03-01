#include "HelpDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Aide — Sudoku Assistant"));
    setFixedSize(580, 680);
    setStyleSheet(
        "QDialog { background-color: #DDE6F0; }"
        "QLabel { color: #2C3E50; font-size: 13px; background-color: transparent; }"
        "QLabel#keyLabel {"
        "  background-color: #FFFFFF;"
        "  border: 1px solid #B0BEC5;"
        "  border-radius: 6px;"
        "  padding: 4px 8px;"
        "  font-size: 13px;"
        "  color: #2C3E50;"
        "}"
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
        );

    QVBoxLayout *main = new QVBoxLayout(this);
    main->setContentsMargins(30, 30, 30, 30);
    main->setSpacing(10);

    // Titre
    QLabel *title = new QLabel(tr("Guide du joueur"), this);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C3E50; background: transparent;");
    main->addWidget(title);
    main->addSpacing(6);

    // Section couleurs
    QLabel *colorTitle = new QLabel(tr("Signification des couleurs"), this);
    colorTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #2C3E50; background: transparent;");
    main->addWidget(colorTitle);

    auto addColorRow = [&](const QString &color, const QString &desc) {
        QWidget *row = new QWidget(this);
        row->setFixedHeight(34);
        row->setAttribute(Qt::WA_TranslucentBackground);
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 2, 0, 2);
        hl->setSpacing(14);

        QLabel *swatch = new QLabel(row);
        swatch->setFixedSize(28, 28);
        swatch->setStyleSheet(QString(
                                  "background-color: %1;"
                                  "border-radius: 6px;"
                                  "border: 1px solid #B0BEC5;"
                                  ).arg(color));

        QLabel *text = new QLabel(desc, row);
        text->setStyleSheet("font-size: 13px; color: #2C3E50; background: transparent;");

        hl->addWidget(swatch);
        hl->addWidget(text, 1);
        main->addWidget(row);
    };

    addColorRow("#EAF0F6", tr("Case fixe — chiffre donné par la grille, non modifiable"));
    addColorRow("#FFFFFF",  tr("Case vide — à remplir par le joueur"));
    addColorRow("#B8D4ED", tr("Case sélectionnée — case active"));
    addColorRow("#FEF9E7", tr("Case surlignée — même ligne, colonne ou bloc"));
    addColorRow("#FADBD8", tr("Contradiction — chiffre en conflit avec la règle Sudoku"));
    addColorRow("#D5F5E3", tr("Naked single — une seule valeur possible dans cette case"));

    main->addSpacing(8);

    // Séparateur
    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("background-color: #B0BEC5;");
    sep->setFixedHeight(1);
    main->addWidget(sep);
    main->addSpacing(8);

    // Section raccourcis
    QLabel *shortcutTitle = new QLabel(tr("Raccourcis clavier"), this);
    shortcutTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #2C3E50; background: transparent;");
    main->addWidget(shortcutTitle);

    auto addShortcut = [&](const QString &key, const QString &desc) {
        QWidget *row = new QWidget(this);
        row->setFixedHeight(38);
        row->setAttribute(Qt::WA_TranslucentBackground);
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(0, 2, 0, 2);
        hl->setSpacing(20);

        QLabel *keyLabel = new QLabel(key, row);
        keyLabel->setObjectName("keyLabel");
        keyLabel->setFixedWidth(180);
        keyLabel->setAlignment(Qt::AlignCenter);

        QLabel *descLabel = new QLabel(desc, row);
        descLabel->setStyleSheet("font-size: 13px; color: #2C3E50; background: transparent;");

        hl->addWidget(keyLabel);
        hl->addWidget(descLabel, 1);
        main->addWidget(row);
    };

    addShortcut("Fleches directionnelles", tr("Naviguer entre les cases"));
    addShortcut("1 - 9",                   tr("Entrer un chiffre dans la case"));
    addShortcut("Suppr / Backspace",       tr("Effacer la case sélectionnée"));
    addShortcut("Echap",                   tr("Désélectionner la case"));
    addShortcut("Ctrl+Z",                  tr("Annuler la dernière action"));
    addShortcut("Ctrl+Y",                  tr("Rétablir"));
    addShortcut("Ctrl+N",                  tr("Nouvelle grille"));
    addShortcut("Ctrl+H",                  tr("Activer/désactiver les indices"));

    main->addStretch();

    // Bouton fermer
    QPushButton *closeBtn = new QPushButton(tr("Fermer"), this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    main->addWidget(closeBtn, 0, Qt::AlignRight);

    setLayout(main);
}

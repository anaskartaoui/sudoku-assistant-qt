# Sudoku Assistant

Application d'assistance au Sudoku developpee en C++17 avec le framework Qt6, suivant le patron architectural MVP (Modele-Vue-Presentateur). Elle offre une interface graphique complete permettant de jouer, d'analyser et de resoudre des grilles de Sudoku avec des aides visuelles en temps reel.

---

## Fonctionnalites

- **Chargement de grilles par difficulte** : quatre niveaux disponibles (Facile, Moyen, Difficile, Insane), selectionnables depuis la barre de difficulte ou le menu Fichier.
- **Chargement depuis un fichier** : ouverture de grilles personnalisees au format texte via le menu Fichier > Ouvrir grille.
- **Selection de cellule** : clic souris ou navigation clavier ; la ligne, la colonne et le bloc de la cellule selectionnee sont automatiquement surlignees.
- **Crayon (pencil marks)** : affichage des candidats possibles dans chaque cellule vide, mis a jour dynamiquement a chaque saisie.
- **Detection des contradictions** : les cellules en conflit avec les regles du Sudoku sont signalees en rouge immediatement.
- **Detection des naked singles** : les cellules n'ayant qu'une seule valeur possible sont mises en evidence en vert.
- **Annulation / Retablissement** : historique complet des actions avec Ctrl+Z et Ctrl+Y.
- **Chronometre** : compteur de temps affiche en MM:SS, avec bouton de pause et bouton de redemarrage.
- **Pause** : superposition visuelle masquant la grille, activable depuis le bouton dedie ou la touche Espace.
- **Navigation clavier** : deplacement entre les cellules avec les touches directionnelles, saisie des chiffres 1 a 9, suppression, echappement et espace pour la pause.
- **Detection de la victoire** : lorsque la grille est resolue, une boite de dialogue affiche les felicitations et le temps ecoule, avec les options Nouvelle partie et Fermer.
- **Aide integree** : dialogue complet avec legende des couleurs et tableau des raccourcis clavier, accessible via F1 ou le menu Aide.
- **Changement de langue FR/EN sans redemarrage** : le menu Aide > Langue permet de basculer instantanement entre le francais et l'anglais grace a `retranslateUi()`.
- **Police Nunito** : interface typographiee avec la police Nunito, embarquee dans les ressources de l'application.

---

## Architecture

Le projet suit le patron **MVP (Modele-Vue-Presentateur)**, une variante du MVC adaptee aux interfaces graphiques Qt. La separation des responsabilites est stricte : le Modele ne connait pas la Vue, et la Vue ne manipule pas directement les donnees.

### Modele

Le Modele encapsule l'etat complet du jeu et la logique metier.

- **`SudokuModel`** : contient la grille 9x9, les marqueurs de cellules fixes, et les ensembles de candidats pour chaque cellule. Il expose des methodes de lecture et d'ecriture (`getValue`, `setValue`, `isFixed`, `getCandidates`) et emet des signaux Qt (`cellChanged`, `candidatesUpdated`, `contradictionDetected`, `gridSolved`) pour notifier la Vue de tout changement d'etat.
- **`SudokuSolver`** : implemente un algorithme de resolution par backtracking avec validation. Il est utilise par le Controleur pour verifier la validite des grilles chargees et pour alimenter la detection des naked singles.

### Vue

La Vue est composee de widgets Qt independants qui affichent l'etat du Modele et transmettent les interactions utilisateur au Controleur.

- **`MainWindow`** : fenetre principale. Elle orchestre la mise en page, les menus, la barre de difficulte, le chronometre et les connexions entre les composants. Elle implemente `retranslateUi()` pour le changement de langue dynamique.
- **`SudokuGridView`** : grille 9x9 composee de `CellWidget`. Gere la selection, le surlignage, l'overlay de pause et les evenements clavier.
- **`CellWidget`** : represente une cellule individuelle. Affiche la valeur ou les candidats selon l'etat, et adopte differents styles visuels (fixe, selectionne, surligne, contradiction, naked single).
- **`NumPad`** : pave numerique virtuel permettant la saisie a la souris.
- **`HelpDialog`** : dialogue d'aide avec legende des couleurs et tableau des raccourcis clavier.
- **`VictoryDialog`** : dialogue de victoire affichant le temps ecoule et les options de fin de partie.

### Controleur

- **`SudokuController`** : intermediaire entre la Vue et le Modele. Il recoit les actions utilisateur (saisie d'une valeur, chargement d'une grille, annulation), met a jour le Modele en consequence, et maintient les piles d'annulation et de retablissement (`QStack`).

### Patron Observateur via signaux et slots Qt

La communication entre les couches repose exclusivement sur le mecanisme de signaux et slots de Qt, qui implemente le patron Observateur de maniere decouple :

| Signal emis par | Signal | Connecte a |
|---|---|---|
| `SudokuModel` | `cellChanged(row, col)` | `SudokuGridView` rafraichit la cellule |
| `SudokuModel` | `candidatesUpdated()` | `SudokuGridView` met a jour la grille |
| `SudokuModel` | `contradictionDetected(row, col)` | `SudokuGridView` marque la cellule en erreur |
| `SudokuModel` | `gridSolved()` | `MainWindow` arrete le chrono et ouvre `VictoryDialog` |
| `SudokuGridView` | `valueChanged(row, col, value)` | `SudokuController` ecrit dans le Modele |
| `SudokuGridView` | `pauseToggled()` | `MainWindow` bascule l'etat de pause |

---

## Structure du projet

```
sudoku-assistant-qt/
├── src/
│   ├── main.cpp
│   ├── Model/
│   │   ├── SudokuModel.h
│   │   ├── SudokuModel.cpp
│   │   ├── SudokuSolver.h
│   │   └── SudokuSolver.cpp
│   ├── View/
│   │   ├── MainWindow.h
│   │   ├── MainWindow.cpp
│   │   ├── SudokuGridView.h
│   │   ├── SudokuGridView.cpp
│   │   ├── CellWidget.h
│   │   ├── CellWidget.cpp
│   │   ├── NumPad.h
│   │   ├── NumPad.cpp
│   │   ├── HelpDialog.h
│   │   ├── HelpDialog.cpp
│   │   ├── VictoryDialog.h
│   │   └── VictoryDialog.cpp
│   └── Controller/
│       ├── SudokuController.h
│       └── SudokuController.cpp
├── resources/
│   ├── grilles/
│   │   ├── Easy.txt
│   │   ├── Medium.txt
│   │   ├── Hard.txt
│   │   └── Insane.txt
│   ├── fonts/
│   │   └── Nunito/
│   ├── i18n/
│   │   ├── sudoku_fr.ts
│   │   ├── sudoku_fr.qm
│   │   ├── sudoku_en.ts
│   │   └── sudoku_en.qm
│   ├── images/
│   │   └── pause.png
│   └── resources.qrc
├── sudoku-assistant.pro
└── README.md
```

---

## Prerequis

| Outil | Version minimale |
|---|---|
| Qt | 6.0 |
| Compilateur C++ | C++17 (GCC, Clang ou MSVC) |
| qmake | Fourni avec Qt6 |
| make | GNU Make ou equivalent |

Qt6 doit etre installe et le repertoire `bin` de Qt doit etre accessible dans le `PATH` systeme.

---

## Compilation et execution

```bash
# Cloner le depot
git clone <url-du-depot>
cd sudoku-assistant-qt

# Generer le Makefile
qmake sudoku-assistant.pro

# Compiler
make -j$(nproc)

# Lancer l'application
./build/sudoku-assistant
```

Pour recompiler les fichiers de traduction apres modification des sources :

```bash
lupdate sudoku-assistant.pro   # Extrait les chaines traduisibles vers les .ts
lrelease sudoku-assistant.pro  # Compile les .ts en fichiers binaires .qm
```

---

## Format des fichiers de grilles

Les grilles sont stockees dans des fichiers `.txt` selon le format suivant :

- La premiere ligne contient un entier indiquant le nombre de grilles dans le fichier.
- Chaque ligne suivante represente une grille complete sous forme d'une chaine de 81 caracteres.
- Les chiffres `1` a `9` correspondent aux cellules preremplies (cellules fixes).
- Le caractere `0` correspond a une cellule vide a completer par le joueur.
- Les cellules sont lues de gauche a droite, ligne par ligne, de la rangee 1 a la rangee 9.

Exemple pour un fichier contenant deux grilles :

```
2
530070000600195000098000060800060003400803001700020006060000280000419005000080079
800000000003600000070090200060005030004007000090010006002008000500090073000000981
```

---

## Raccourcis clavier

| Touche | Action |
|---|---|
| Fleches directionnelles | Naviguer entre les cellules |
| 1 -- 9 | Saisir un chiffre dans la cellule selectionnee |
| Suppr / Retour arriere | Effacer la cellule selectionnee |
| Echap | Deselectionner la cellule active |
| Espace | Mettre le jeu en pause / reprendre |
| Ctrl+Z | Annuler la derniere action |
| Ctrl+Y | Retablir la derniere action annulee |
| Ctrl+N | Charger une nouvelle grille |
| Ctrl+Q / Cmd+Q | Quitter l'application |
| F1 | Ouvrir le guide du joueur |
| Ctrl+H | Activer / desactiver les indices |

---

## Legende des couleurs

| Couleur | Etat de la cellule |
|---|---|
| Bleu clair (`#EAF0F6`) | Cellule fixe -- chiffre donne par la grille, non modifiable |
| Blanc (`#FFFFFF`) | Cellule vide -- a remplir par le joueur |
| Bleu moyen (`#B8D4ED`) | Cellule selectionnee -- cellule active |
| Jaune pale (`#FEF9E7`) | Cellule surlignee -- meme ligne, colonne ou bloc que la selection |
| Rouge pale (`#FADBD8`) | Contradiction -- chiffre en conflit avec les regles du Sudoku |
| Vert pale (`#D5F5E3`) | Naked single -- une seule valeur possible dans cette cellule |

---

## Contexte academique

Ce projet a ete realise dans le cadre d'un cours de genie logiciel et d'interfaces graphiques en deuxieme annee de master. Il a pour objectif de mettre en pratique les patrons de conception orientee objet (MVP, Observateur), la programmation en C++17, et le developpement d'interfaces graphiques avec le framework Qt6.

- **Auteur** : Anas Kartaoui
- **Etablissement** : Universite de Caen Normandie
- **Cours** : Interfaces graphiques et genie logiciel

# Sudoku Assistant

Application d'assistance au Sudoku developpee en C++17 avec le framework Qt6, suivant le patron architectural MVP (Modele-Vue-Presentateur). Elle offre une interface graphique complete permettant de jouer, d'analyser et de resoudre des grilles de Sudoku avec des aides visuelles en temps reel, un chronometre, un systeme d'annulation, et un support bilingue francais/anglais.

---

## Demo

https://github.com/user-attachments/assets/50759301-e5bc-4b9b-9944-008e9b7ce915

---

## Fonctionnalites

### Gestion des grilles

- **Chargement par difficulte** : quatre niveaux disponibles (Facile, Moyen, Difficile, Insane), chacun associe a un fichier de grilles embarque dans les ressources. Une grille est tiree aleatoirement dans le fichier correspondant a chaque nouvelle partie.
- **Chargement depuis un fichier** : ouverture de grilles personnalisees au format texte (`.txt`, `.sdk`) via le menu Fichier > Ouvrir grille ou le raccourci Ctrl+O.
- **Reinitialisation** : le bouton de redemarrage (↺) recharge une nouvelle grille de la meme difficulte et remet le chronometre a zero.

### Saisie et navigation

- **Selection de cellule** : clic souris ou touches directionnelles. La cellule selectionnee est mise en evidence en bleu, et toute la ligne, la colonne et le bloc 3x3 correspondants sont automatiquement surlignees en jaune pale.
- **Saisie de valeurs** : chiffres 1 a 9 via le clavier ou le pave numerique virtuel. Les cellules fixes (donnees initiales) sont protegees en ecriture.
- **Effacement** : touche Suppr ou Retour arriere pour vider la cellule selectionnee.
- **Deselection** : touche Echap pour deselectionner la cellule active et retirer le surlignage.

### Aides visuelles (indices)

- **Pencil marks** : lorsqu'une cellule vide est selectionnee, les candidats possibles (valeurs non encore presentes dans la meme ligne, colonne ou bloc) sont affiches en petits chiffres dans une grille 3x3 a l'interieur de la cellule.
- **Detection des naked singles** : les cellules n'ayant qu'une seule valeur candidate possible sont mises en evidence en vert clair automatiquement, signalant au joueur une case facilement resolvable.
- **Detection des contradictions** : toute cellule dont la valeur entre en conflit avec une autre cellule sur la meme ligne, colonne ou bloc est immediatement coloree en rouge. Les cases vides sans aucun candidat valide sont egalement signalee.
- **Activation/desactivation des indices** : un bouton dedie "H" dans la barre du chronometre (et son equivalent dans le menu Aide > Activer/desactiver les indices ou Ctrl+H) permet de masquer ou de restaurer instantanement les pencil marks et le surlignage des naked singles, sans affecter la detection des contradictions.

### Chronometre et pause

- **Chronometre** : un compteur MM:SS demarrant automatiquement au chargement de chaque nouvelle grille, affiche en permanence dans la barre sous la grille.
- **Pause** : le bouton ⏸ ou la touche Espace suspend le chronometre et affiche un overlay semi-transparent masquant la grille, rendant le jeu invisible pour eviter les regards indesirables. Une pression supplementaire reprend la partie.
- **Victoire** : a la resolution complete de la grille, le chronometre s'arrete automatiquement et une boite de dialogue de felicitations s'ouvre, affichant le temps total en MM:SS, avec les options Nouvelle partie (charge une nouvelle grille et ferme le dialogue) et Fermer.

### Annulation et retablissement

- **Annulation** : Ctrl+Z annule la derniere saisie et restaure la valeur precedente de la cellule modifiee. L'historique couvre toutes les modifications effectuees depuis le chargement de la grille.
- **Retablissement** : Ctrl+Y (ou Ctrl+Shift+Z selon la plateforme) retablit la derniere action annulee.
- Les piles d'annulation et de retablissement sont automatiquement effacees au chargement d'une nouvelle grille.

### Interface et ergonomie

- **Barre de difficulte** : quatre boutons a selection exclusive (Facile, Moyen, Difficile, Insane) avec le niveau actif mis en evidence. Chaque bouton porte une infobulle descriptive.
- **Barre de controle (chronometre)** : affiche le temps ecoule et regroupe les boutons ⏸ (pause), ↺ (redemarrer), H (indices), ? (aide) et FR/EN (langue).
- **Pave numerique virtuel** : widget NumPad en bas de fenetre permettant la saisie des chiffres 1 a 9 et l'effacement a la souris, avec infobulles.
- **Barre de menus** : menus Fichier, Edition, Aide, chacun avec actions, raccourcis clavier et separateurs. Le menu Aide contient egalement le sous-menu Langue.
- **Barre d'etat** : messages contextuels informant du resultat de chaque action (grille chargee, annulation effectuee, etc.).
- **Infobulles** : tous les elements interactifs (boutons, actions de menu) disposent d'une infobulle descriptive au survol, avec fond blanc et texte sombre.
- **Police Nunito** : interface typographiee avec la police Nunito, embarquee directement dans les ressources Qt (`.qrc`) pour un rendu identique sur toutes les plateformes.
- **Guide du joueur** : accessible via F1, le bouton ? ou le menu Aide. Presente la legende des couleurs avec des echantillons visuels, et un tableau complet des raccourcis clavier.

### Internationalisation (FR / EN)

- L'application supporte le francais et l'anglais sans redemarrage.
- Le changement de langue est accessible via le bouton FR/EN dans la barre de controle, ou via le menu Aide > Langue.
- La traduction repose sur le pipeline Qt : `lupdate` extrait les chaines depuis les appels `tr()`, `Qt Linguist` permet de les traduire dans les fichiers `.ts`, et `lrelease` compile ces fichiers en `.qm` binaires embarques dans les ressources.
- Le changement de langue en cours d'execution est gere par `retranslateUi()`, qui met a jour tous les textes (menus, boutons, infobulles, barre d'etat) sans reconstruire les widgets ni redemarrer l'application.

---

## Architecture

Le projet suit le patron **MVP (Modele-Vue-Presentateur)**, une variante du MVC adaptee aux interfaces graphiques. La separation des responsabilites est stricte : le Modele ignore totalement la Vue, et la Vue ne manipule jamais les donnees directement.

```
Vue  <---signals/slots--->  Controleur  <---appels directs--->  Modele
 |                                                                  |
 +------------------signals/slots (Observer)------------------------+
```

### Modele (`src/Model/`)

Encapsule l'etat complet du jeu et la logique metier. Ne depend d'aucun composant graphique.

#### `SudokuModel`

Coeur du Modele. Maintient :

- La grille 9x9 (`m_grid[9][9]`) avec les valeurs entrees.
- Les marqueurs de cellules fixes (`m_fixed[9][9]`) pour proteger les valeurs initiales.
- Les ensembles de candidats (`m_candidates[9][9]`) calcules dynamiquement.
- L'etat des indices (`m_hintsEnabled`) controlant l'exposition des candidats a la Vue.

Methodes publiques principales :

| Methode | Role |
|---|---|
| `getValue(row, col)` | Retourne la valeur d'une cellule |
| `setValue(row, col, value)` | Ecrit une valeur, declenche le recalcul et emets les signaux |
| `isFixed(row, col)` | Verifie si la cellule est une valeur initiale |
| `getCandidates(row, col)` | Retourne l'ensemble des candidats pour une cellule |
| `updateAllCandidates()` | Recalcule tous les candidats et emet les signaux de notification |
| `loadGrid(grid[9][9])` | Charge une grille, marque les cellules fixes, recalcule les candidats |
| `clearGrid()` | Remet la grille a zero |
| `isSolved()` | Verifie si toutes les cellules sont remplies (condition de victoire) |
| `setHintsEnabled(bool)` | Active ou desactive l'exposition des indices a la Vue |
| `hintsEnabled()` | Retourne l'etat courant des indices |

Signaux emis :

| Signal | Moment d'emission |
|---|---|
| `cellChanged(row, col)` | Apres chaque appel a `setValue()` |
| `candidatesUpdated()` | Apres recalcul complet des candidats |
| `contradictionDetected(row, col)` | Pour chaque cellule en conflit ou vide sans candidat |
| `gridSolved()` | Quand `isSolved()` retourne vrai apres une saisie |

#### `SudokuSolver`

Implemente un algorithme de resolution par backtracking avec validation. Utilise en interne pour verifier la coherence des grilles chargees.

### Vue (`src/View/`)

Ensemble de widgets Qt independants qui affichent l'etat du Modele et transmettent les interactions utilisateur vers le Controleur.

#### `MainWindow`

Fenetre principale (`QMainWindow`). Responsabilites :

- Orchestration de la mise en page : barre de difficulte, grille, barre de controle, pave numerique.
- Construction des menus (Fichier, Edition, Aide) avec leurs actions, raccourcis et infobulles.
- Gestion du chronometre (`QTimer`) : demarrage, arret, pause, affichage MM:SS.
- Slots de haut niveau : `onNewGrid()`, `onLoadGrid()`, `onDifficultySelected()`, `onUndo()`, `onRedo()`, `onToggleHints()`, `onPauseClicked()`, `onShowHelp()`, `onGridSolved()`, `onLanguageChanged()`.
- `retranslateUi()` : met a jour en une seule passe tous les textes traduisibles des widgets membres lors d'un changement de langue.
- Stocke comme membres tous les pointeurs vers les widgets et actions traduisibles pour pouvoir les mettre a jour sans reconstruire l'interface.

Membres principaux de l'interface :

| Membre | Type | Role |
|---|---|---|
| `m_gridView` | `SudokuGridView*` | La grille de jeu |
| `m_controller` | `SudokuController*` | Le controleur |
| `m_numPad` | `NumPad*` | Le pave numerique |
| `m_difficultyBar` | `QWidget*` | Barre de selection de difficulte |
| `m_difficultyButtons` | `QList<QPushButton*>` | Les 4 boutons de difficulte (pour retranslation) |
| `m_timerBar` | `QWidget*` | Barre du chronometre et des controles |
| `m_timerLabel` | `QLabel*` | Affichage MM:SS |
| `m_pauseBtn` | `QPushButton*` | Bouton pause/reprise (⏸/▶) |
| `m_restartBtn` | `QPushButton*` | Bouton redemarrage (↺) |
| `m_hintsBtn` | `QPushButton*` | Bouton indices (H), checkable |
| `m_helpBtn` | `QPushButton*` | Bouton aide (?) |
| `m_langBtn` | `QPushButton*` | Bouton langue (FR/EN) |
| `m_timer` | `QTimer*` | Timer 1 seconde |
| `m_seconds` | `int` | Secondes ecoulees depuis le debut |
| `m_paused` | `bool` | Etat de pause |
| `m_currentLang` | `QString` | Code langue actif ("fr" ou "en") |
| `m_fileMenu` | `QMenu*` | Menu Fichier |
| `m_editMenu` | `QMenu*` | Menu Edition |
| `m_helpMenu` | `QMenu*` | Menu Aide |
| `m_langMenu` | `QMenu*` | Sous-menu Langue |
| `m_newAction` | `QAction*` | Nouvelle grille |
| `m_loadAction` | `QAction*` | Ouvrir grille |
| `m_quitAction` | `QAction*` | Quitter (avec `NoRole` pour macOS) |
| `m_undoAction` | `QAction*` | Annuler |
| `m_redoAction` | `QAction*` | Retablir |
| `m_hintsAction` | `QAction*` | Activer/desactiver les indices, checkable |
| `m_helpAction` | `QAction*` | Guide du joueur |

#### `SudokuGridView`

Widget affichant la grille 9x9. Responsabilites :

- Creation et mise en page des 81 instances de `CellWidget` dans un `QGridLayout`.
- Gestion de la selection courante (`m_selectedRow`, `m_selectedCol`).
- Surlignage de la ligne, colonne et bloc 3x3 de la cellule selectionnee.
- Traitement des evenements clavier : touches directionnelles, chiffres 1-9, Suppr, Echap, Espace.
- Overlay de pause : un `QLabel` semi-transparent couvre toute la grille quand le jeu est en pause, avec l'icone de pause coloree en `#7A9AB5`.
- `refreshCell(row, col)` : met a jour un `CellWidget` a partir du Modele ; passe un `QSet<int>` vide si `m_model->hintsEnabled()` est faux, masquant ainsi les indices sans modifier les donnees du Modele.
- `resetView()` : reinitialise tous les etats visuels (contradictions, surlignage, selection, overlay) lors du chargement d'une nouvelle grille.

Signaux emis :

| Signal | Moment d'emission |
|---|---|
| `cellSelected(row, col)` | Quand l'utilisateur selectionne une cellule |
| `valueChanged(row, col, value)` | Quand une valeur est saisie ou effacee |
| `pauseToggled()` | Quand la touche Espace est pressee |

#### `CellWidget`

Represente une cellule individuelle (55x55 pixels). Responsabilites :

- Affichage de la valeur numerique via un `QLabel` principal.
- Affichage des pencil marks via un sous-widget `m_pencilWidget` contenant une grille 3x3 de `QLabel` (digits 1-9), visible uniquement quand la cellule est vide, selectionnee, et non fixe.
- Application du style visuel selon les etats prioritaires (via `updateStyle()`) :

| Etat | Couleur de fond | Couleur du texte |
|---|---|---|
| Selectionne | `#B8D4ED` | `#1A2A3A` |
| Contradiction | `#FADBD8` | `#C0392B` |
| Naked single | `#D5F5E3` | `#1E8449` |
| Surligne | `#FEF9E7` | `#2C3E50` (fixe) / `#5B8CCC` (joueur) |
| Fixe | `#EAF0F6` | `#2C3E50` |
| Vide (joueur) | `#FFFFFF` | `#5B8CCC` |

- Bordures epaissies (3px) aux frontieres des blocs 3x3, bordures fines (1px) a l'interieur.
- `mousePressEvent` : emet `cellClicked(row, col)` uniquement si la cellule n'est pas fixe.

#### `NumPad`

Pave numerique virtuel. Contient 9 boutons chiffres (1-9) et un bouton d'effacement (X). Chaque bouton emet `numberClicked(int)` a la pression. Les boutons 1-9 affichent le meme style que les cellules joueur, avec une taille fixe et des coins arrondis.

#### `HelpDialog`

Dialogue d'aide modal (`QDialog`). Contient :

- Un titre "Guide du joueur".
- Une section "Signification des couleurs" avec 6 echantillons visuels (carres colores) et leur description.
- Une section "Raccourcis clavier" avec un tableau de deux colonnes (touche, action) listant l'integralite des raccourcis disponibles.
- Un bouton Fermer.
- Toutes les chaines sont traduisibles via `tr()`.

#### `VictoryDialog`

Dialogue de victoire modal (`QDialog`). S'affiche quand la grille est resolue. Contient :

- Titre "Felicitations !".
- Message "Vous avez resolu la grille !".
- Temps ecoule formate en MM:SS.
- Bouton "Nouvelle partie" : emet le signal `newGameRequested()`.
- Bouton "Fermer" : appelle `accept()`.

La connexion dans `MainWindow::onGridSolved()` utilise `Qt::QueuedConnection` pour le slot `onNewGrid()`, afin d'eviter un crash lie a la destruction du dialogue pendant l'execution de son propre slot.

### Controleur (`src/Controller/`)

#### `SudokuController`

Intermediaire entre la Vue et le Modele. Responsabilites :

- Chargement des grilles (`loadDefaultGrid`, `loadRandomGrid`, `loadGridFromFile`). Le chargement aleatoire lit le nombre de grilles en premiere ligne du fichier, tire un index au hasard avec `QRandomGenerator`, et parse la ligne de 81 caracteres correspondante.
- Ecriture des valeurs (`setCellValue`) avec empilement de l'etat precedent dans la pile d'annulation.
- Gestion de l'historique d'annulation/retablissement via deux `QStack<CellState>`. Chaque `CellState` est un `QPair<QPair<int,int>, int>` encodant la position et l'ancienne valeur.
- Delegation du toggle des indices au Modele (`toggleHints`) et declenchement d'un recalcul complet des candidats pour forcer le rafraichissement de la Vue.

---

## Connexions signaux/slots

Tableau complet des connexions etablies dans `MainWindow` :

| Emetteur | Signal | Recepteur | Slot / Lambda |
|---|---|---|---|
| `SudokuGridView` | `valueChanged(row, col, val)` | `SudokuController` | `setCellValue(row, col, val)` |
| `SudokuGridView` | `pauseToggled()` | `MainWindow` | `onPauseClicked()` |
| `SudokuModel` | `gridSolved()` | `MainWindow` | `onGridSolved()` |
| `SudokuModel` | `candidatesUpdated()` | `SudokuGridView` | `onCandidatesUpdated()` |
| `SudokuModel` | `contradictionDetected(r, c)` | `SudokuGridView` | `onContradictionDetected(r, c)` |
| `QTimer` | `timeout()` | `MainWindow` | `onTimerTick()` |
| `m_numPad` | `numberClicked(val)` | `MainWindow` | `onNumberClicked(val)` |
| `m_pauseBtn` | `clicked()` | `MainWindow` | `onPauseClicked()` |
| `m_restartBtn` | `clicked()` | `MainWindow` | `onNewGrid()` |
| `m_hintsBtn` | `clicked()` | `MainWindow` | `onToggleHints()` |
| `m_helpBtn` | `clicked()` | `MainWindow` | `onShowHelp()` |
| `m_langBtn` | `clicked()` | `MainWindow` | lambda → `onLanguageChanged("en"/"fr")` |
| `m_newAction` | `triggered()` | `MainWindow` | `onNewGrid()` |
| `m_loadAction` | `triggered()` | `MainWindow` | `onLoadGrid()` |
| `m_quitAction` | `triggered()` | `MainWindow` | `close()` |
| `m_undoAction` | `triggered()` | `MainWindow` | `onUndo()` |
| `m_redoAction` | `triggered()` | `MainWindow` | `onRedo()` |
| `m_hintsAction` | `triggered()` | `MainWindow` | `onToggleHints()` |
| `m_helpAction` | `triggered()` | `MainWindow` | `onShowHelp()` |
| `VictoryDialog` | `newGameRequested()` | `MainWindow` | `onNewGrid()` (QueuedConnection) |
| `VictoryDialog` | `newGameRequested()` | `VictoryDialog` | `accept()` (DirectConnection) |

---

## Internationalisation

Le systeme de traduction repose sur le pipeline standard de Qt :

1. Les chaines traduisibles sont marquees avec `tr()` dans le code source.
2. `lupdate sudoku-assistant.pro` extrait les nouvelles chaines et met a jour les fichiers `.ts` (`sudoku_fr.ts`, `sudoku_en.ts`) sans ecraser les traductions existantes.
3. Les traductions sont saisies dans les fichiers `.ts` (XML) directement ou via Qt Linguist.
4. `lrelease sudoku-assistant.pro` compile les `.ts` en fichiers binaires `.qm` embarques dans `resources.qrc`.
5. Au demarrage, `main.cpp` charge le fichier `.qm` correspondant a la locale systeme via un `QTranslator` statique.
6. A chaque changement de langue, `onLanguageChanged(lang)` :
   - Met a jour `m_currentLang`.
   - Supprime l'ancien traducteur et installe le nouveau via `QCoreApplication::installTranslator()`.
   - Appelle `retranslateUi()` qui met a jour en une passe tous les textes des widgets membres.

Cette approche evite tout redemarrage de l'application et preserve l'etat courant du jeu (grille, chronometre, score).

---

## Systeme d'indices (hints)

Le systeme d'indices couvre deux aides visuelles : les pencil marks et le surlignage des naked singles. Il est pilote par le flag `m_hintsEnabled` du `SudokuModel`.

- **Activation/desactivation** : `SudokuController::toggleHints()` inverse le flag via `m_model->setHintsEnabled()` puis appelle `m_model->updateAllCandidates()` pour forcer un rafraichissement complet de la Vue.
- **Dans la Vue** : `SudokuGridView::refreshCell()` consulte `m_model->hintsEnabled()`. Si les indices sont desactives, il passe un `QSet<int>` vide a `CellWidget::setCandidates()`, ce qui a deux effets : `m_nakedSingle` est mis a `false` (plus de surlignage vert) et les pencil marks ne sont pas affiches.
- **Les contradictions** restent actives independamment du flag d'indices, car elles dependent uniquement du signal `contradictionDetected` emis par le Modele.
- **Synchronisation UI** : `onToggleHints()` lit `m_controller->model()->hintsEnabled()` apres le toggle pour synchroniser l'etat coche du bouton `m_hintsBtn` et de l'action `m_hintsAction` de facon canonique.

---

## Structure du projet

```
sudoku-assistant-qt/
├── src/
│   ├── main.cpp                        Point d'entree, chargement police et traducteur
│   ├── Model/
│   │   ├── SudokuModel.h / .cpp        Grille, candidats, hints, signaux
│   │   ├── SudokuSolver.h / .cpp       Algorithme de resolution par backtracking
│   ├── View/
│   │   ├── MainWindow.h / .cpp         Fenetre principale, menus, chrono, retranslateUi
│   │   ├── SudokuGridView.h / .cpp     Grille 9x9, selection, surlignage, pause overlay
│   │   ├── CellWidget.h / .cpp         Cellule individuelle, pencil marks, styles
│   │   ├── NumPad.h / .cpp             Pave numerique virtuel
│   │   ├── HelpDialog.h / .cpp         Guide du joueur, legende, raccourcis
│   │   └── VictoryDialog.h / .cpp      Dialogue de victoire avec temps ecoule
│   └── Controller/
│       ├── SudokuController.h / .cpp   Chargement, saisie, undo/redo, hints
├── resources/
│   ├── grilles/
│   │   ├── Easy.txt                    Grilles faciles (format 81 chars)
│   │   ├── Medium.txt                  Grilles moyennes
│   │   ├── Hard.txt                    Grilles difficiles
│   │   └── Insane.txt                  Grilles extremes
│   ├── fonts/
│   │   ├── Nunito/                     Police Nunito (variable + static)
│   │   └── Oswald/                     Police Oswald (variable + static)
│   ├── i18n/
│   │   ├── sudoku_fr.ts / .qm          Traductions francaises
│   │   └── sudoku_en.ts / .qm          Traductions anglaises
│   ├── images/
│   │   └── pause.png                   Icone de pause (overlay)
│   └── resources.qrc                   Manifeste des ressources embarquees
├── sudoku-assistant.pro                Fichier de projet qmake
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
# Editer les .ts avec Qt Linguist ou manuellement
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
| Ctrl+O | Ouvrir une grille depuis un fichier |
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

Ce projet a ete realise dans le cadre d'un cours de genie logiciel et d'interfaces graphiques en deuxieme annee a l'ENSI-CAEN. Il a pour objectif de mettre en pratique les patrons de conception orientee objet (MVP, Observateur), la programmation en C++17, et le developpement d'interfaces graphiques avec le framework Qt6.

- **Auteur** : Anas Kartaoui

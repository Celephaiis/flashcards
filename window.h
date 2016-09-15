#ifndef WINDOW_H
#define WINDOW_H

#include "adddeck.h"
#include "cardview.h"
#include <QPushButton>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QFile>
#include <QString>
#include <QMainWindow>
#include <QApplication>
#include <QMenu>
#include <QString>
#include <QFileSystemWatcher>
#include <QTableWidget>
#include <string>

// Main Window
// Base Window for all other functions

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class Window : public QMainWindow {
    Q_OBJECT
    public:
        Window(QWidget *parent = 0);
    public slots:
        void chooseDeck();
        void addDeck();
    private:
        QMenu   *createMenu();
        QGroupBox *createButtons();
        QPushButton *e_button;
        QTableWidget *deckList;
        QFile Deck;
        QTableWidget *createDeckView();
        QPushButton *c_button;
        QPushButton *a_button;
        QFileSystemWatcher *watcher;
        int  getDeckName();
        int xmlInit();
        void refreshDeckList();
        //QFile *deckName;
        int m;
};
#endif // WINDOW_H

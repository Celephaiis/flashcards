#ifndef CARDVIEW_H
#define CARDVIEW_H

#include "window.h"
#include "adddeck.h"
#include "practicecards.h"
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
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <string>
#include <QTableWidget>

// 
// View Cards corresponding to the currently selected Deck
//

class CardView : public QMainWindow {
    Q_OBJECT
    public:
        CardView(int deckIndex, QWidget *parent = 0);
    public slots:
        void practiceCards();
        void editCards();
        void closeCardView();
        void refreshCardList();
    private:
        QPushButton *p_button;
        QPushButton *e_button;
        QPushButton *q_button;
        QTableWidget *table;
        void *createMenu();
        QGroupBox *createButtons();
        QTableWidget *createCardList();
        int loadCards();
        QString DeckName;
        QString getDeckName();
        int deckIndex;
};

#endif // CARDVIEW_H

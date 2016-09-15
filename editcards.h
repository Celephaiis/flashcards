#ifndef EDITCARDS_H
#define EDITCARDS_H

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


class EditCards : public QMainWindow {
    Q_OBJECT
    public:
        EditCards(QString DeckName, QWidget *parent = 0);
    public slots:
        void closeEditCards();
        void deleteCard();
        void findCard();
        void editCard();
        void addCard();
        void refreshCardList(int row);
    private:
        QMenu   *createMenu();
        QGroupBox *createButtons();
        int loadCards();
        void *createToolBar();
        QTableWidget *table;
        QFile Deck;
        QTableWidget *createCardList();
        QPushButton *s_button;
        QPushButton *q_button;
        QPushButton *a_button;
        QPushButton *e_button;
        QPushButton *testButton;
        QLineEdit *lineedit;
        QString DeckName;
        int getCurrentRow();
};
#endif // EDITCARDS_H

#ifndef ADDCARD_H
#define ADDCARD_H

#include "window.h"
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
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QSpinBox>
#include <QDateTimeEdit>


//
// Show Dialog to add a Card to the Deck
//

class AddCard : public QDialog {
    Q_OBJECT
    public:
        AddCard(QString DeckName, QWidget *parent = 0);
    public slots:
        void closeAddCard();
        void saveCard();
    private:
        QPushButton *e_button;
        QPushButton *s_button;
        QPlainTextEdit *question_field;
        QPlainTextEdit *answer_field;
        QPlainTextEdit *tip_field;
        QDateTimeEdit *interval_field;
        QLabel *question_label;
        QLabel *answer_label;
        QLabel *interval_label;
        QLabel *tip_label;
        QString DeckName;
        void * setupDialog();
        QString dateTimeString;
};

#endif // ADDCARD_H

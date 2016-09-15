#ifndef EDITCARD_H
#define EDITCARD_H

#include "window.h"
#include "cardview.h"
#include "editcards.h"
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
#include <QDateTimeEdit>


//
// Show Dialog to add a Card to the Deck
//

class EditCard : public QDialog {
    Q_OBJECT
    public:
        EditCard(QString question, QString answer, QString tip, QString interval, int row, QString DeckName, QWidget *parent = 0);
    public slots:
        void closeEditCard();
        void saveCard();
        void deleteCard();
    private:
        QPushButton *e_button;
        QPushButton *s_button;
        QPushButton *d_button;
        QPlainTextEdit *question_field;
        QPlainTextEdit *answer_field;
        QPlainTextEdit *tip_field;
        QDateTimeEdit *interval_field;
        QLabel *question_label;
        QLabel *answer_label;
        QLabel *tip_label;
        QLabel *interval_label;
        QString DeckName;
        QString tip;
        void setupDialog();
        int row;
        QString dateTimeString;
};

#endif // EDITCARD_H

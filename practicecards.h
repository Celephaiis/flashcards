#ifndef PRACTICECARDS_H
#define PRACTICECARDS_H

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
#include <string>
#include <QTableWidget>

// 
//

class PracticeCards : public QMainWindow {
    Q_OBJECT
    public:
        PracticeCards(QString DeckName, QWidget *parent = 0);
    public slots:
    private:
        QPushButton *a_button;
        QPushButton *e_button;
        QPushButton *q_button;
        QTableWidget *table;
        void *createMenu();
        QGroupBox *createButtons();
        QString DeckName;
};

#endif // PRACTICECARDS_H 

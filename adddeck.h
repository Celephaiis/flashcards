#ifndef ADDDECK_H
#define ADDDECK_H

#include "window.h"
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

//
// Add deck QDialog Window
//

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class AddDeck : public QDialog {
    Q_OBJECT
    public:
        AddDeck(QWidget *parent = 0);
    public slots:
        void closeAddDeck();
        void saveDeck();
    private:
        QPushButton *e_button;
        QPushButton *s_button;
        QLineEdit *name_field;
        QPlainTextEdit *desc_field;
        QLabel *name_label;
        QLabel *desc_label;
};

#endif // ADDDECK_H

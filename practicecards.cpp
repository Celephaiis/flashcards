#include "cardview.h"
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QApplication>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QFontMetrics>
#include <QFormLayout>
#include <string>
#include <QDebug>
#include <QTableWidget>
#include <QStringList>
#include <QString>
#include <QHeaderView>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QtXml>

//
// Window
//

PracticeCards::PracticeCards(QString DeckName, QWidget *parent) : QMainWindow(parent) {
    //
    // Create 
    setWindowTitle("Flash Cards - Deck: " + DeckName +  " - Practice Cards");
    resize(700, 500);
    
    createMenu();

    this->DeckName = DeckName;

    // Set Layout

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(createButtons(), 1, Qt::AlignHCenter);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);


    connect(a_button, SIGNAL(clicked()), parent, SLOT(refreshCardList()));
    connect(e_button, SIGNAL(clicked()), parent, SLOT(refreshCardList()));
    connect(q_button, SIGNAL(clicked()), parent, SLOT(refreshCardList()));
}

void *PracticeCards::createMenu() {
    // Create Menu
    //
    // First add pictures, actions and buttons.
    // Set Shortcuts and
    // Assemble Menu
    //
    QPixmap choosepix("res/load.png");
    QPixmap addpix("res/add.png");
    QPixmap quitpix("res/quit.png");
    QAction *choose = new QAction(choosepix, "&Edit Card", this);
    QAction *add = new QAction(addpix, "&Add Card", this);
    QAction *quit = new QAction(quitpix, "&Quit", this);
    choose->setShortcut(tr("CTRL+O"));
    add->setShortcut(tr("CTRL+I"));
    quit->setShortcut(tr("CTRL+Q"));


    QMenu *menu;
    menu = menuBar()->addMenu("&File");
    menu->addAction(choose);
    menu->addAction(add);
    menu->addSeparator();
    menu->addAction(quit);
    connect(choose, SIGNAL(triggered()), this, SLOT(editCard()));
    connect(add, SIGNAL(triggered()), this, SLOT(addCard()));
    connect(quit, SIGNAL(triggered()), this, SLOT(closeEditCards()));

    return menu;
}

QGroupBox *PracticeCards::createButtons() {
    //
    // Create basic functionality buttons
    //
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setFixedSize(550, 50);
    groupBox->setStyleSheet("QGroupBox {border: 0px;}");

    //s_button = new QPushButton("Save", this);
    e_button = new QPushButton("Edit", this);
    a_button = new QPushButton("Add", this);
    q_button = new QPushButton("Back", this);
    connect(e_button, SIGNAL(clicked()), this, SLOT(editCard()));
    connect(a_button, SIGNAL(clicked()), this, SLOT(addCard()));
    connect(q_button, SIGNAL(clicked()), this, SLOT(closeEditCards()));

    QHBoxLayout *vbox = new QHBoxLayout;
    //vbox->addWidget(s_button);
    vbox->addWidget(a_button);
    vbox->addWidget(e_button);
    vbox->addWidget(q_button);
    groupBox->setLayout(vbox);
    return groupBox;
}

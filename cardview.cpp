#include "adddeck.h"
#include "window.h"
#include "addcard.h"
#include "editcards.h"
#include "practicecards.h"
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
#include <QtXml>

//
// Window to view and further interact with the chosen Deck
//

CardView::CardView(int deckIndex, QWidget *parent) : QMainWindow(parent) {
    //
    // Create CardView Window
    // and figure out the corresponding entry
    // in the Decks.txt file from the Index
    // found in the QTableWidget
    //
    
    resize(700, 500);
    
    createMenu();

    this->deckIndex = deckIndex;

    QFile DeckFile("Decks.xml");
    DeckName = getDeckName() + ".xml";
    setWindowTitle("Flash Cards - Deck: " + DeckName + " - Card View");

    // Set Layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(createCardList());
    layout->addWidget(createButtons(), 1, Qt::AlignTop);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);



    loadCards();
}

QString CardView::getDeckName() {
    //
    // CHECK LATER IF METHOD IS REALLY NEEDED
    //
    QDomDocument document;
    QFile file("Decks.xml");
    QString errorString;
    int errorLine, errorColumn;

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file!";
    } else {
        if(!document.setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
            qDebug() << "Failed to load file!";
            qDebug() << errorLine << errorColumn;
            qDebug() << errorString;
        }
        file.close();
    }

    QDomElement root = document.firstChildElement();
    // List Elements
    QDomNodeList itemQ = root.elementsByTagName("Name");
    QDomNode itemNode = itemQ.at(deckIndex);
    QDomElement itemEle = itemNode.toElement();
    return itemEle.text();
}

int CardView::loadCards() {
    //
    // Load the Cards from the Deck file
    // and add them to the Table
    //
    QDomDocument document;
    QFile file(DeckName);
    QString errorString;
    int errorLine, errorColumn;

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file!";
        return -1;
    } else {
        if(!document.setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
            qDebug() << "Failed to load file!";
            qDebug() << errorLine << errorColumn;
            qDebug() << errorString;
            return -1;
        }
        file.close();
    }

    QDomElement root = document.firstChildElement();

    int k = 0;
    // List Elements
    QDomNodeList itemQ = root.elementsByTagName("Question");
    for (int i = 0; i < itemQ.count(); i++) {
        QDomNode itemNode = itemQ.at(i);
        if (itemNode.isElement()) {
            QDomElement itemEle = itemNode.toElement();
            table->setItem(k, 0, new QTableWidgetItem(itemEle.text().simplified()));
            k++;
        }
    }
    
    return 0;
}

void CardView::refreshCardList() {
    QDomDocument document;
    QFile file(DeckName);
    QString errorString;
    int errorLine, errorColumn;

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file!";
    } else {
        if(!document.setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
            qDebug() << "Failed to load file!";
            qDebug() << errorLine << errorColumn;
            qDebug() << errorString;
        }
        file.close();
    }

    QDomElement root = document.firstChildElement();


    int k = 0, j = 0;
    // List Elements
    QDomNodeList itemQ = root.elementsByTagName("Question");
    table->setRowCount(0);
    table->setRowCount(itemQ.count());
    for (int i = 0; i < itemQ.count(); i++) {
        QDomNode itemNode = itemQ.at(i);
        if (itemNode.isElement()) {
            QDomElement itemEle = itemNode.toElement();
            table->setItem(k, 0, new QTableWidgetItem(itemEle.text().simplified()));
            k++;
        }
    }
}

void *CardView::createMenu() {
    //
    // Create Menu
    //
    QPixmap practicepix("res/load.png");
    QPixmap addpix("res/add.png");
    QPixmap editpix("res/edit.png");
    QPixmap quitpix("res/quit.png");
    QAction *practice = new QAction(practicepix, "&Practice Cards", this);
    //QAction *add = new QAction(addpix, "&Add card", this);
    QAction *edit = new QAction(editpix, "&Edit card", this);
    QAction *quit = new QAction(quitpix, "&Quit", this);
    practice->setShortcut(tr("CTRL+O"));
    //add->setShortcut(tr("CTRL+I"));
    edit->setShortcut(tr("CTRL+E"));
    quit->setShortcut(tr("CTRL+Q"));


    QMenu *menu;
    menu = menuBar()->addMenu("&File");
    menu->addAction(practice);
    //menu->addAction(add);
    menu->addAction(edit);
    menu->addSeparator();
    menu->addAction(quit);
    //connect(practice, SIGNAL(triggered()), this, SLOT(practiceDeck()));
    //connect(add, SIGNAL(triggered()), this, SLOT(addCard()));
    connect(edit, SIGNAL(triggered()), this, SLOT(editCards()));
    connect(quit, SIGNAL(triggered()), this, SLOT(closeCardView()));

    return 0;
}

QTableWidget *CardView::createCardList() {
    //
    // Creat the inital card list
    //
    QDomDocument document;
    QFile DeckFile(DeckName);
    QString errorString;
    int errorLine, errorColumn;

    if(!DeckFile.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file!";
    } else {
        if(!document.setContent(&DeckFile, false, &errorString, &errorLine, &errorColumn)) {
            qDebug() << "Failed to load file!";
            qDebug() << errorLine << errorColumn;
            qDebug() << errorString;
        }
        DeckFile.close();
    }
    QDomElement root = document.firstChildElement();
    // List Elements
    QDomNodeList itemQ = root.elementsByTagName("Question");

    // Set Properties
    table = new QTableWidget(this);
    table->setRowCount(itemQ.count());
    table->setColumnCount(1);
    table->setColumnWidth(0, 615);
    table->setShowGrid(false);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setHorizontalHeaderLabels(QString("Card").split("|"));
    table->selectRow(0);

    connect(table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(editCards()));
    
    return table;
}

QGroupBox *CardView::createButtons() {
    //
    // Create basic functionality buttons
    //
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setFixedSize(150, 150);
    groupBox->setStyleSheet("QGroupBox {border: 0px;}");

    p_button = new QPushButton("Practice", this);
    e_button = new QPushButton("Edit Cards", this);
    q_button = new QPushButton("Quit CardView", this);
    connect(p_button, SIGNAL(clicked()), this, SLOT(practiceCards()));
    connect(e_button, SIGNAL(clicked()), this, SLOT(editCards()));
    connect(e_button, SIGNAL(clicked()), this, SLOT(refreshCardList()));
    connect(q_button, SIGNAL(clicked()), this, SLOT(closeCardView()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(p_button);
    vbox->addWidget(e_button);
    vbox->addWidget(q_button);
    groupBox->setLayout(vbox);
    return groupBox;
}

void CardView::practiceCards() {
    //
    // Open window to practice cards
    //
    PracticeCards *openPracticeCards = new PracticeCards(DeckName, this);
    openPracticeCards->setWindowModality(Qt::ApplicationModal);

    openPracticeCards->show();
    this->hide();

}

void CardView::editCards() {
    //
    // Show a window to view and edit all Cards
    // and hide this window
    //
    EditCards *openEditCards = new EditCards(DeckName, this);
    openEditCards->setWindowModality(Qt::ApplicationModal);

    openEditCards->show();
    this->hide();
}

void CardView::closeCardView() {
    //
    // Quit this CardView Window
    // and unhide the Base parent window
    //
    this->close();
    parentWidget()->show();
}

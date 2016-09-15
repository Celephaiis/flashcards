#include "window.h"
#include "adddeck.h"
#include "cardview.h"
#include <QPushButton>
#include <QGroupBox>
#include <QApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include <iostream>
#include <sstream>
#include <fstream>
#include <QDebug>
#include <string>
#include <QtXml>

// Base Window for all other functionalities

using namespace std;

Window::Window(QWidget *parent) : QMainWindow(parent) {
    resize(700, 500);
    setWindowTitle("Flash Cards - DeckView");

    //Create Menu
    createMenu();

    //Create rest of the Window
    QHBoxLayout *box = new QHBoxLayout;
    box->addWidget(createDeckView(), 0, 0);
    box->addWidget(createButtons(), 1, Qt::AlignTop);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(box);

    // initialize the table with XML data
    xmlInit();
}


QMenu *Window::createMenu() {
    // Create Menu
    //
    // First add pictures, actions and buttons.
    // Set Shortcuts and
    // Assemble Menu
    //
    QPixmap choosepix("res/load.png");
    QPixmap addpix("res/add.png");
    QPixmap quitpix("res/quit.png");
    QAction *choose = new QAction(choosepix, "&Choose Deck", this);
    QAction *add = new QAction(addpix, "&Add deck", this);
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
    connect(choose, SIGNAL(triggered()), this, SLOT(chooseDeck()));
    connect(add, SIGNAL(triggered()), this, SLOT(addDeck()));
    connect(quit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    return menu;
}

int Window::xmlInit() {
    //
    // Initialize the QTableWidget with XML Data
    //
    //
    QDomDocument document;
    QFile file("Decks.xml");
    QString errorString;
    int errorLine, errorColumn;

    // Open file
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file!";
        return -1;
    } else {
        // Set working document to the File address
        if(!document.setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
            qDebug() << "Failed to load file!";
            qDebug() << errorLine << errorColumn;
            qDebug() << errorString;
            return -1;
        }
        file.close();
    }

    // Find the first child element
    QDomElement root = document.firstChildElement();


    int k = 0, j = 0;
    //
    // Count "Name" tags in the document 
    // and loop through them
    QDomNodeList itemQ = root.elementsByTagName("Name");
    for (int i = 0; i < itemQ.count(); i++) {
        QDomNode itemNode = itemQ.at(i);
            QDomElement itemEle = itemNode.toElement();
            // Add each entry to the Cardname Column in the Table
            deckList->setItem(k, 0, new QTableWidgetItem(itemEle.text()));
            k++;
    }
    // Count "Description" tags and loop through them
    QDomNodeList itemA = root.elementsByTagName("Description");
    for (int i = 0; i < itemA.count(); i++) {
        // Loop throug each node
        QDomNode itemNode = itemA.at(i);
        m = 0;
        if (itemNode.isElement()) {
            // Get next elements/tag's text
            QDomElement itemEle = itemNode.toElement();
            QString str = itemEle.text();
            
            // Add each entry to the Desc Column in the table
            // with simplified (removed the newlines) string
            deckList->setItem(j, 1, new QTableWidgetItem(itemEle.text().simplified()));
            j++;
        }
    }

    return 0;
}

QGroupBox *Window::createButtons() {
    // Create Basic Buttons
    //
    // Create Buttons to add/change Decks
    // And add them to the Layout
    //
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setFixedSize(150, 150);
    groupBox->setStyleSheet("QGroupBox {border: 0px;}");

    c_button = new QPushButton("Choose Deck", this);
    a_button = new QPushButton("Add Deck", this);
    e_button = new QPushButton("Quit", this);
    connect(c_button, SIGNAL(clicked()), this, SLOT(chooseDeck()));
    connect(a_button, SIGNAL(clicked()), this, SLOT(addDeck()));
    connect(e_button, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(c_button);
    vbox->addWidget(a_button);
    vbox->addWidget(e_button);
    groupBox->setLayout(vbox);
    return groupBox;
}

QTableWidget *Window::createDeckView() {
    // Create List of Decks
    //
    // Create a List of Decks with QTableWidget
    // Set properties of the TableWidget
    // And fill it with Decks
    //

    //
    // Count the Lines in the Decks.xml file 
    // 
    QDomDocument document;
    QFile DeckFile("Decks.xml");
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
    // Count "Deck" entries in the file
    QDomNodeList itemQ = root.elementsByTagName("Deck");

    //
    // Set properties of the QTableWidget
    //
    DeckFile.close();
    deckList = new QTableWidget(this);
    deckList->setRowCount(itemQ.count());
    deckList->setColumnCount(2);
    deckList->setColumnWidth(0, 250);
    deckList->horizontalHeader()->setStretchLastSection(true);
    deckList->setShowGrid(false);
    deckList->verticalHeader()->hide();
    deckList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    deckList->setSelectionBehavior(QAbstractItemView::SelectRows);
    deckList->setSelectionMode(QAbstractItemView::SingleSelection);
    deckList->setHorizontalHeaderLabels(QString("Name|Description").split("|"));
    deckList->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    deckList->selectRow(0);

    
    connect(deckList, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(chooseDeck()));

   return deckList;;
}

void Window::refreshDeckList() {
    //
    // Refresh the QTableWidget of Decks once
    // a new Entry has been added
    //     
    QDomDocument document1;
    QDomDocument document;
    QFile file("Decks.xml");
    QString errorString;
    int errorLine, errorColumn;

    // Open File
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file!";
    } else {
        // Set working doument to the Decks.xml file
        if(!document1.setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
            qDebug() << "Failed to load file!";
            qDebug() << errorLine << errorColumn;
            qDebug() << errorString;
        }
        file.close();
    }
    // Find first child element
    QDomElement root1 = document1.firstChildElement();
    // Count number of "Name" entries in the file
    QDomNodeList entry_count = root1.elementsByTagName("Name");
    // Add a new column for the newly added Deck
    deckList->insertRow(deckList->rowCount());
    // Loop through the entries
    for (int h = 0; h < entry_count.count(); h++) {
        // find entry at each node
        QDomNode itemNode = entry_count.at(h);
        QDomElement itemEle = itemNode.toElement();
        // add entry to table
        deckList->setItem(h, 0, new QTableWidgetItem(itemEle.text()));
        // count the "Description" entries
        QDomNodeList count = root1.elementsByTagName("Description");
        QDomNode node = count.at(h);
        QDomElement ele = node.toElement();
        // Add entry to the table
        deckList->setItem(h, 1, new QTableWidgetItem(ele.text()));
    
    }
}

int Window::getDeckName() {
    //
    // Get name of the currently selected Deck to pass it to its children
    //
    int deck = 0; 
    QString deckname;
    QItemSelectionModel *select = deckList->selectionModel();
    //
    // Find the currently selected Row
    // 
    
    deck = select->currentIndex().row();
    
    //
    // Turn the index of the current row to a string
    //
    
    deckname = deckList->model()->data(deckList->model()->index(deck,0)).toString();

    return deck;
}

void Window::chooseDeck() {
    //
    // Open CardView to see the Cards correspoding
    // to the currently selected Deck
    // set it so you can't interact with this base Window
    // and hide his window for a clear View
    //
    CardView *openCardView = new CardView(getDeckName(), this);
    openCardView->setWindowModality(Qt::ApplicationModal);
    openCardView->show();
    this->hide();
}

void Window::addDeck() {
    //
    // Open QDialog Window to add a new Deck
    //
    AddDeck *openAddDeck = new AddDeck(this);
    openAddDeck->setWindowModality(Qt::ApplicationModal);

//    watcher = new QFileSystemWatcher(this);
//    watcher->addPath("Decks.txt");

    if(openAddDeck->exec() == QDialog::Accepted) {
        refreshDeckList();
    }
}


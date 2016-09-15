#include "cardview.h"
#include "window.h"
#include "addcard.h"
#include "editcards.h"
#include "editcard.h"
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

EditCards::EditCards(QString DeckName, QWidget *parent) : QMainWindow(parent) {
    //
    // Create 
    setWindowTitle("Flash Cards - Deck: " + DeckName +  " - Edit Cards");
    resize(700, 500);
    
    createMenu();
    createToolBar();

    this->DeckName = DeckName;

    // Set Layout

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(createCardList());
    layout->addWidget(createButtons(), 1, Qt::AlignHCenter);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    loadCards();

    connect(a_button, SIGNAL(clicked()), parent, SLOT(refreshCardList()));
    connect(e_button, SIGNAL(clicked()), parent, SLOT(refreshCardList()));
    connect(q_button, SIGNAL(clicked()), parent, SLOT(refreshCardList()));
}

void *EditCards::createToolBar() {
    QToolBar * toolBar= new QToolBar("Search");
    QLabel *label = new QLabel("Search: ");
    lineedit = new QLineEdit(this);
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    lineedit->setFixedWidth(200);
    toolBar->addWidget(spacerWidget);
    toolBar->addWidget(label);
    toolBar->addWidget(lineedit);
        
    connect(lineedit, SIGNAL(textEdited(const QString&)), this, SLOT(findCard()));
                     
    addToolBar(Qt::TopToolBarArea, toolBar);
    return 0;
}

int EditCards::loadCards() {
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


    int k = 0, j = 0, h = 0;;
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
    
    QDomNodeList itemA = root.elementsByTagName("Answer");
    for (int i = 0; i < itemA.count(); i++) {
        QDomNode itemNode = itemA.at(i);
        if (itemNode.isElement()) {
            QDomElement itemEle = itemNode.toElement();
            table->setItem(j, 1, new QTableWidgetItem(itemEle.text().simplified()));
            j++;
        }
    }

    QDomNodeList intervalItem = root.elementsByTagName("Interval");
    for (int i = 0; i < intervalItem.count(); i++) {
        QDomNode intervalNode = intervalItem.at(h);
        if (intervalNode.isElement()) {
            QDomElement intervalElement = intervalNode.toElement();
            QDateTime dateTime;
            dateTime.fromString(intervalElement.text(),"hh:mm dd/MM/yy");
            table->setItem(h, 2, new QTableWidgetItem(intervalElement.text().simplified()));
            h++;
        }
    }
    return 0;
}

void EditCards::refreshCardList(int row) {
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


    int k = 0, j = 0, h = 0;
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
    
    QDomNodeList itemA = root.elementsByTagName("Answer");
    for (int i = 0; i < itemA.count(); i++) {
        QDomNode itemNode = itemA.at(i);
        if (itemNode.isElement()) {
            QDomElement itemEle = itemNode.toElement();
            table->setItem(j, 1, new QTableWidgetItem(itemEle.text().simplified()));
            j++;
        }
    }

    QDomNodeList intervalItem = root.elementsByTagName("Interval");
    for (int i = 0; i < intervalItem.count(); i++) {
        QDomNode intervalNode = intervalItem.at(h);
        if (intervalNode.isElement()) {
            QDomElement intervalElement = intervalNode.toElement();
            table->setItem(h, 2, new QTableWidgetItem(intervalElement.text().simplified()));
            h++;
        }
    }
}

QMenu *EditCards::createMenu() {
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

QTableWidget *EditCards::createCardList() {
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
    table->setColumnCount(3);
    table->setColumnWidth(0, 295);
    table->setColumnWidth(1, 295);
    table->setShowGrid(false);
    table->verticalHeader()->hide();
    table->setSortingEnabled(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setHorizontalHeaderLabels(QString("Question|Answer|Repetition").split("|"));
    table->horizontalHeader()->setStretchLastSection(true);
    table->selectRow(0);

    connect(table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(editCard()));

    return table;
}

QGroupBox *EditCards::createButtons() {
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

int EditCards::getCurrentRow() {
    return table->selectionModel()->currentIndex().row();
}

void EditCards::addCard() {
    int row = getCurrentRow();
    AddCard *openAddCard = new AddCard(DeckName, this);
    openAddCard->setWindowModality(Qt::ApplicationModal);

    if(openAddCard->exec() == QDialog::Accepted) {
        refreshCardList(row);
        //refresh card list;
    }
}

void EditCards::editCard() {
    int row = getCurrentRow();
    QString question, answer, tip, interval;

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


    int j = 0, k = 0, h = 0, m = 0;
    // List Elements
    QDomNodeList itemQ = root.elementsByTagName("Question");
    for (int i = 0; i < itemQ.count(); i++) {
        QDomNode itemNode = itemQ.at(row);
        if (itemNode.isElement()) {
            QDomElement itemEle = itemNode.toElement();
            question = itemEle.text();
           // table->setItem(k, 0, new QTableWidgetItem(itemEle.text()));
            k++;
        }
    }
    
    QDomNodeList itemA = root.elementsByTagName("Answer");
    for (int i = 0; i < itemA.count(); i++) {
        QDomNode itemNode = itemA.at(row);
        if (itemNode.isElement()) {
            QDomElement itemEle = itemNode.toElement();
            answer = itemEle.text();
           // table->setItem(j, 1, new QTableWidgetItem(itemEle.text()));
            j++;
        }
    }
    
    QDomNodeList tipItem = root.elementsByTagName("Tip");
    for (int i = 0; i < tipItem.count(); i++) {
        QDomNode tipNode = tipItem.at(row);
        if (tipNode.isElement()) {
            QDomElement tipElement = tipNode.toElement();
            tip = tipElement.text();
           // table->setItem(j, 1, new QTableWidgetItem(itemEle.text()));
            h++;
        }
    }
    
    QDomNodeList intervalItem = root.elementsByTagName("Interval");
    for (int i = 0; i < intervalItem.count(); i++) {
        QDomNode intervalNode = intervalItem.at(row);
        if (intervalNode.isElement()) {
            QDomElement intervalElement = intervalNode.toElement();
            interval = intervalElement.text();
           // table->setItem(j, 1, new QTableWidgetItem(itemEle.text()));
            m++;
        }
    }

    EditCard *openEditCard = new EditCard(question, answer, tip, interval, row, DeckName, this);
    openEditCard->setWindowModality(Qt::ApplicationModal);

    if(openEditCard->exec() == QDialog::Accepted) {
        refreshCardList(row);
    }
}

void EditCards::findCard() {
    QString filter = lineedit->text();
    for (int i = 0; i < table->rowCount(); i++) {
        bool match = false;
        for (int j = 0; j < 3; j++) {
            QTableWidgetItem *item = table->item(i, j);
            if (item->text().contains(filter, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        table->setRowHidden(i, !match);
    }
}

void EditCards::deleteCard() {
    int deck = getCurrentRow();
    table->removeRow(deck);

    QFile CardFile(DeckName);
    QFile TempFile(DeckName+".tmp");

    int line_count=0;
    QString line, s;
    
    //  WORKS - FROM THE INTERNET
    if (CardFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream istream(&CardFile);
        while(!istream.atEnd()) {
            line = istream.readLine();
            if(line_count != deck) {
                s.append(line + "\n");
            }
            line_count++;
        }
        CardFile.resize(0);
        istream << s;
        CardFile.close();
        
    }
}

void EditCards::closeEditCards() {
    this->close();
    parentWidget()->show();
}

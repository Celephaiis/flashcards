#include "adddeck.h"
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
#include <QtXml>
#include <QStandardItem>

AddDeck::AddDeck(QWidget *parent) : QDialog(parent) {
    //
    // Set properties and buttons for this add deck window
    //
    
    setWindowTitle("Flash Cards - Add Deck");
    name_field = new QLineEdit(this);
    desc_field = new QPlainTextEdit(this);
    
    // Set Height of textedit field
    QFontMetrics font_metric(desc_field->font());
    int rowHeight = font_metric.lineSpacing();
    desc_field->setFixedHeight(5*rowHeight);

    desc_field->setTabChangesFocus(true);

    name_label = new QLabel(this);
    desc_label = new QLabel(this);
    name_label->setText("Deck Name:");
    desc_label->setText("Deck Description:");

    s_button = new QPushButton("Save");
    e_button = new QPushButton("Cancel");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(s_button, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(e_button, QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveDeck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Assemble the Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(name_label);
    layout->addWidget(name_field);
    layout->addWidget(desc_label);
    layout->addWidget(desc_field);
    layout->addWidget(buttonBox);
    setLayout(layout);

}

void AddDeck::closeAddDeck() {
    //
    // Destroy this window upon exit
    //
    
    this->destroy();
}

void AddDeck::saveDeck() {
    //
    // Save to file and quit back to Deck View
    //

    if(!name_field->text().isEmpty() && !desc_field->toPlainText().isEmpty()) {
       QDomDocument document1;
       QDomDocument document;
       QFile file("Decks.xml");
       QString errorString;
       int errorLine, errorColumn;

       // Start of building the xml file
       QDomElement xmlroot = document.createElement("Decks");
       //xmlroot.setAttribute("deck", "Marise");
       document.appendChild(xmlroot);

       // Open File
       if(!file.open(QIODevice::ReadWrite)) {
           qDebug() << "Failed to open file!";
       } else {
           if(!document1.setContent(&file, false, &errorString, &errorLine, &errorColumn)) {
               qDebug() << "Failed to load file!";
               qDebug() << errorLine << errorColumn;
               qDebug() << errorString;
           }
           file.close();
       }

       // Find first element in the file
       QDomElement root1 = document1.firstChildElement();

       int k = 0, j = 0, h = 0;
       // Count occurences of the "Name" element
       QDomNodeList entry_count = root1.elementsByTagName("Name");
        // loop through file
       while (h < entry_count.count()) {
           // Read existing entries in file
           QDomElement xmldeck = document.createElement("Deck");
           xmldeck.setAttribute("id", j);
           xmlroot.appendChild(xmldeck);
           QDomNode itemNode = entry_count.at(k);
           QDomElement itemEle = itemNode.toElement();
           xmldeck.appendChild(itemEle);
           QDomNodeList count = root1.elementsByTagName("Description");
           QDomNode node = count.at(k);
           QDomElement ele = node.toElement();
           xmldeck.appendChild(ele);
           j++;

       }

       //
       // add new deck to document
       for (int i = 0; i < 1; i++) {
           QDomElement xmldeck = document.createElement("Deck");
           xmldeck.setAttribute("id", j);
           xmlroot.appendChild(xmldeck);

           QDomElement xmlname = document.createElement("Name");
           xmldeck.appendChild(xmlname);
           QDomText name = document.createTextNode(name_field->text());
           xmlname.appendChild(name);
           

           QDomElement xmldesc = document.createElement("Description");
           xmldeck.appendChild(xmldesc);
           QDomText desc = document.createTextNode(desc_field->toPlainText());
           xmldesc.appendChild(desc);
       }

       // Try to open the file
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           qDebug() << "Failed to write file!";
       }

       // Write document to file
       QTextStream stream(&file);
       stream << document.toString();
       file.close();
       accept();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Not all fields filled out!"));
        
    }
}

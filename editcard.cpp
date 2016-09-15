#include "addcard.h"
#include "cardview.h"
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
#include <QString>
#include <QDebug>
#include <QDateTimeEdit>
#include <vector>
#include <sstream>
#include <QtXml>

//
// QDialog Window to add a card to the chosen deck
//


EditCard::EditCard(QString question, QString answer, QString tip, QString interval, int row , QString DeckName, QWidget *parent) : QDialog(parent) {
    this->DeckName = DeckName;
    this->row = row;
    this->tip = tip;
    setupDialog();
    question_field->appendPlainText(question);
    answer_field->appendPlainText(answer);
    tip_field->appendPlainText(tip);

    QDateTime date = QDateTime::fromString(interval, "hh:mm dd/MM/yy");
    interval_field->setDateTime(date);
}

void EditCard::setupDialog() {
    question_field = new QPlainTextEdit(this);
    answer_field = new QPlainTextEdit(this);
    tip_field = new QPlainTextEdit(this);
    interval_field = new QDateTimeEdit(this);

    interval_field->setCalendarPopup(true);
    interval_field->setDisplayFormat("hh:mm dd/MM/yy");
    interval_field->setFocusPolicy(Qt::ClickFocus);
    interval_field->setDateRange(QDate::currentDate(), QDate::currentDate().addDays(365));
    interval_field->setMinimumTime(QTime::currentTime());


    // Set Height of the question & answer fields
    QFontMetrics font_metric(question_field->font());
    int rowHeight = font_metric.lineSpacing();
    question_field->setFixedHeight(11*rowHeight);
    answer_field->setFixedHeight(11*rowHeight);
    tip_field->setFixedHeight(5*rowHeight);
    
    question_label = new QLabel(this);
    answer_label = new QLabel(this);
    tip_label = new QLabel(this);
    interval_label = new QLabel(this);
    question_label->setText("Question:");
    answer_label->setText("Answer:");
    tip_label->setText("Tip:");
    interval_label->setText("Repeat:");
    
    question_field->setTabChangesFocus(true);
    answer_field->setTabChangesFocus(true);
    tip_field->setTabChangesFocus(true);

    s_button = new QPushButton("Save");
    e_button = new QPushButton("Cancel");
    d_button = new QPushButton("Delete");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(s_button, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(e_button, QDialogButtonBox::RejectRole);
    buttonBox->addButton(d_button, QDialogButtonBox::AcceptRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveCard()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(d_button, SIGNAL(clicked()), this, SLOT(deleteCard()));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(question_label, question_field);
    layout->addRow(answer_label, answer_field);
    layout->addRow(tip_label, tip_field);
    layout->addRow(interval_label, interval_field);
    layout->addRow(buttonBox);
    setLayout(layout);
}

void EditCard::closeEditCard() {
    //
    // Destroy this window to return back to CardView
    //
    this->destroy();
}

void EditCard::saveCard() {
    //
    // Save Card to file and exit if successfull
    // otherwise display an error message
    //
    if(!question_field->toPlainText().isEmpty() && !answer_field->toPlainText().isEmpty()) {
       QDomDocument document1;
       QDomDocument document;
       QFile file(DeckName);;
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

       QDomElement root1 = document1.firstChildElement();


       int k = 0, j = 0, h = 0;
       QDomNodeList entry_count = root1.elementsByTagName("Question");
       //for (int h = 0; h < entry_count.count(); h++) {
       while (h < entry_count.count()) {
                QDomNode itemNode = entry_count.at(k);
                QDomElement itemEle = itemNode.toElement();
                QDomNodeList count = root1.elementsByTagName("Answer");
                QDomNode node = count.at(k);
                QDomElement ele = node.toElement();
                QDomElement xmldeck = document.createElement("Card");
                xmldeck.setAttribute("id", j);
                xmlroot.appendChild(xmldeck);
                xmldeck.appendChild(ele);
                xmldeck.appendChild(itemEle);

                QDomNodeList tipCount = root1.elementsByTagName("Tip");
                QDomNode tipNode = tipCount.at(k);
                QDomElement tipElement = tipNode.toElement();
                xmldeck.appendChild(tipElement);

                QDomNodeList intervalCount = root1.elementsByTagName("Interval");
                QDomNode intervalNode = intervalCount.at(k);
                QDomElement intervalElement = intervalNode.toElement();
                xmldeck.appendChild(intervalElement);
           
           if (j == row) {
                    QDomElement xmlcard = document.createElement("Card");
                    xmlcard.setAttribute("id", j);
                    xmlroot.replaceChild(xmlcard, xmldeck);

                    QDomElement xmlquestion = document.createElement("Question");
                    xmlcard.appendChild(xmlquestion);
                    QDomText question = document.createTextNode(question_field->toPlainText());
                    xmlquestion.appendChild(question);
                    
                    QDomElement xmlanswer = document.createElement("Answer");
                    xmlcard.appendChild(xmlanswer);
                    QDomText answer = document.createTextNode(answer_field->toPlainText());
                    xmlanswer.appendChild(answer);
                    
                    QDomElement xmltip = document.createElement("Tip");
                    xmlcard.appendChild(xmltip);
                    QDomText tip = document.createTextNode(tip_field->toPlainText());
                    xmltip.appendChild(tip);

                    QDomElement xmlinterval = document.createElement("Interval");
                    xmlcard.appendChild(xmlinterval);

                    QDateTime dateTime = interval_field->dateTime();
                    dateTimeString = dateTime.toString("hh:mm dd/MM/yy");
                    
                    QDomText interval = document.createTextNode(dateTimeString);
                    xmlinterval.appendChild(interval);
           }   
           j++;

       }

       //
       // Write Card to file
       //

       if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           qDebug() << "Failed to write file!";
       }

       QTextStream stream(&file);
       stream << document.toString();
       file.close();
       accept();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Not all fields filled out!"));
        
    }
}

void EditCard::deleteCard() {
       QDomDocument document1;
       QDomDocument document;
       QFile file(DeckName);;
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

       QDomElement root1 = document1.firstChildElement();


       int k = 0, j = 0, h = 0;
       QDomNodeList entry_count = root1.elementsByTagName("Question");
       while (h < entry_count.count()) {
                QDomNode itemNode = entry_count.at(k);
                QDomElement itemEle = itemNode.toElement();
                QDomNodeList count = root1.elementsByTagName("Answer");
                QDomNode node = count.at(k);
                QDomElement ele = node.toElement();
                QDomElement xmldeck = document.createElement("Card");
                xmldeck.setAttribute("id", j);
                xmlroot.appendChild(xmldeck);
                xmldeck.appendChild(ele);
                xmldeck.appendChild(itemEle);

                QDomNodeList tipCount = root1.elementsByTagName("Tip");
                QDomNode tipNode = tipCount.at(k);
                QDomElement tipElement = tipNode.toElement();
                xmldeck.appendChild(tipElement);

                QDomNodeList intervalCount = root1.elementsByTagName("Interval");
                QDomNode intervalNode = intervalCount.at(k);
                QDomElement intervalElement = intervalNode.toElement();
                xmldeck.appendChild(intervalElement);
           
           if (j == row) {
                    QDomElement xmlcard = document.createElement("Card");
                    xmlcard.setAttribute("id", j);
                    xmlroot.removeChild(xmldeck);

                    QDomElement xmlname = document.createElement("Question");
                    xmlcard.appendChild(xmlname);

                    QDomText name = document.createTextNode(question_field->toPlainText());
                    xmlname.appendChild(name);
                    
                    QDomElement xmldesc = document.createElement("Answer");
                    xmlcard.appendChild(xmldesc);
                    QDomText desc = document.createTextNode(answer_field->toPlainText());
                    xmldesc.appendChild(desc);

                    QDomElement xmltip = document.createElement("Tip");
                    xmlcard.appendChild(xmltip);
                    QDomText tip = document.createTextNode(tip_field->toPlainText());
                    xmltip.appendChild(tip);

                    QDomElement xmlinterval = document.createElement("Interval");
                    xmlcard.appendChild(xmlinterval);

                    QDateTime dateTime = interval_field->dateTime();
                    dateTimeString = dateTime.toString("hh:mm dd/MM/yy");
                    
                    QDomText interval = document.createTextNode(dateTimeString);
                    xmlinterval.appendChild(interval);
           }
           j++;

       }
       //
       // Write Card to file
       //

       if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           qDebug() << "Failed to write file!";
       }

       QTextStream stream(&file);
       stream << document.toString();
       file.close();
       accept();
}

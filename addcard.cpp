#include "addcard.h"
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
#include <QString>
#include <QDebug>
#include <QSpinBox>
#include <vector>
#include <QFontMetrics>
#include <QDateTimeEdit>
#include <QtXml>
#include <QDomDocument>

//
// QDialog Window to add a card to the chosen deck
//

AddCard::AddCard(QString DeckName, QWidget *parent) : QDialog(parent) {
    //
    // Set buttons, labels, fields 
    // and properties to display the
    // add Card window
    //
    setWindowTitle("Flash Cards - Add Card");

    this->DeckName = DeckName;

    setupDialog();
}

void *AddCard::setupDialog() {
    question_field = new QPlainTextEdit(this);
    answer_field = new QPlainTextEdit(this);
    tip_field = new QPlainTextEdit(this);
    interval_field = new QDateTimeEdit(this);

    interval_field->setDateTime(QDateTime::currentDateTime());
    interval_field->setCalendarPopup(true);
    interval_field->setDisplayFormat("hh:mm dd/MM/yy");
    interval_field->setDateRange(QDate::currentDate(), QDate::currentDate().addDays(365));
    interval_field->setMinimumTime(QTime::currentTime());
    interval_field->setFocusPolicy(Qt::ClickFocus);

    QDateTime dateTime = interval_field->dateTime();
    dateTimeString = dateTime.toString("hh:mm dd/MM/yy");

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
    interval_label->setText("Interval:");
    
    question_field->setTabChangesFocus(true);
    answer_field->setTabChangesFocus(true);
    tip_field->setTabChangesFocus(true);

    s_button = new QPushButton("Save");
    e_button = new QPushButton("Cancel");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(s_button, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(e_button, QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveCard()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(question_label, question_field);
    layout->addRow(answer_label, answer_field);
    layout->addRow(tip_label, tip_field);
    layout->addRow(interval_label, interval_field);
    layout->addRow(buttonBox);
    setLayout(layout);
    return 0;
}

void AddCard::closeAddCard() {
    //
    // Destroy this window to return back to CardView
    //
    this->destroy();
}

void AddCard::saveCard() {
    //
    // Save Card to file and exit if successfull
    // otherwise display an error message
    //
    QDomDocument document1;
    QDomDocument document;
    QFile file(DeckName);
    QString errorString;
    int errorLine, errorColumn;

    if(!question_field->toPlainText().isEmpty() && !answer_field->toPlainText().isEmpty()) {
        // Start of building the xml file
        QDomElement xmlroot = document.createElement("Deck");
        xmlroot.setAttribute("deck", DeckName);
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
            QDomElement xmlcard = document.createElement("Card");
            xmlcard.setAttribute("id", j);
            xmlroot.appendChild(xmlcard);
            
            QDomNode questionNode = entry_count.at(k);
            QDomElement questionElement = questionNode.toElement();
            xmlcard.appendChild(questionElement);
            
            QDomNodeList answerCount = root1.elementsByTagName("Answer");
            QDomNode answerNode = answerCount.at(k);
            QDomElement answerElement = answerNode.toElement();
            xmlcard.appendChild(answerElement);

            QDomNodeList tipCount = root1.elementsByTagName("Tip");
            QDomNode tipNode = tipCount.at(k);
            QDomElement tipElement = tipNode.toElement();
            xmlcard.appendChild(tipElement);

            QDomNodeList intervalCount = root1.elementsByTagName("Interval");
            QDomNode intervalNode = intervalCount.at(k);
            QDomElement intervalElement = intervalNode.toElement();
            xmlcard.appendChild(intervalElement);
            
            j++;
        }
        //
        // Write new Card to file
        //
           QDomElement newcard = document.createElement("Card");
           newcard.setAttribute("id", j);
           xmlroot.appendChild(newcard);

           QDomElement newquestion = document.createElement("Question");
           newcard.appendChild(newquestion);
           QDomText newque = document.createTextNode(question_field->toPlainText());
           newquestion.appendChild(newque);
           
           QDomElement newanswer = document.createElement("Answer");
           newcard.appendChild(newanswer);
           QDomText newans = document.createTextNode(answer_field->toPlainText());
           newanswer.appendChild(newans);

           QDomElement newtip = document.createElement("Tip");
           newcard.appendChild(newtip);
           QDomText newtipfield = document.createTextNode(tip_field->toPlainText());
           newtip.appendChild(newtipfield);

           QDomElement newinterval = document.createElement("Interval");
           newcard.appendChild(newinterval);
           QDomText newinterv = document.createTextNode(dateTimeString);
           newinterval.appendChild(newinterv);

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

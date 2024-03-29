#include "terminal.h"
#include <QDebug>
#include <QLineEdit>
#include <QtGlobal>
#include <QDebug>
#include <stdlib.h>     /* getenv */
#include <QMouseEvent>
#include <QKeyEvent>

Terminal::Terminal(QWidget *parent) : QPlainTextEdit (parent)
{
    // Set max saved lines from user
    maxSavedLines = 50;

    QLineEdit *lineEdit = new QLineEdit();
    this->setMinimumSize(QSize(300,500));
    process = new QProcess(this);
    QString shell = qgetenv("SHELL");



    if(shell.contains("zsh")){
        currentShell = Shell::zsh;
    } else if(shell.contains("bash")){
        currentShell = Shell::bash;
    } else{
        qDebug() << "No default shell detected, defaulting to bash";
        currentShell = Shell::bash;
    }

    process->setProgram(shell);
    process->start();


    // Set all necessary variables
    checkVariables();

    // Get computer name (only needs to be executed once)
    process->write("hostname\n");
    process->waitForReadyRead();
    hostName = process->readAllStandardOutput();
    hostName.chop(1);

    unchangedText = "@" + hostName + ":";

    // Write to terminal
    this->insertPlainText(shellText);

    minSize = this->textCursor().position();
}


void Terminal::checkVariables(){
    /*
     * Get bash-like lines
     * Read user name & hostname
     * For example:
     * whoami@hostname:~/Downloads$
    */

    switch (currentShell) {
    case Shell::zsh:
        process->write("echo \"${(%%)PS1}\"");
        break;
    case Shell::bash:
        process->write("echo \"${PS1@P}\"");
        break;
    }

    process->waitForReadyRead();
    shellText = process->readAllStandardOutput();

}


void Terminal::append(QString s){
    s.chop(1);

    if(!s.isEmpty())
        this->appendPlainText(s);

}

QString Terminal::cleanInput(){
    int countSpaces = 0;
    // count amount of prefix spaces and remove them if character is not a space
    for(int i = 0; i < userInput.count(); i++){
        if(!userInput[i].isSpace())
            return userInput.remove(0,  countSpaces);

        countSpaces++;
    }
}

void Terminal::run(){
    cleanInput();

    // Remove first element if it is equal to maxSavedLines
    if(inputs.size() >= maxSavedLines)
        inputs.removeFirst();

    inputs.append(userInput);

    if(userInput == "clear")
        this->clear();

    // Add '\n' so bash executes it
    process->write(userInput.toUtf8() + '\n');
    process->waitForReadyRead(100);

    this->append(process->readAllStandardOutput());
    //this->append(process->readAllStandardError());

    userInput.clear();
    
    // update minSize & set shellText again
    checkVariables();
    this->appendPlainText(shellText);
    this->update();
    minSize = this->textCursor().position();
}


void Terminal::writeOutput(){
    qDebug() << "blaa";
}


void Terminal::mouseReleaseEvent(QMouseEvent *e){

    QPlainTextEdit::mouseReleaseEvent(e);
}


void Terminal::keyPressEvent(QKeyEvent *e){

    qDebug() << e->key();
    // Up Arrow
    if(e->key() == 16777235){
        qDebug() << inputs.size()  << " - " << inputIndex << " = " << inputs.size() - inputIndex;
        // Range check
        if(inputs.size() - inputIndex > 0 && inputs.size() - inputIndex <= inputs.size()){
            inputIndex++;
            // Because arrays start at 0, result - 1
            replaceUserInput(inputs[(inputs.size() - inputIndex)]);

        }
        else{
            replaceUserInput("");
        }
    // Down Arrow
    } else if(e->key() == 16777237){

        // Range check
        if(inputs.size() - inputIndex >= 0 && inputs.size() - inputIndex < inputs.size()){
            inputIndex--;
            // Because arrays start at 0, result - 1
            replaceUserInput(inputs[(inputs.size() - inputIndex)]);
        }
        else{
            replaceUserInput("");
        }
    }


    //if(e->key())
    if(minSize > this->textCursor().position()){
        e->ignore();
        return;
    // Ignore the key if it is a backspace and is on the minimum size
    } else if(minSize >= this->textCursor().position() && e->key() == 16777219){
        e->ignore();
        return;
    // On enter
    } else if(e->key() == 16777220){
        run();
        e->ignore();
        return;
    }

    userInput += e->text();

    QPlainTextEdit::keyPressEvent(e);
}


Terminal::~Terminal(){
    process->closeWriteChannel();
    process->close();

}


void Terminal::clearUserInput(){
    QTextCursor cursor = this->textCursor();

    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();

    this->setTextCursor(cursor);
}
void Terminal::addUserInput(QString s){

}
void Terminal::replaceUserInput(QString s){
    clearUserInput();

    this->insertPlainText(shellText + s);
    this->textCursor().setPosition(1000);
    userInput = s;
}



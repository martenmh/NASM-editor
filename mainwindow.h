#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSessionManager>
#include <QtWidgets>

#include "highlighter.h"
#include "texteditor.h"
#include "terminal.h"

#include "window.h"

#include "sidetab.h"

namespace Ui {
class MainWindow;
}


enum devMode {
    Debug,
    NoDebug
};

class MainWindow : public QMainWindow
{

 Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void loadFile(const QString &fileName);
    void loadDirectory(const QString &fileName);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void documentWasModified();

private:

    devMode developMode;
    void setupEditor();
    void createActions();
    void createStatusBar();

    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    SideTab *left;
    SideTab *right;

    Window *window;
    TextEditor *editor;
    Highlighter *highlighter;
    Terminal *terminal;

    /* Sidetab & statusbar has buttons for opening and closing the terminal & file viewer */

    Ui::MainWindow *ui;
    QString curFile;
    QString curDir;
};

#endif // MAINWINDOW_H

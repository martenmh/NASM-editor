#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSessionManager>
#include <QtWidgets>
#include "highlighter.h"

#include "texteditor.h"
namespace Ui {
class MainWindow;
}

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
    void setupEditor();
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);


    TextEditor *editor;
    Highlighter *highlighter;
    Ui::MainWindow *ui;
    QString curFile;
    QString curDir;
};

#endif // MAINWINDOW_H

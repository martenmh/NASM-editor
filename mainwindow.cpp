#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSessionManager>
#include <QApplication>
#include <QGuiApplication>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QHBoxLayout>

#include "sidetab.h"
#include "filetree.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Set up the window layout */
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    window = new Window(this);

    /* Set up the central code editor */
    setupEditor();

    createStatusBar();
    createActions();

    /* Set up the left side tab */
    FileTree *tree = new FileTree();

    left = new SideTab(this, tree);
    QString *s = new QString("Hello");
    left->addAction(0, *s);
    layout->addWidget(left, 1);

    QSplitter *splitter = new QSplitter(this);
    splitter->setChildrenCollapsible(false);
    splitter->setContentsMargins(0, 0, 0, 0);
    splitter->addWidget(tree);
    splitter->setHandleWidth(0);
    /* Set up the central layout for the console & code editor */
//    QWidget *widget = splitter->widget(index);
//    QSizePolicy policy = widget->sizePolicy();
//    policy.setHorizontalStretch(stretch);
//    policy.setVerticalStretch(stretch);
//    widget->setSizePolicy(policy);


    QSplitter *vertical_splitter = new QSplitter(Qt::Vertical, this);
    vertical_splitter->setChildrenCollapsible(false);
    vertical_splitter->setContentsMargins(0, 0, 0, 0);
    vertical_splitter->setHandleWidth(0);
        /* Set the main code editor */
    vertical_splitter->addWidget(editor);
    vertical_splitter->addWidget(terminal);
    splitter->addWidget(vertical_splitter);
    layout->addWidget(splitter, 96);

    /* Set up the right side tab */
    SideTab *right = new SideTab(this);
    layout->addWidget(right, 1);

    window->setLayout(layout);
    window->show();


    setCentralWidget(window);

    readSettings();

    connect(editor->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);


    setCurrentFile(QString());
    setUnifiedTitleAndToolBarOnMac(true);
    setWindowTitle("NASM IDE [*]");
}


void MainWindow::setupEditor(){
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    terminal = new Terminal(this);
    editor = new TextEditor(this);

    terminal->setFont(font);
    editor->setFont(font);

    // Set editor palette
    QPalette ep = editor->palette();
    ep.setColor(QPalette::Base, Qt::black);
    ep.setColor(QPalette::Text, Qt::white);
    editor->setPalette(ep);

    // Set console palette
    QPalette cp = editor->palette();
    cp.setColor(QPalette::Base, Qt::black);
    cp.setColor(QPalette::Text, Qt::white);
    editor->setPalette(cp);



    highlighter = new Highlighter(editor->document());

    QFile file("mainwindow.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}


void MainWindow::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        writeSettings();
        event->accept();
    } else{
        event->ignore();
    }
}


void MainWindow::newFile(){
    if(maybeSave()){
        editor->clear();
        setCurrentFile(QString());
    }
}
#include "filedialog.h"
#include <iostream>
void MainWindow::open(){

    if(maybeSave()){
        QFileDialog* fileDialog = new QFileDialog(this);
          fileDialog->setFileMode(QFileDialog::Directory);
          fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);

          // Try to select multiple files and directories at the same time in QFileDialog
          QListView *l = fileDialog->findChild<QListView*>("listView");
          if (l) {
            //l->setSelectionMode(QAbstractItemView::MultiSelection);
           }
          QTreeView *t = fileDialog->findChild<QTreeView*>();
           if (t) {
             //t->setSelectionMode(QAbstractItemView::MultiSelection);
            }

          int nMode = fileDialog->exec();

          QStringList files = fileDialog->selectedFiles();
            auto dirs = fileDialog->selectedUrls().at(0).url();
          this->left->tree()->setCurDir(dirs);
//        f.open();
//        f.show();
//        f.
//        QStringList files = QFileDialog::getOpenFileNames(
//                                this,
//                                "Select one or more files to open",
//                                "/home");

//        QString fileName = QFileDialog::getOpenFileNames();
        if(!files.empty()){
            for(auto file : files){

                loadFile(file);
            }
        }
            //loadFile(files);
    }
}

bool MainWindow::save(){
    if(curFile.isEmpty()){
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}


bool MainWindow::saveAs(){
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::documentWasModified(){
    setWindowModified(editor->document()->isModified());
}

void MainWindow::createActions(){
    /* First, create the menu & toolbar for files */
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    /* Then create the new action */
    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);            // Set shortcut to CTRL + N
    newAct->setStatusTip(tr("Create a new file"));      // Set tooltip
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);   // Connect slots to actions
    fileMenu->addAction(newAct);

    /* Then the open action.. */
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save current file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(saveAct);

    QAction *saveAsAct = new QAction(tr("&Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save file as a new file"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(saveAsAct);

    /* Set all menu items like this */
    /* Create edit menu & toolbar */
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));


    QAction *cutAct = new QAction(tr("&Cut"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selected text"));
    connect(cutAct, &QAction::triggered, this, &MainWindow::open);
    editMenu->addAction(cutAct);

    QAction *copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selected text"));
    connect(copyAct, &QAction::triggered, this, &MainWindow::open);
    editMenu->addAction(copyAct);


    #ifndef QT_NO_CLIPBOARD
        cutAct->setEnabled(false);
        copyAct->setEnabled(false);
        connect(editor, &TextEditor::copyAvailable, cutAct, &QAction::setEnabled);
        connect(editor, &TextEditor::copyAvailable, copyAct, &QAction::setEnabled);
    #endif // !QN_NO_CLIPBOARD



}

void MainWindow::createStatusBar(){


    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if(geometry.isEmpty()){
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
            (availableGeometry.height() - height()) / 2);
    } else{
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave(){
    /* Simple switch statement to turn of maybeSave functionality when developing */
    switch(developMode){
    case devMode::NoDebug:
    {
        if(!editor->document()->isModified())
            return true;

        const QMessageBox::StandardButton ret
                = QMessageBox::warning(this, tr("Application"), tr("The document has been modified.\n"
                                                                    "Do you want to save your changes?"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        switch(ret){
        case QMessageBox::Save:
            return save();
        case QMessageBox::Cancel:
            return false;
        default:
            break;
        }
        return true;
    }
    case devMode::Debug:
        return true;
    }
}

void MainWindow::loadFile(const QString &fileName){
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::loadDirectory(const QString &dirName){
    QDir dir(dirName);
    if(!dir.isReadable()){
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read directory %1:\n%2.")
                             .arg(QDir::toNativeSeparators(dirName)));
        return;
    }

    //QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        //editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif
      ///  setCurrentFile(fileName);
        statusBar()->showMessage(tr("File loaded"), 2000);
}



bool MainWindow::saveFile(const QString &fileName){
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        out << editor->toPlainText();
#ifndef QT_NO_CURSOR
        QGuiApplication::restoreOverrideCursor();
#endif

        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
}

void MainWindow::setCurrentFile(const QString &fileName){
    curFile = fileName;
    editor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if(curFile.isEmpty())
        shownName = "untitled.asm";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}


MainWindow::~MainWindow()
{
    delete editor;
    delete highlighter;
    delete ui;
}

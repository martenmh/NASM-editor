#ifndef TERMINAL_H
#define TERMINAL_H
#include <QPlainTextEdit>
#include <QProcess>
#include <QString>

class Terminal : public QPlainTextEdit
{
    Q_OBJECT
public:
    /* Begin bash process */
    Terminal(QWidget *parent);
    ~Terminal() override;

    /* Run bash when user has input */
    void run();
    /* Reset variables */
    void checkVariables();
public slots:
    // Write to output when process is finished
    void writeOutput();

protected:
    /* Intercept cursor change */
    void mouseReleaseEvent(QMouseEvent *e) override;
    /* Intercept user input for running commands & ignoring changed text */
    void keyPressEvent(QKeyEvent *e) override;
private:
    enum Shell{
        zsh,
        bash
    };
    Shell currentShell;

    // Functions for clearing and adding the userinput
    void clearUserInput();
    void addUserInput(QString s);
    void replaceUserInput(QString s);

    // Store the last 50 inputs of the user
    QStringList inputs;
    // Store current index of user looking at inputs
    int inputIndex;
    int maxSavedLines;

    // Clean the user input given
    QString cleanInput();
    void append(QString s);
    // Set the minimum size of the text;
    int minSize;
    QProcess *process;
    QString userInput;
    QString shellText;


    QString fullWorkingDir;
    QString userName;
    QString hostName;
    char* homeDir;
    QString bashPath;
    QString unchangedText;
    /*
     * State that holds if user only has entered spaces or backspaces
     * (for ignoring the first spaces) = '  clear'
     *                                    ^^-- ignore
    */
    bool hasText;
};

#endif // TERMINAL_H

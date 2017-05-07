#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "cpusim.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_compile_clicked();
    void on_step_compile_clicked();

    void on_reset_clicked();
    void updateState();
    void on_textEdit_textChanged();

    void updateLineNumbers();
private:
    Ui::MainWindow *ui;
    int registers[4];
    QString createRegisterContent(int val);
    int* getRegisterState();
    QString getInterpreterText();
    void updateRegisterStates();
    void setConsoleText(const QString message);


    int currentStep{0};
    QStringList program;
    CPU cpu;
};

#endif // MAINWINDOW_H



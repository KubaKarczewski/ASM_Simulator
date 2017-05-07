#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include "cpusim.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->regAX->setText(createRegisterContent(0));
    ui->regBX->setText(createRegisterContent(0));
    ui->regCX->setText(createRegisterContent(0));
    ui->regDX->setText(createRegisterContent(0));

    updateLineNumbers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getInterpreterText(){
    QString text = ui->textEdit->toPlainText();
    return text;
}

void MainWindow::setConsoleText(const QString message){
    ui->console->setText(message);

}

void MainWindow::updateState()
{
    updateRegisterStates();
    ui->currentStep->setValue(currentStep);
}

void MainWindow::updateRegisterStates()
{
    ui->regAX->setText(createRegisterContent(cpu.AX));
    ui->regBX->setText(createRegisterContent(cpu.BX));
    ui->regCX->setText(createRegisterContent(cpu.CX));
    ui->regDX->setText(createRegisterContent(cpu.DX));
}

void MainWindow::on_reset_clicked()
{
    currentStep = 0;
    cpu = CPU();
    updateState();
}

void MainWindow::on_compile_clicked()
{
    ui->console->setText("");
    QString result = compileProgram(getInterpreterText(), cpu);
    ui->console->setText(result);
    updateState();
}
QString MainWindow::createRegisterContent(int val){
    QString content = QString("%1 | %2")
            .arg(QString::number(val,2).rightJustified(16,'0'))
            .arg(QString::number(val,10));
    return content;
}
void MainWindow::on_step_compile_clicked()
{
    ui->console->setText("");
    if (currentStep==0) { // split into lines
        program = getInterpreterText().split("\n");
    }

    if (currentStep<program.size()) {
        QString result = parseLine(program[currentStep], cpu);
        if (result.size() > 0) {
            ui->console->setText(QString("Error on line %1: %2")
                                 .arg(currentStep+1)
                                 .arg(result));
        }
        currentStep+=1;
    } else {
        ui->console->setText("End of program.");
    }
    updateState();
}

void MainWindow::updateLineNumbers()
{
    QString lines;
    for( int i=0; i<getInterpreterText().split("\n").size(); i++) {
        lines += QString("%1\n").arg(i+1);
    }

    ui->lines->setText(lines);
}

void MainWindow::on_textEdit_textChanged()
{
    updateLineNumbers();
}

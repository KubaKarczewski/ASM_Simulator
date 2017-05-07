#ifndef CPUSIM_H
#define CPUSIM_H
#include <QString>
#include <QVector>

struct CPU
{
    unsigned short AX;
    unsigned short BX;
    unsigned short CX;
    unsigned short DX;
};

QString compileProgram(QString program, CPU& cpu);
QString parseLine(QString& line, CPU& cpu);

#endif // CPUSIM_H

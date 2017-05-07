#include "cpusim.h"
#include <QDebug>

enum ArgumentType
{
    Invalid,
    Immediate,
    Register,
    HalfRegister
};

ArgumentType getArgumentType(QString arg)
{
    char reg = arg[0].toLatin1();
    if (reg == 'A' || reg == 'B' || reg == 'C' || reg == 'D') {
        if (arg.size() == 2) {
            char regType = arg[1].toLatin1();
            if (regType == 'X') {
                return Register;
            } else if(regType == 'H' || regType == 'L') {
                return HalfRegister;
            }
       }
    } else {
        bool ok = false;
        short immval = arg.toShort(&ok);
        if (ok) {
            return ArgumentType::Immediate;
        }
    }

    return ArgumentType::Invalid;
}

unsigned short* getFullRegister(QString arg, CPU& cpu)
{
    char reg = arg[0].toLatin1();
    switch(reg)
    {
    case 'A':
        return &cpu.AX;
    case 'B':
        return &cpu.BX;
    case 'C':
        return &cpu.CX;
    case 'D':
        return &cpu.DX;
    }
}

unsigned char* getHalfRegister(QString arg, CPU& cpu)
{
    unsigned char* reg = (unsigned char*)getFullRegister(arg, cpu);
    if (arg[1] == "H") {
        reg+=1;
    }
    return reg;
}

unsigned short getImmediateShort(QString arg)
{
    return arg.toShort();
}

unsigned char getImmediateByte(QString arg)
{
    return (unsigned char)arg.toShort();
}

QString parseAdd(QString arg1, QString arg2, CPU& cpu)
{
    ArgumentType arg1t = getArgumentType(arg1);
    ArgumentType arg2t = getArgumentType(arg2);

    if (arg1t == Register) {
        unsigned short* reg = getFullRegister(arg1, cpu);
        if (arg2t == Register) {
            *reg += *getFullRegister(arg2, cpu);
            return "";
        } else if(arg2t == HalfRegister) {
            *reg += *getHalfRegister(arg2, cpu);
            return "";
        } else if(arg2t == Immediate) {
            *reg += getImmediateShort(arg2);
        }
    } else if (arg1t == HalfRegister) {
        unsigned char* reg = getHalfRegister(arg1, cpu);
        if (arg2t == HalfRegister) {
            *reg += *getHalfRegister(arg2, cpu);
        } else if(arg2t == Immediate) {
            *reg += getImmediateByte(arg2);
        } else {
            return "Half registers only support byte operations.";
        }
    }

    return "";
}

QString parseSub(QString arg1, QString arg2, CPU& cpu)
{
    ArgumentType arg1t = getArgumentType(arg1);
    ArgumentType arg2t = getArgumentType(arg2);

    if (arg1t == Register) {
        unsigned short* reg = getFullRegister(arg1, cpu);
        if (arg2t == Register) {
            *reg -= *getFullRegister(arg2, cpu);
            return "";
        } else if(arg2t == HalfRegister) {
            *reg -= *getHalfRegister(arg2, cpu);
            return "";
        } else if(arg2t == Immediate) {
            *reg -= getImmediateShort(arg2);
        }
    } else if (arg1t == HalfRegister) {
        unsigned char* reg = getHalfRegister(arg1, cpu);
        if (arg2t == HalfRegister) {
            *reg -= *getHalfRegister(arg2, cpu);
        } else if(arg2t == Immediate) {
            *reg -= getImmediateByte(arg2);
        } else {
            return "Half registers only support byte operations.";
        }
    }

    return "";
}

QString parseMov(QString arg1, QString arg2, CPU& cpu)
{
    ArgumentType arg1t = getArgumentType(arg1);
    ArgumentType arg2t = getArgumentType(arg2);

    if (arg1t == Register) {
        unsigned short* reg = getFullRegister(arg1, cpu);
        if (arg2t == Register) {
            *reg = *getFullRegister(arg2, cpu);
            return "";
        } else if(arg2t == HalfRegister) {
            *reg = *getHalfRegister(arg2, cpu);
            return "";
        } else if(arg2t == Immediate) {
            *reg = getImmediateShort(arg2);
        }
    } else if (arg1t == HalfRegister) {
        unsigned char* reg = getHalfRegister(arg1, cpu);
        if (arg2t == HalfRegister) {
            *reg = *getHalfRegister(arg2, cpu);
        } else if(arg2t == Immediate) {
            *reg = getImmediateByte(arg2);
        } else {
            return "Half registers only support byte operations.";
        }
    }

    return "";

}

QString parseLine(QString& line, CPU& cpu)
{
    int comment = line.indexOf(';');
    if (comment != -1) {
        line = line.left(comment);
    }
    if (!line.size()) {
        return "";
    }

    QString opname = line.left(line.indexOf(' '));
    QStringList args = line.right(line.size()-opname.size()).trimmed().split(',');

    if (args.size() != 2) {
        return "Invalid number of arguments.";
    }

    QString arg1 = args[0].trimmed();
    QString arg2 = args[1].trimmed();

    if (getArgumentType(arg1) == ArgumentType::Invalid) {
        return "Argument 1 is malformed.";
    }
    if (getArgumentType(arg1) == ArgumentType::Immediate) {
        return "Argument 1 must be a register.";
    }
    if (getArgumentType(arg2) == ArgumentType::Invalid) {
        return "Argument 2 is malformed.";
    }

    qDebug() << "opname: " << opname;
    QString result;
    if (opname == "mov") {
        result = parseMov(arg1, arg2, cpu);
    } else if (opname == "add") {
        result = parseAdd(arg1, arg2, cpu);
    } else if (opname == "sub") {
        result = parseSub(arg1, arg2, cpu);
    } else {
        return QString("Unsupported opcode %1").arg(opname);
    }

    if (result.size() > 0) {
        return result;
    }
    qDebug() << "arg1: " << arg1;
    qDebug() << "arg2: " << arg2;

    return "";
}

QString compileProgram(QString progstring, CPU& cpu)
{
    QVector<char> program;
    int lineNum = 0;
    foreach( QString str, progstring.split('\n'))
    {
        lineNum++;
        QString result = parseLine(str, cpu);
        if (result.size() > 0) {
            return QString("Error on line %1: %2")
                    .arg(lineNum)
                    .arg(result);
        }
    }

    return "";
}


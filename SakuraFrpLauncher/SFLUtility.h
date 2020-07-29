﻿#pragma once
#ifndef SFLUTILITY_H
#define SFLUTILITY_H

#include <QObject>
#include <QString>

class SFLUtility : public QObject
{

    Q_OBJECT

public:

    SFLUtility();
    ~SFLUtility();

public:

    static void UnicodeTransfrom(
        QString& str
    );

    static QString NextUuid(
    );

private:

};

#endif // SFLUTILITY_H

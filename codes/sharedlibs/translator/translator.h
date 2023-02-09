#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QtCore>
#include "translator_global.h"

class TRANSLATOR_EXPORT Translator {
    public:
        enum Lang { NONE, DEFAULT };
        Translator(const Lang &l = Lang::NONE);

        void translate(QString &pattern, const Lang &l = Lang::NONE);
        QString translate(const QString &pattern, const Lang &l = Lang::NONE);

    private:
        Lang defLang;
};

#endif // TRANSLATOR_H

#include "translator.h"

Translator::Translator(const Lang &l) {
    if(l != Lang::DEFAULT) defLang = l;
    else defLang = Lang::NONE;
}

QString Translator::translate(const QString &pattern, const Lang &l) {
    switch(l) {
        case Lang::NONE:
            return pattern;
        default:
            return "";
    }
}

void Translator::translate(QString &pattern, const Lang &l) {
    switch(l) {
        case Lang::NONE:
            break;
        default:
            break;
    }
}

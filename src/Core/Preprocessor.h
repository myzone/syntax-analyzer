#pragma once

#include <QString>
#include "ErrorEventBroadcaster.h"
#include "Enum.h"

namespace Core {

    class Preprocessor {
    private:

        class Directive : public Enum<QRegExp> {
        protected:

            Directive(const QRegExp& value) : Enum<QRegExp>(value) {
            }

        public:
            static const Directive IMPORT;
        };

        QString pathToLibrary;

        static const QRegExp TEXT_SPLITTER;
        static const QRegExp LINE_SPLITTER;
        static const QRegExp COMMENTS_REMOVER;

    public:
        Preprocessor(const QString& pathToLibrary);
        virtual ~Preprocessor();

        QString process(const QString& source);

    private:
        void removeComments(QString& source);
        void import(QString& source, const QString& importName);


    };

}
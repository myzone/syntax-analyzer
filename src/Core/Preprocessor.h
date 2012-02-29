#pragma once

#include <QString>

#include "../Events/EventBroadcaster.h"
#include "../Events/Event.h"
#include "../Utils/Enum.h"
#include "../Core/Exeption.h"

#include "../defines.h"

namespace Core {

    class Preprocessor {
    private:

        class Directive : public Enum<QString> {
        protected:

            Directive(const QString& value) : Enum<QString>(value) {
            }

        public:
            static const QString IDENTYFIER_STRING;
            static const QString SPACE_STRING;
            static const QString END_OF_STRING;

            static const Directive IMPORT;

            QRegExp getMatcherRegExp() const {
                return QRegExp(value + SPACE_STRING + IDENTYFIER_STRING);
            }

            QRegExp getRemoverRegExp(const QString& identifier) const {
                return QRegExp(value + SPACE_STRING + identifier + END_OF_STRING);
            }
        };

        QString pathToLibrary;
        Events::EventBroadcaster* broadcaster;

        static const QString FILE_FORMAT_MASK;
        
        static const QRegExp TEXT_SPLITTER;
        static const QRegExp LINE_SPLITTER;
        static const QRegExp COMMENTS_REMOVER;
        static const QRegExp SPACES_REMOVER;

    public:
        Preprocessor(const QString& pathToLibrary);
        virtual ~Preprocessor();

        QString process(const QString& source) const throws(AnalyzeCrashExeption);

    private:
        void removeComments(QString& source) const;
        void import(QString& source, const QString& importName) const throws(AnalyzeCrashExeption);

    };

}
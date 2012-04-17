#pragma once

#include <QString>
#include <QList>
#include <QSet>

#include "../Events/EventBroadcaster.h"
#include "../Events/Event.h"
#include "../Utils/Enum.h"
#include "../Core/Symbol.h"
#include "../Core/Exeption.h"

#include "../defines.h"

namespace Core {

    class Preprocessor {
    private:
        Events::EventBroadcaster* broadcaster;
        QString pathToLibrary;
        
        mutable QSet<QString> allreadyImported;
    public:
        static const QString FILE_FORMAT_MASK;
        static const QString IMPORT_DERECTIVE;
        
        Preprocessor(Events::EventBroadcaster* broadcaster, const QString& pathToLibrary);
        virtual ~Preprocessor();

        QList<Symbol> process(const QString& source) const throws(AnalyzeCrashExeption);
    private:
        QList<Symbol> import(const QString& importName) const throws(AnalyzeCrashExeption);

        File* openFile(const QString& pathToFile) const;
    };

}
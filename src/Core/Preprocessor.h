#pragma once

#include <QString>
#include <QList>

#include "../Events/EventBroadcaster.h"
#include "../Events/Event.h"
#include "../Utils/Enum.h"
#include "../Core/Symbol.h"
#include "../Core/Exeption.h"

#include "../defines.h"

namespace Core {

    class Preprocessor {
    private:
        QString pathToLibrary;
        const Events::EventBroadcaster* broadcaster;
        
    public:
        static const QString FILE_FORMAT_MASK;
        static const QString IMPORT_DERECTIVE;
        
        Preprocessor(Events::EventBroadcaster* broadcaster, const QString& pathToLibrary);
        virtual ~Preprocessor();

        QList<Symbol> process(const QString& source) const throws(AnalyzeCrashExeption);
    private:
        QList<Symbol> import(const QString& importName) const throws(AnalyzeCrashExeption);

    };

}
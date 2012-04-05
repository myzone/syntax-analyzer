#include <QTextStream>
#include <QString>
#include <QRegExp>
#include <QString>

#include "../Core/Preprocessor.h"
#include "../Core/SyntaxTreeFactory.h"

namespace Core {

    const QString Preprocessor::FILE_FORMAT_MASK = ".lng";
    const QString Preprocessor::IMPORT_DERECTIVE = "import";

    Preprocessor::Preprocessor(Events::EventBroadcaster* broadcaster, const QString& pathToLibrary) : broadcaster(broadcaster), pathToLibrary(pathToLibrary) { }

    Preprocessor::~Preprocessor() { }

    QList<Symbol> Preprocessor::process(const QString& source) const throws(AnalyzeCrashExeption) {
        QList<Symbol> libraryResult = QList<Symbol > ();
        QList<Symbol> currentResult = QList<Symbol > ();

        SymbolFactory symbolFactory = SymbolFactory(source);
        while (symbolFactory.isNextSymbol()) {
            Symbol current = symbolFactory.getNextSymbol();

            if (current.getType() == Symbol::SymbolType::IDENTYFIER
                    && current.getRepresentation() == IMPORT_DERECTIVE) {
                symbolFactory.getNextSymbol(); // skip AND symbol

                libraryResult += import(symbolFactory.getNextSymbol().getRepresentation());

                if (symbolFactory.getNextSymbol().getType() != Symbol::SymbolType::DEFINE_END) {
                    throws AnalyzeCrashExeption();
                }

            } else {
                currentResult += current;
            }
        }
        
        return currentResult + libraryResult;
    }

    QList<Symbol> Preprocessor::import(const QString& importName) const throws(AnalyzeCrashExeption) {
        File* fileDescriptor = null;

        // search lib file in current folder 
        if (!fileDescriptor) fileDescriptor = fopen((importName + FILE_FORMAT_MASK).toLocal8Bit(), "rt");
        // search lib file in libraries folder
        if (!fileDescriptor) fileDescriptor = fopen((pathToLibrary + importName + FILE_FORMAT_MASK).toLocal8Bit(), "rt");
        if (!fileDescriptor) {
            Events::LibraryFileCannotBeFoundErrorEvent event = Events::LibraryFileCannotBeFoundErrorEvent(importName);
            event.share(*broadcaster);
            throws AnalyzeCrashExeption(importName);
        }

        QFile file;
        file.open(fileDescriptor, QFile::ReadOnly | QFile::Text);

        QList<Symbol> result = QList<Symbol > ();
        SymbolFactory symbolFactory = SymbolFactory(file.readAll());

        file.close();

        while (symbolFactory.isNextSymbol()) {
            result += symbolFactory.getNextSymbol();
        }

        return result;
    }

}
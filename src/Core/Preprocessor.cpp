#include <QTextStream>
#include <QString>
#include <QRegExp>
#include <QString>

#include "../Core/Preprocessor.h"
#include "../Core/Symbol.h"
#include "../Core/SyntaxTreeFactory.h"

namespace Core {

    const QString Preprocessor::FILE_FORMAT_MASK = ".lng";
    const QString Preprocessor::IMPORT_DERECTIVE = "import";

    Preprocessor::Preprocessor(Events::EventBroadcaster* broadcaster, const QString& pathToLibrary) : broadcaster(broadcaster), pathToLibrary(pathToLibrary) { }

    Preprocessor::~Preprocessor() { }

    QList<Symbol> Preprocessor::process(const QString& source) const throws(AnalyzeCrashExeption) {
        QList<Symbol> libraryResult = QList<Symbol > ();
        QList<Symbol> currentResult = QList<Symbol > ();

        /*
         * Hack. Need to fix;
         */
        QString temp = source;
        temp = temp.replace(QRegExp("(//[^\n]*\n)|(/\\*.*\\*/)"),"");

        SymbolFactory symbolFactory = SymbolFactory(temp);
        while (symbolFactory.isNextSymbol()) {
            Symbol current;

            try {
                current = symbolFactory.getNextSymbol();
            } catch (AnalyzeCrashExeption exeption) {
                Events::SymbolIsNotClosedErrorEvent event = Events::SymbolIsNotClosedErrorEvent(exeption.getMessage());
                event.share(*broadcaster);

                throw;
            }

            try {
                if (current.getType() == Symbol::SymbolType::IDENTYFIER
                        && current.getRepresentation() == IMPORT_DERECTIVE) {

                    symbolFactory.getNextSymbol(); // skip AND symbol
                    Symbol importSymbol = symbolFactory.getNextSymbol();

                    if (importSymbol.getType() != Symbol::SymbolType::IDENTYFIER 
                                || allreadyImported.contains(importSymbol.getRepresentation())) {
                        Events::LibraryFileHasMistakeErrorEvent event = Events::LibraryFileHasMistakeErrorEvent(importSymbol.getRepresentation());
                        event.share(*broadcaster);

                        throw AnalyzeCrashExeption();
                    }

                    allreadyImported.insert(importSymbol.getRepresentation());
                    libraryResult += import(importSymbol.getRepresentation());
                    allreadyImported.remove(importSymbol.getRepresentation());

                    if (symbolFactory.getNextSymbol().getType() != Symbol::SymbolType::DEFINE_END)
                        throw AnalyzeCrashExeption();

                    continue;
                }
            } catch (WarningExeption) {
                throw AnalyzeCrashExeption();
            }

            currentResult += current;
        }

        return currentResult + libraryResult;
    }

    QList<Symbol> Preprocessor::import(const QString& importName) const throws(AnalyzeCrashExeption) {
        File* fileDescriptor = null;

        // search lib file in current folder 
        if (!fileDescriptor) fileDescriptor = openFile(importName + FILE_FORMAT_MASK);
        // search lib file in libraries folder
        if (!fileDescriptor) fileDescriptor = openFile(pathToLibrary + importName + FILE_FORMAT_MASK);
        if (!fileDescriptor) {
            Events::LibraryFileCannotBeFoundErrorEvent event = Events::LibraryFileCannotBeFoundErrorEvent(importName);
            event.share(*broadcaster);

            throw AnalyzeCrashExeption();
        }

        QFile file;
        file.open(fileDescriptor, QFile::ReadOnly | QFile::Text);

        QList<Symbol> result;

        try {
            result = process(file.readAll());

            file.close();
            fclose(fileDescriptor);
        } catch (AnalyzeCrashExeption) {
            Events::LibraryFileHasMistakeErrorEvent event = Events::LibraryFileHasMistakeErrorEvent(importName);
            event.share(*broadcaster);

            throw;
        }

        return result;
    }

    File* Preprocessor::openFile(const QString& pathToFile) const {
        return fopen(pathToFile.toLatin1(), "rt");
    }

}
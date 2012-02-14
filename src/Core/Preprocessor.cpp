#include <QTextStream>
#include <QString>
#include <QRegExp>

#include "Preprocessor.h"

namespace Core {

    const Preprocessor::Directive Preprocessor::Directive::IMPORT = Directive(QRegExp("import\\s+\\S+"));

    const QRegExp Preprocessor::TEXT_SPLITTER = QRegExp("\\s*;(\\s|\n)*");
    const QRegExp Preprocessor::LINE_SPLITTER = QRegExp("\\s+");
    const QRegExp Preprocessor::COMMENTS_REMOVER = QRegExp("(\\/\\/[^\n]*\n)|(\\/\\*.*\\*\\/)");
    const QRegExp Preprocessor::SPACES_REMOVER = QRegExp("[\n\t\\s]+");

    Preprocessor::Preprocessor(const QString& pathToLibrary) : pathToLibrary(pathToLibrary) {
    }

    Preprocessor::~Preprocessor() {
    }

    QString Preprocessor::process(const QString& source) {
        QString result = source;

        removeComments(result);

        QStringList lines = result.split(TEXT_SPLITTER, QString::SkipEmptyParts);

        for (int i = 0; i < lines.size(); i++) {
            if (((QRegExp) Directive::IMPORT).exactMatch(lines[i])) {
                import(result, lines[i].split(LINE_SPLITTER, QString::SkipEmptyParts)[1]);
            }
        }
        return result.replace(SPACES_REMOVER, "");
    }

    void Preprocessor::removeComments(QString& source) {
        source = source.replace(COMMENTS_REMOVER, "");
    }

    void Preprocessor::import(QString& source, const QString& importName) {
        QString path = pathToLibrary + importName;

        QFile file(path);
        file.open(QFile::ReadOnly | QFile::Text);

        source = source.replace(QRegExp("import\\s+" + importName + ";"), "");
        source = source + file.readAll();
    }

}
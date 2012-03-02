#pragma once

#include <QString>

#include "../defines.h"

#define DECLARE_EXEPTION_SUBCLASS(class_name) \
        class class_name : public Exeption {\
        public:\
            class_name();\
            class_name(const QString& message);\
            class_name(const class_name& that);\
            virtual ~class_name();\
        };

namespace Core {

    class Exeption {
    protected:
        QString message;

    public:
        Exeption();
        Exeption(const QString& message);
        Exeption(const Exeption& that);
        virtual ~Exeption();

        const QString& getMessage() const;
    };
    
    DECLARE_EXEPTION_SUBCLASS(AnalyzeCrashExeption);
    DECLARE_EXEPTION_SUBCLASS(WarningExeption);

}
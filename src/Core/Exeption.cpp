#include "Exeption.h"

#define IMPLEMENT_EXEPTION_SUBCLASS(class_name) \
        class_name::class_name() : Exeption() {} \
        class_name::class_name(const QString& message) : Exeption(message) {} \
        class_name::class_name(const class_name& that) : Exeption(that.message) {} \
        class_name::~class_name() {}

namespace Core {
    
    Exeption::Exeption() : message("") {}

    Exeption::Exeption(const QString& message) : message(message) {}
    
    Exeption::Exeption(const Exeption& that) : message(that.message) {}
    
    Exeption::~Exeption() {}

    const QString& Exeption::getMessage() const {
        return message;
    }
    
    IMPLEMENT_EXEPTION_SUBCLASS(AnalyzeCrashExeption);
    IMPLEMENT_EXEPTION_SUBCLASS(WarningExeption);
    
}
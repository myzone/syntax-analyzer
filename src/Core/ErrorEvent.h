#pragma once

#include <QString>

#define DECLARE_SHARE_METHOD(class_name) \
    void share(const ErrorEventBroadcaster& broadcaster) const;

#define DECLARE_ERROR_EVENT_SUBCLASS(class_name) \
    class class_name : public ErrorEvent {\
    public:\
        class_name(const QString& symbol);\
        virtual ~class_name();\
        DECLARE_SHARE_METHOD(class_name);\
    };

namespace Core {

    class ErrorEventBroadcaster;

    class ErrorEvent {
    private:
        QString symbol;
    public:
        ErrorEvent(const QString& symbol);
        virtual ~ErrorEvent();
        const QString& getSymbol() const;

        DECLARE_SHARE_METHOD(ErrorEvent);
    };

    DECLARE_ERROR_EVENT_SUBCLASS(FatalErrorEvent);
    DECLARE_ERROR_EVENT_SUBCLASS(WarningEvent);

}
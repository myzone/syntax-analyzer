#pragma once

#include "Event.h"

#ifndef pure
    #define pure =0
#endif

#define DECLARE_PURE_HANDLE_METHOD(class_name) \
        virtual void handle(const class_name& event) pure;

namespace Events{

    class EventListener {
    public:
        EventListener();
        virtual ~EventListener();

        DECLARE_PURE_HANDLE_METHOD(Event);

        DECLARE_PURE_HANDLE_METHOD(FatalErrorEvent);
        DECLARE_PURE_HANDLE_METHOD(WarningEvent);
        DECLARE_PURE_HANDLE_METHOD(ErrorEvent);
        
        DECLARE_PURE_HANDLE_METHOD(DoubleDefenitionErrorEvent);
        DECLARE_PURE_HANDLE_METHOD(SymbolIsNotDefinedErrorEvent);
    };

}
#pragma once

#include "Event.h"

#include <iostream>
#define DECLARE_HANDLE_METHOD(class_name) \
        virtual void handle(const class_name& event) { std::cout<<#class_name<<"\n"; }

namespace Events {

    class EventListener {
    public:
        EventListener();
        virtual ~EventListener();
        
        /**
         * @brief Don't implement it;
         */
        DECLARE_HANDLE_METHOD(Event);
        /**
         * @brief Don't implement it;
         */
        DECLARE_HANDLE_METHOD(ErrorEvent);

        DECLARE_HANDLE_METHOD(AnalysingWasStartedEvent);
        DECLARE_HANDLE_METHOD(AnalysingWasEndedEvent);

        DECLARE_HANDLE_METHOD(LibraryFileCannotBeFoundErrorEvent);
        DECLARE_HANDLE_METHOD(LibraryFileHasMistakeErrorEvent);

        DECLARE_HANDLE_METHOD(DoubleDefenitionErrorEvent);
        DECLARE_HANDLE_METHOD(SymbolIsNotUsedErrorEvent);
        DECLARE_HANDLE_METHOD(SymbolIsNotDefinedErrorEvent);
        DECLARE_HANDLE_METHOD(SymbolHasMistakeErrorEvent);
        DECLARE_HANDLE_METHOD(WrongSymbolDefinitionErrorEvent);

        DECLARE_HANDLE_METHOD(SymbolIsNotClosedErrorEvent);
        DECLARE_HANDLE_METHOD(WrongBracketsNumberErrorEvent);
    };

}
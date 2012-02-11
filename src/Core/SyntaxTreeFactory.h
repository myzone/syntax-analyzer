#pragma once

#include <QString>

#include "Tree.h"
#include "ErrorEventBroadcaster.h"

namespace Core {

    class SyntaxTreeFactory {
    private:
        ErrorEventBroadcaster broadcaster;

    public:
        SyntaxTreeFactory();
        SyntaxTreeFactory(const SyntaxTreeFactory& orig);
        virtual ~SyntaxTreeFactory();

        Tree<QString> createTree(QString string);

        const ErrorEventBroadcaster& getErrorEventBroadcaster() const;
        void addErrorEventListener(ErrorEventListener* listener);
        void removeErrorEventListener(ErrorEventListener* listener);
    };

}
#include "SyntaxTreeFactory.h"

namespace Core {

    SyntaxTreeFactory::SyntaxTreeFactory() {
        broadcaster = ErrorEventBroadcaster();
    }

    SyntaxTreeFactory::SyntaxTreeFactory(const SyntaxTreeFactory& that) {
        this->broadcaster = broadcaster;
    }

    SyntaxTreeFactory::~SyntaxTreeFactory() {
    }

    Tree<QString> createTree(QString string) {
        Tree<QString> tree = Tree<QString>();
        
        return tree;
    }
    
    
    const ErrorEventBroadcaster& SyntaxTreeFactory::getErrorEventBroadcaster() const {
        return broadcaster;
    }

    void SyntaxTreeFactory::addErrorEventListener(ErrorEventListener* listener) {
        broadcaster.addErrorEventListener(listener);
    }

    void SyntaxTreeFactory::removeErrorEventListener(ErrorEventListener* listener) {
        broadcaster.removeErrorEventListener(listener);
    }

}

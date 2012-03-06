/*
 * File:   main.cpp
 * Author: myzone
 *
 * Created on 1 February 2012 y.
 */

#define _DEBUG_MODE_

#include "Utils/Tree.h"
#include "Core/Preprocessor.h"
#include "Core/SyntaxTreeFactory.h"
#include "Core/Exeption.h"

#include "defines.h"
#include <string>


#include <iostream>

using namespace Core;

class ToDOT : public Tree<Symbol>::DataProcessor {
private:
    File* outFile;
    unsigned int i;
public:

    virtual void dataProcessingStarts() {
        outFile = fopen("/home/myzone/Рабочий стол/tree.dot", "wt");
        fprintf(outFile, "digraph G {\n");
    }

    virtual void dataProcessingEnds() {
        fprintf(outFile, "}\n");
    }

    virtual void processData(Tree<Symbol>& nodeProvider) {
        if (!nodeProvider.isRoot()) {
            fprintf(outFile, "\t%s -> %s\n", nodeProvider.getSupertree().get().getId().toStdString().c_str(), nodeProvider.get().getId().toStdString().c_str());
            fprintf(outFile, "\t%s [label=\"%s\"]\n",
                    nodeProvider.getSupertree().get().getId().toStdString().c_str(),
                    QString(nodeProvider.getSupertree().get().getRepresentation()).replace("\"", "\\\"").toStdString().c_str()
                    );
            fprintf(outFile, "\t%s [label=\"%s\"]\n",
                    nodeProvider.get().getId().toStdString().c_str(),
                    QString(nodeProvider.get().getRepresentation()).replace("\"", "\\\"").toStdString().c_str()
                    );
        }
    }

    virtual const TraverseType & getTraverseType() const {
        return TraverseType::WIDTH_TRAVERSE;
    }
};

class Listener : public Events::EventListener {

    virtual void handle(const Events::SymbolIsNotUsedErrorEvent& event) {
        std::cout << event.getRepresentation().toStdString() << "\n";
    }
};

void test1() {

    Preprocessor proc = Preprocessor("/home/myzone/Рабочий стол/");
    std::cout << proc.process("import lib;import lib;main->;\n").toStdString() << "\n";
}

void test4() {

    Events::EventBroadcaster b;
    ToDOT dot;

    b.addEventListener(new Listener());

    SyntaxTreeFactory f = SyntaxTreeFactory(&b);

    QFile file;
    file.open(fopen("/home/myzone/Рабочий стол/self.lng", "rt"), QFile::ReadOnly | QFile::Text);

    QString def = "";
    def += file.readAll();
    std::cout << def.toStdString() << "\n";


    Tree<Symbol> t = f.createTree(def);
    t.traverse(dot);
}

int main() {
    std::cout << "Start\n";

    test4();

    std::cout << "End\n";
    return 0;
}

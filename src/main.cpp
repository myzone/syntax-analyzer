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
#include "Core/TreeAnalyzer.h"

#include "defines.h"
#include <string>


#include <iostream>

using namespace Core;

class ToDOT : public Tree<Symbol>::DataProcessor {
private:
    mutable File* outFile;
    mutable unsigned int i;
public:

    virtual void dataProcessingStarts() const {
        outFile = fopen("/home/myzone/Рабочий стол/tree.dot", "w");
        fprintf(outFile, "digraph G {\n");
    }

    virtual void dataProcessingEnds() const {
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

    virtual void handle(const Events::WrongBracketsNumberErrorEvent& event) {
        std::cout << event.toString().toStdString() << ":" << event.getRepresentation().toStdString() << "|\n";
    }

    virtual void handle(const Events::LitheralIsNotClosedErrorEvent& event) {
        std::cout << event.toString().toStdString() << ":" << event.getRepresentation().toStdString() << "|\n";
    }

    virtual void handle(const Events::SymbolIsNotUsedErrorEvent& event) {
        std::cout << event.toString().toStdString() << ":" << event.getRepresentation().toStdString() << "|\n";
    }

    virtual void handle(const Events::SymbolIsNotDefinedErrorEvent& event) {
        std::cout << event.toString().toStdString() << ":" << event.getRepresentation().toStdString() << "|\n";
    }

    virtual void handle(const Events::SymbolHasMistakeErrorEvent& event) {
        std::cout << event.getRepresentation().toStdString() << "[style=filled,color=red]\n";
    }
};

void test1() {

    Preprocessor proc = Preprocessor("/home/myzone/Рабочий стол/");
    std::cout << proc.process("import lib;import lib;main->;\n").toStdString() << "\n";
}

char* fuckThisShit(const int& v) {
    char* res = new char[10];
    sprintf(res, "%d", v);

    return res;
}

class Proc : public Tree<char*>::DataProcessor, public Tree<int>::DataProcessor {

    void processData(Tree<char*>& node) {
        std::cout << "char*:" << node.get() << "\n";
    }

    void processData(Tree<int>& node) {
        std::cout << "int:" << node.get() << "\n";
    }

    const TraverseType& getTraverseType() const {
        return TraverseType::WIDTH_TRAVERSE;
    }

};

void test2() {
    Tree<int> t1;
    t1[0] = 5;
    t1[1][1] = 6;
    t1.traverse(*new Proc());

    Tree<char*> t2 = t1.to<char*>(&fuckThisShit);
    t2.traverse(*new Proc());
}

void test4() {

    Events::EventBroadcaster b;
    ToDOT dot;

    b.addEventListener(new Listener());

    QFile file;
    file.open(fopen("/home/myzone/Рабочий стол/self.lng", "rt"), QFile::ReadOnly | QFile::Text);

    QString def = "";
    def += file.readAll();


    SyntaxTreeFactory f = SyntaxTreeFactory(&b);
    Tree<Symbol> t = f.createTree(def);
    t.traverse(dot);
    //TreeAnalyzer an = TreeAnalyzer(&b);
    //an.analyzeTree(t);
}


int main() {
    std::cout << "Start\n";

    test4();

    std::cout << "End\n";
    return 0;
}

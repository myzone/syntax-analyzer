/*
 * File:   main.cpp
 * Author: myzone
 *
 * Created on 1 February 2012 y., 18:03
 */


#include "Utils/Tree.h"
#include "Core/Preprocessor.h"
#include "Core/SyntaxTreeFactory.h"
#include "Core/Exeption.h"


#include <iostream>
#define cout std::cout

using namespace Core;

class ToDOT : public Tree<Symbol>::DataProcessor {
private:
    File* out;
public:

    virtual void dataProcessingStarts() {
        out = stdout; //fopen("tree.dot", "wt");

        fprintf(out, "datagram G {\n");
    }

    virtual void dataProcessingEnds() {
        fprintf(out, "}");
    }

    virtual void processData(Tree<Symbol>& nodeProvider) throws(Tree<Symbol>::TraverseStoppedExeption) {
        if (!nodeProvider.isRoot()) {
            fprintf(out, "\t%s -> %s\n", nodeProvider.getSuperTree().get().toString().toStdString().c_str(), nodeProvider.get().toString().toStdString().c_str());
        }
    }

    virtual void processData(const Tree<Symbol>& nodeProvider) const throws(Tree<Symbol>::TraverseStoppedExeption) {
    }

    virtual const TraverseType & getTraverseType() const {
        return TraverseType::WIDTH_TRAVERSE;
    }
};

void test1() {
    Preprocessor proc = Preprocessor("/home/myzone/Рабочий стол/");
    cout << proc.process("import lib;import lib;main->;\n").toStdString() << "\n";

}

void test3() {
    Events::EventBroadcaster b;
    SyntaxTreeFactory f = SyntaxTreeFactory(&b);
    QList<Symbol> l = f.toPostfixSymbolsList("( hello\"+\\\"\"dude mother) | c");
    for (int i = 0; i < l.size(); i++) {
        cout << l[i].toString().toStdString() << " ";
    }
}

/*
    (asd & b) | a
     a asd b & a |
 */


void test4() {

    ToDOT dot;

    //SyntaxTreeFactory f = SyntaxTreeFactory(&b);
    //Tree<Symbol> t = f.createTree("main -> A; A->\"a\"");
    //t[0] = Symbol("asd", Symbol::SymbolType::IDENTYFIER);
    //t.walk(dot);
}

int main(void) {
    cout << "Start\n";

    test3();

    cout << "End\n";
    return 0;
}

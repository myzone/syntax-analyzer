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

void test1() {
    Preprocessor proc = Preprocessor("/home/myzone/Рабочий стол/");
    cout << proc.process("import lib;import lib;main->;\n").toStdString() << "\n";

}

void test2() {

    class A : public Tree<QString>::IDataProcessor {
    private:
        mutable int i;
    public:

        virtual void dataProcessingStarts() {
            i = 0;
        }

        virtual void dataProcessingEnds() {

        }

        virtual void processData(Tree<QString>::DataProvider& nodeProvider) {
            cout << i++ << " " << nodeProvider.get().toStdString().c_str() << " ";
            if (nodeProvider.isLeaf()) {
                cout << "leaf";
            }
            cout << "\n";
        }

        virtual void processData(const Tree<QString>::DataProvider& nodeProvider) const {
        }

        virtual const TraverseType& getTraverseType() const {
            return TraverseType::DEPTH_TRAVERSE;
        }
    };

    A a = A();

    Tree<QString> t1 = Tree<QString > ();
    Tree<QString> t2 = Tree<QString > ();
    Tree<QString> t3 = Tree<QString > ();

    t1 = "t1";

    t2 = "t2";
    t2[0] = t1;

    t3 = "t3";
    t3[0] = t2;
    t3[1] = t1;

    t1[0] = "t1[0]";
    t1[0][1] = "t1[0][1]";
    t1[1] = "t1[1]";
    t1[1][0] = "t1[1][0]";
    t1[1][1] = "t1[1][1]";

    t3.walk(a);

}

void test3() {
    SymbolFactory f = SymbolFactory("( a \"+\" b ) | c");

    while (f.isNextSymbol()) {
        cout << f.getNextSymbol().getString().toStdString() << ".";
    }
}

void test4() {

}

int main(void) {
    cout << "Start\n";

    test3();
    cout << "\n";
    test3();
    cout << "\n";
    test4();

    cout << "End\n";
    return 0;
}

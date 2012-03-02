/*
 * File:   main.cpp
 * Author: myzone
 *
 * Created on 1 February 2012 y., 18:03
 */
#define MYZONES_DEBUG_MODE

#include "Utils/Tree.h"
#include "Core/Preprocessor.h"
#include "Core/SyntaxTreeFactory.h"
#include "Core/Exeption.h"


#include "defines.h"
#include <string>

using namespace Core;

class ToDOT : public Tree<Symbol>::DataProcessor {
private:
    File* outFile;
    unsigned int i;
public:

    virtual void dataProcessingStarts() {
        outFile = stdout; //fopen("tree.dot", "wt");

        fprintf(outFile, "digraph G {\n");
    }

    virtual void dataProcessingEnds() {
        fprintf(outFile, "}\n");
    }

    virtual void processData(Tree<Symbol>& nodeProvider) throws(Tree<Symbol>::TraverseStoppedExeption) {
        if (!nodeProvider.isRoot()) {
            std::cout << "\t" << nodeProvider.getSuperTree().get().getId().toStdString() << " -> " << nodeProvider.get().getId().toStdString() << "\n";
            std::cout << "\t" << nodeProvider.getSuperTree().get().getId().toStdString()
                    << " [label=\"" << QString(nodeProvider.getSuperTree().get().toString()).replace("\"", "\\\"").toStdString() << "\"]\n";
            std::cout << "\t" << nodeProvider.get().getId().toStdString() << " [label=\""
                    << QString(nodeProvider.get().toString()).replace("\"", "\\\"").toStdString() << "\"]\n";
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
    std::cout << proc.process("import lib;import lib;main->;\n").toStdString() << "\n";
}

void test2(Tree<std::string> tree) {

    tree = "tree";
    std::cout << tree.get() << "\n";

    // tree[1][1] = "tree[1][1]";

    tree = tree[1];
    tree[1][1] = "tree[1][1]";
    //tree_1[1][1] = "tree_1[1][1]";

    //   std::cout << "tree "<<(tree.getSuperTree())[1][1].get().toString().toStdString() << "\n";
}

/*
    (asd & b) | a
     a asd b & a |
 */


void test4() {
    Events::EventBroadcaster b;
    ToDOT dot;

    SyntaxTreeFactory f = SyntaxTreeFactory(&b);

    QFile file;
    file.open(fopen("/home/myzone/a", "rt"), QFile::ReadOnly | QFile::Text);

    QString def = "";
    def += file.readAll();

    //std::cout << def.toStdString() << "\n";

    Tree<Symbol> t = f.createTree(def);
    t.walk(dot);
}

int main(void) {
    std::cout << "Start\n";

    test4();

    std::cout << "End\n";
    return 0;
}

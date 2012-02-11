/*
 * File:   main.cpp
 * Author: myzone
 *
 * Created on 1 February 2012 y., 18:03
 */


#include "../src/Core/Preprocessor.h"
#include "Core/Tree.h"

#include <iostream>
#define cout std::cout

using namespace Core;

void test1() {
    Preprocessor proc = Preprocessor("/home/myzone/Рабочий стол/");
    cout << proc.process("import lib;import lib;main->;\n").toStdString() << "\n";

}

void test2() {


}

int main(void) {
    test2();

    return 0;
}

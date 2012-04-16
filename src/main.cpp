/*
 * File:   main.cpp
 * Author: myzone
 *
 * Created on 1 February 2012 y.
 */

#define _DEBUG_MODE_

#include <QApplication>
#include <QMainWindow>

#include "Utils/Tree.h"
#include "Core/Preprocessor.h"
#include "Core/SyntaxTreeFactory.h"
#include "Core/Exeption.h"
#include "Core/TreeAnalyzer.h"
#include "Core/Analyzer.h"

#include "defines.h"

#include "GUI/TextTabWidget.h"
#include "GUI/MainWindow.h"

#include <string>
#include <iostream>

class A : public Events::EventListener {
public:

    A() : EventListener() { }

    void handle(const Events::WrongSymbolDefinitionErrorEvent& e) {
        std::cout << e.toString().toStdString() << "\n";
    }
};

int main_() {
    Core::Analyzer an("/home/myzone/Рабочий стол/");
    an.addErrorEventListener(new A());

    an.analyze(
            "main -> a;\n"
            "a -> \"a\";\n"
            );
    
    return 0;
}

int main(int argc, char** argv) {

    QApplication app(argc, argv);

    GUI::MainWindow mainWindow;
    mainWindow.resize(640, 480);
    mainWindow.show();

    return app.exec();
}

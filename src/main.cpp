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

using namespace Core;

int main(int argc, char** argv) {
    /*std::cout << "Start\n";

    Analyzer an = Analyzer("/home/myzone/Рабочий стол/");
    an.addErrorEventListener(new Events::EventListener());
    
    QFile file;
    file.open(fopen("/home/myzone/Рабочий стол/self.lng", "rt"), QFile::ReadOnly | QFile::Text);

    an.analyze(file.readAll());

    std::cout << "End\n";
    return 0;*/
    QApplication app(argc, argv);
    
    MainWindow mainWindow;
    mainWindow.resize(640, 480);
    mainWindow.show();          
    
    return app.exec();
}

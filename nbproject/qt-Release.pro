# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = Syntax_Analyzer
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui
SOURCES += src/main.cpp src/Core/TreeAnalyzer.cpp src/Events/EventBroadcaster.cpp src/Core/Analyzer.cpp src/Utils/Tree.cpp src/Events/EventListener.cpp src/Core/SyntaxTreeFactory.cpp src/Events/Event.cpp src/Core/Exeption.cpp src/Core/Preprocessor.cpp
HEADERS += src/defines.h src/Events/Event.h src/Core/Analyzer.h src/Utils/Tree.h src/Core/TreeAnalyzer.h src/Core/SyntaxTreeFactory.h src/Core/Preprocessor.h src/Core/Exeption.h src/Events/EventBroadcaster.h src/Events/EventListener.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 

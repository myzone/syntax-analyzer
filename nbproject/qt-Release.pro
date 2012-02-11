# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = Syntax_Analyzer
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui
SOURCES += src/Core/TreeAnalyzer.cpp src/main.cpp src/Core/ErrorEventBroadcaster.cpp src/Core/Tree.cpp src/Core/ErrorEvent.cpp src/Core/SyntaxTreeFactory.cpp src/Core/ErrorEventListener.cpp src/Core/Preprocessor.cpp
HEADERS += src/Core/ErrorEvent.h src/Core/ErrorEventBroadcaster.h src/Core/Tree.h src/Core/ErrorEventListener.h src/Core/TreeAnalyzer.h src/Core/SyntaxTreeFactory.h src/Core/Preprocessor.h
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

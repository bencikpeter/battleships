TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
TARGET = battleships.out

INCLUDEPATH += battleships/ \

include(battleships/battleships.pri)


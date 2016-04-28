TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
TARGET = battleships.out

LIBS += -lSDL2
LIBS += -lSDL2_net

INCLUDEPATH += battleships/ \
        /usr/include/SDL2 \
        /usr/local/include/SDL2 \

include(battleships/battleships.pri)


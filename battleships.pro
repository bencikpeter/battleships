TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
TARGET = battleships.out

LIBS += -L/usr/local/lib -lSDL2 -lSDL2_net -pthread

INCLUDEPATH += battleships/ \
        /usr/include/SDL2 \
        /usr/local/include/SDL2 \
        asio-1.10.6/include \
        asio-1.10.6/include/asio

include(battleships/battleships.pri)


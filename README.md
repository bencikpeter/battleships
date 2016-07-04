# battleships

To compile this game correctly do following steps:

1

clone this repo somewhere to your computer

2

install dependencies:

sudo apt-get install libsdl2-dev

sudo apt-get install libsdl2-ttf-dev

you will also need qmake, so the easiest way to get it is install Qt directly from https://www.qt.io

3

run Makefile in {your_path}/battleships/debug


PS: The Makefile uses standard c++14 and clang++. To change the compiler, 
you will have to change value of CXX variable in the Makefile on line 14

4 

now your game should be compiled, binary is {your_path}/battleships/debug/battleships.out

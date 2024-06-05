all: compile link

compile:
    g++ -c main.cpp -I"C:\SFML-2.6.1\include" -DSFML_STATIC

link:
    g++ main.o -o main -L"C:\SFML-2.6.1\lib" -lwinmm -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

clean:
    rm -f main *.o

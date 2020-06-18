#include <stdio.h>
#include "moving.h"
#include "reading.h"

int **jogo(int argc, char* argv[]){ // esta função serve para ir buscar o maze
    int **maze;

    clearscreen();  //Limpar o ecrã
    cbreak();       //Colocar o terminal no modo cbreak ou rare
    maze = ler(argc, argv);     //retorna a matriz
    nocbreak();     //Por o terminal de volta no modo cooked

    return maze;
}

int *posi(int **maze){ //esta função irá retornar a posição do cursor
    int *pos;
    
    pos = mapa(maze);    // retorna a posição
    return pos;
}

int *direcao(){ //esta função irá retornar a direção do cursor
    int *dir;

    dir = dirIni();

    return dir;
}

int move(int **maze, char *buf, int *pos){ //esta  função irá fazer o movimento do cursor no maze

    cbreak();
    andar(maze, buf, pos); // move o cursor
    nocbreak();

}

int moveb(int **maze, char *buf, int *pos, int *dir){ // esta função move o cursor no modo 3 teclas
    
    cbreak();
    andarb(maze, buf, pos, dir); // move o cursor
    nocbreak();
}
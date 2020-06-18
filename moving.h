void clearscreen();
void cbreak();
void nocbreak();

int *up(int **maze, int *pos);
int *down(int **maze, int *pos);
int *right(int **maze, int *pos);
int *left(int **maze, int *pos);

int *mapa(int **maze);

int andar(int **maze, char *buf, int *pos);

int *mudaDir(int *dir, int dire);
int *dirIni();
int andarb(int **maze, char *buf, int *pos, int *dir);
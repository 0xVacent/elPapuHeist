#ifndef MAPCREATOR_H_INCLUDED
#define MAPCREATOR_H_INCLUDED
#include <windows.h>

typedef struct indicator{
    int indicatorY;
    int indicatorX;
}indicator;

#define MY_BUFSIZE 1024
char pszOldWindowTitle[MY_BUFSIZE];
extern int fil;
extern int col;
extern char archivoMapas[20];
POINT cursorPos;
HWND hwndFound;


void iniciarCreador();
void fixMapa(char mapa[fil][col]);
void estrucuraBasica(char mapa[fil][col]);
void mostrarMapa(char mapa[fil][col], indicator * cursor);
void indicador(char mapa[fil][col], indicator * cursor);
void infoArchivo(char mapa[fil][col]);
void leerArchivo();

#endif // MAPCREATOR_H_INCLUDED

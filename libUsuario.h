#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef LIBUSUARIO_H_INCLUDED
#define LIBUSUARIO_H_INCLUDED

typedef struct
{
    char nombreJugador[30];
    char contraseniaJugador[30];
    int puntaje[100];
    int validos;
    int bajaLogica;
    int id;
    char admin;///s=si  n=no
} puntuacionJugador;

typedef struct nodoArbol
{
    puntuacionJugador dato;
    struct nodoArbol* izq;
    struct nodoArbol* der;
} nodoArbol;



nodoArbol* agregaPuntuacion(nodoArbol* arbol,puntuacionJugador jugador,int puntaje);
void cargarArchivoJugadores(char nombreArchivo[],puntuacionJugador persona);
int buscaJugadorEnArchivo(char nombreArchivo[],puntuacionJugador persona);
nodoArbol* crearNodoArbol(puntuacionJugador dato);
nodoArbol* insertarNodoArbol(nodoArbol* arbol, nodoArbol* nuevo);
void mostrarPuntaje (puntuacionJugador dato);
void mostrarInorder (nodoArbol* arbol);
void mostrarInorderActivos (nodoArbol* arbol);
void mostrarInorderBaja (nodoArbol* arbol);
void mostrarEspecifico(nodoArbol* arbol,int id);
nodoArbol* archivoArbol2(char nombreArchivo[],nodoArbol* arbol);
void inicJugador(puntuacionJugador* dato);
void nuevoUsuarioAlArchivo(char nombreArchivo[]);
int creaID(char nombreArchivo[]);
int iniciarSesion(char nombreArchivo[], puntuacionJugador jugador);
int iniciarSesionAdmin(char nombreArchivo[]);
void insertar(int arreglo[], int u, int dato);
void ordenacionInsercion (puntuacionJugador* dato);
nodoArbol* darDeBajaUsuario(nodoArbol* arbol,int id);
void guardarNodoEnArchivo(nodoArbol* nodo, FILE* archi);
void guardarArbolEnArchivo(nodoArbol* arbol,char nombreArchivo[]);


#endif // LIBUSUARIO_H_INCLUDED

//ACA INICIA EL CODIGO :3
#include "mapCreator.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "color.h"
#include "libUsuario.h"
//CREAMOS ESTRUCTURAS

typedef struct{
    int posY;
    int posX;
    int dir;
}proyectile;

typedef struct{
    int activa;
    int posY;
    int posX;
}arma;

typedef struct{
    int posY;
    int posX;
    int hp;
    int attack;
    int potion;
}player;

typedef struct Plantillas{
    char nivel;
    struct nodoPlantillas * listaMapas;
}plantillas;

typedef struct nodoPlantillas{
    char mapa[28][96];
    struct nodoPlantillas * sig;
}nodoPlantillas;

typedef struct{
    int posY;
    int posX;
    int id;
    int orientation;
    int* tickrate;
    int attack;
    int hp;
}npc;

typedef struct nodoPartida{
    int nivel;
    int sala;
    char mapaNodo[28][96];
    struct nodoPartida * sig;
}nodoPartida;

//DECLARAMOS CONSTANTES

const int filMostrar = 28;
const int colMostrar = 96;
char nombreArchivo[20] = "mapas.dat";

//VARIABLE GLOGAL
int fil = 28;
int col = 96;
int angulo = 0;
int balaCreada  = 0;
plantillas arregloPlantillas[3];
int currLevel = 0;
nodoPartida * partidaActual = NULL;
int bossCooldown = 5;
int bossAttackTimeLeft = 0;
int modoCaos = 0;
int bossHP = 10;
int iFrames = 0;
int puntosAgregados = 0;
int hp[30];
int playerShootinCooldown = 0;
int balaAcutal = 0;

//PROTOTIPADO
player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation);
void runnpcs(char mapa[fil][col],npc * enemy, int * tickRate, player jugador);
void fillMap(char mapa[fil][col], char mapaRandom[fil][col]);
void runPlayer(char mapa[fil][col], player jugador, int * currentOrietation);
void shootingEnemy(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet);
nodoPlantillas * crearNodoPlantillas(char mapa[fil][col]);
nodoPlantillas * agreagarAlPpioPlantillas(nodoPlantillas * lista, nodoPlantillas * nuevoNodo);
nodoPlantillas * inicarListaPlantilla();
void randomMapSelector(char mapaAux[fil][col], int currLevel);
nodoPartida * crearNodoPartida(char mapaAux[fil][col], int currLevel);
nodoPartida * agregarAlFinal(nodoPartida * lista, nodoPartida * nuevoNodo);
nodoPartida * buscarUltimo(nodoPartida * lista, int * sala);
void defensa(arma * armaAux, player jugador, char mapa[fil][col], int * tickrate);
void detectarColision(arma armaAux, npc * enemy, char mapa[fil][col], int * cantidadEnemigosActuales, player * jugador, proyectile balas[100]);
//MAIN

int main(){

    int eleccion=1;
    system("color");

    srand(time(NULL));
    arregloPlantillas[0].nivel = '1';
    arregloPlantillas[1].nivel = '2';
    arregloPlantillas[2].nivel = '3';
    char mapa[fil][col];
    player jugador;
    jugador.hp = 7;
    jugador.attack = 2;
    jugador.posY = 12;
    jugador.posX = 6;
    npc enemys[30];
    arma armaXD;
    armaXD.posX = 0;
    armaXD.posY = 0;
    proyectile balas[100];
    for(int i = 0; i<100;i++){
        balas[i].dir=NULL;
        balas[i].posX=NULL;
        balas[i].posY=NULL;
    }

    memset(mapa, 32, sizeof(mapa));

    int * currentOrientiation = 94;

    int * tickRate;
    int cantindadEnemigos;
    cantindadEnemigos = (currLevel + 1) * 5;
    int cantidadEnemigosActuales = cantindadEnemigos-1;

    printf("Bienvenido, ingrese 1 para comenzar el juego o 2 para ir al creador de mapas\n");
    fflush(stdin);
    scanf("%i", &eleccion);

    while(eleccion!=1 && eleccion!=2 && eleccion!=777)
    {
        printf("\nSu numero es incorrecto, porfavor ingrese 1,2 o 3\n");
        fflush(stdin);
        scanf("%i", &eleccion);
    }

    if(eleccion == 2){
        iniciarCreador();

    }

    if(eleccion == 777){
        modoCaos = 1;
        eleccion = 1;
    }

    system("cls");

    //Organizamos el mapa y sus cosas



    PlaySound("C:\\Users\\confi\\Desktop\\elPapuHeist-test\\level1.wav", NULL, SND_ASYNC);
    leerArchivoPlantilla();
    char randomMap[fil][col];
    randomMapSelector(randomMap, currLevel);
    nodoPartida * nuevaHabitacion = crearNodoPartida(randomMap, currLevel);
    partidaActual = agregarAlFinal(partidaActual,nuevaHabitacion);
    spawnearEnemigos(randomMap, enemys, cantindadEnemigos);
    int sala = 1;

    nodoPartida * ultimoNodo = buscarUltimo(partidaActual, &sala);
    system("cls");

    for(;;){
    printf("\x1b[H");
    system("");
    fillMap(mapa, randomMap);

    jugador = runGameplay(mapa, jugador, &currentOrientiation);
    defensa(&armaXD, jugador, mapa, &tickRate);
    for(int i = 0; i<100; i++){
    handlerBalas(&balas[i], mapa);
    }
    for(int i = 0; i<cantindadEnemigos-1; i++){
    detectarColision(armaXD, &enemys[i], mapa, &cantidadEnemigosActuales, &jugador, balas);
    runnpcs(mapa, &enemys[i], &tickRate, jugador);
    }
    runPlayer(mapa, jugador, &currentOrientiation);
    runMap(mapa, jugador, &tickRate,ultimoNodo);
    sistemaDeVida(&jugador, eleccion, mapa);
    disparoJugador(&jugador, balas);
    printf("%i", cantidadEnemigosActuales);
    if((mapa[jugador.posY][jugador.posY] == mapa[13][93] || mapa[jugador.posY][jugador.posY] == mapa[14][93]) && cantidadEnemigosActuales <= 0){
        if(currLevel != 2){
        if(ultimoNodo->sala >= 3){
            PlaySound(NULL, 0, 0);
            currLevel = currLevel + 1;
            PlaySound("C:\\Users\\confi\\Desktop\\elPapuHeist-test\\level2.wav", NULL, SND_ASYNC);
        }
        cantindadEnemigos = (currLevel + 1) * 5;
        cantidadEnemigosActuales = cantindadEnemigos-1;
        if(currLevel == 2){
            PlaySound(NULL, 0, 0);
            PlaySound("C:\\Users\\confi\\Desktop\\elPapuHeist-test\\FinalBoss.wav", NULL, SND_ASYNC);
            cantindadEnemigos = 2;
            cantidadEnemigosActuales = cantindadEnemigos-1;
        }
        pasarDeHabitacion(randomMap, partidaActual, enemys, cantindadEnemigos);

        jugador.posY = 12;
        jugador.posX = 6;
        }

    }
    if(currLevel == 2){
            if(GetKeyState('G') & 0x8000){
                system("cls");
                mostrarListaPartida(partidaActual);
                system("pause");
                return 0;
            }
    }tickRate = tickRate + 1;
    printf("        ");
    //printf("\x1b[J");

    if(bossHP <= 0){
        Sleep(1000);
        system("cls");
        PlaySound("C:\\Users\\confi\\Desktop\\elPapuHeist-test\\Ganaste.wav", NULL, SND_ASYNC);
        endScreen(mapa);
        system("pause");
        return 0;
    }

    }
    return 0;
}



//MUESTRA EL MAPA

void runMap(char mapa[fil][col], player jugador, int * tickRate, nodoPartida * ultimoNodo){
    int i, j;

    printf("%c------------------------------------------------------------------------------------------------%c\n", 218, 191);
    for(i=0 ; i<filMostrar;i++){
            printf("|");

        for(j=0; j<colMostrar; j++){
            printf("%c", mapa[i][j]);
        }
        printf("|");
        switch(i){
        case 0:
            printf(YEL"        ""BIENVENIDO A       "WHT);
            break;
        case 1:
            printf(YEL"    ""LA DUNGEON DEL PAPU    "WHT);
            break;
        case 3:
            printf(YEL"    ""NIVEL: %i""    ""SALA: %i"WHT, currLevel + 1, ultimoNodo->sala);
            break;

        case 8:
                if(currLevel == 2){
                printf(RED"        BOSS HP: %i "WHT, bossHP);
                }
                break;
        case 10:
                if(currLevel == 2){
                printf(RED"           _____"WHT);
                }
                break;
        case 11:
                if(currLevel == 2){
                printf(RED"          /     \\"WHT);
                }
                break;
        case 12:
                if(currLevel == 2){
                printf(RED"         | () () |"WHT);
                }
                break;
        case 13:
                if(currLevel == 2){
                printf(RED"          \\  ^  /"WHT);
                }
                break;
        case 14:
                if(currLevel == 2){
                printf(RED"           |||||"WHT);
                }
                break;
        case 15:
                if(currLevel == 2){
                printf(RED"           |||||"WHT);
                }
                break;
        case 18:
                printf(YEL"         PUNTOS: %i "WHT, puntosAgregados);
                break;
        case 19:
                printf(YEL"          VIDA: %i"WHT, jugador.hp);
        }
        printf("\n");
    }
    printf("%c------------------------------------------------------------------------------------------------%c", 192, 217);

}


// SISTEMA DE VIDA ( corazoncitos y espadas )

void sistemaDeVida (player * jugador, int eleccion, char mapa[fil][col], nodoArbol * arbol)
{
    if(jugador->hp>0)
    {
        printf("                \n");
        int i=0;
        while(i<jugador->hp && eleccion==1)
        {
            int hp[30];
            hp[i]=03;
            i++;

        }

    }
        if(jugador->hp == 0){
            Sleep(1000);
            system("cls");
            defeatScreen(mapa);
            PlaySound("C:\\Users\\confi\\Desktop\\elPapuHeist-test\\Perdiste.wav", NULL, SND_ASYNC);
            system("pause");
            exit(0);
        }
}


//MOVIMIENTO DEL JUGADOR

void runPlayer(char mapa[fil][col], player jugador, int * currentOrietation){
    mapa[jugador.posY][jugador.posX] = *currentOrietation;
}
player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation){
    if(GetKeyState('S') & 0x8000 && mapa[jugador.posY + 1][jugador.posX] != '#' && mapa[jugador.posY + 1][jugador.posX] !='$'){
        jugador.posY = jugador.posY + 1;
        mapa[jugador.posY - 1][jugador.posX] = ' ';
        *currentOrientation = 118;
    }
    if(GetKeyState('W') & 0x8000 && mapa[jugador.posY - 1][jugador.posX] != '#' && mapa[jugador.posY - 1][jugador.posX] !='$'){
        jugador.posY = jugador.posY - 1;
        mapa[jugador.posY + 1][jugador.posX] = ' ';
        *currentOrientation = 94;
    }
    if(GetKeyState('D') & 0x8000 && mapa[jugador.posY][jugador.posX + 1] != '#' && mapa[jugador.posY][jugador.posX+1] !='$'){
        jugador.posX = jugador.posX + 1;
        mapa[jugador.posY][jugador.posX - 1] = ' ';
        *currentOrientation = 62;
    }
    if(GetKeyState('A') & 0x8000 && mapa[jugador.posY][jugador.posX - 1] != '#' && mapa[jugador.posY][jugador.posX-1] !='$'){
        jugador.posX = jugador.posX - 1;
        mapa[jugador.posY][jugador.posX + 1] = ' ';
        *currentOrientation = 60;
    }

    if(GetKeyState('X') & 0x8000){
        mapa[jugador.posY][jugador.posX] = ' ';
        jugador.posY = 13;
        jugador.posX = 91;
    }

    return jugador;
}

//Cargamos la estrcuctura del mapa de una de las plantillas y se la cargamaos al mapa actual

void fillMap(char mapa[fil][col], char mapaRandom[fil][col]){
        int i,j;
        memcpy(mapa, mapaRandom, fil*col);


 //PAREDES BORDER NO BORRAR BAJO NINGUNA SIRCUNSTANCIA :3
    for(i = 0; i < fil; i++){
        for(j = 0; j<2; j++){
            mapa[i][j] = '#';
        }
    }
    for(i = 0; i < fil; i++){
        for(j = col - 2; j<col; j++){
            mapa[i][j] = '#';
        }
    }
    for(i = fil - 2; i < fil; i++){
        for(j = 0; j<col; j++){
            mapa[i][j] = '#';
        }
    }
    for(i = 0; i < 2; i++){
        for(j = 0; j<col; j++){
            mapa[i][j] = '#';
        }
    }

    mapa[13][93] = 175;
    mapa[14][93] = 175;

    if(currLevel == 2){
    mapa[13][93] = '#';
    mapa[14][93] = '#';

    }

}


//HACEMOS COSAS CON LOS GUARDIAS

void runnpcs(char mapa[fil][col],npc * enemy, int * tickRate, player jugador){

    if(currLevel == 2){
    bossHP = enemy->hp;
    if(bossHP <= 0){
        puntosAgregados = puntosAgregados + 500;
    }

    }
    if(enemy->hp > 0){

    if(enemy->id == 1){
    mapa[enemy->posY][enemy->posX] = '$';

    if(*tickRate % 5 == 0){

    if(enemy->orientation == 1){
    if(mapa[enemy->posY -1][enemy->posX] == '#' || mapa[enemy->posY -1][enemy->posX] == '$'){
        enemy->orientation = 3;
    }else{
    enemy->posY = enemy->posY -1;
    }
    if(mapa[enemy->posY+1][enemy->posX] != '#'){
    mapa[enemy->posY+1][enemy->posX] = ' ';
    }
   }
    if(enemy->orientation == 3){
    if(mapa[enemy->posY +1][enemy->posX] == '#' || mapa[enemy->posY +1][enemy->posX] == '$'){
        enemy->orientation = 1;
    }else{
    enemy->posY = enemy->posY +1;
    }
    if(mapa[enemy->posY-1][enemy->posX] != '#'){
    mapa[enemy->posY-1][enemy->posX] = ' ';
    }
    }
    if(enemy->orientation == 2){
    if(mapa[enemy->posY][enemy->posX +1] == '#' || mapa[enemy->posY ][enemy->posX+1] == '$'){
        enemy->orientation = 4;
    }else{
    enemy->posX = enemy->posX +1;
    }
    if(mapa[enemy->posY][enemy->posX -1] != '#'){
    mapa[enemy->posY][enemy->posX -1] = ' ';
    }
    }
    if(enemy->orientation == 4){
    if(mapa[enemy->posY][enemy->posX-1] == '#' || mapa[enemy->posY][enemy->posX-1] == '$'){
        enemy->orientation = 2;
    }else{
    enemy->posX = enemy->posX -1;
    }
    if(mapa[enemy->posY][enemy->posX +1] != '#'){
    mapa[enemy->posY][enemy->posX +1] = ' ';
    }
    }
    }
    if(*tickRate % rand()%15 == 0 && *tickRate != 0){
        enemy->orientation = rand()%5;
        while(enemy->orientation ==0){
            enemy->orientation = rand() % 5;
        }
    }
    }


    // Enemigo que te sigue pegado a la pared O_O (Descartado Totalmente)

    else if(enemy->id == 2){
        mapa[enemy->posY][enemy->posX] = '$';

    if(mapa[enemy->posY][enemy->posX+1] !='<' && mapa[enemy->posY][enemy->posX+1] !='>' && mapa[enemy->posY][enemy->posX+1] !='v' && mapa[enemy->posY][enemy->posX+1] !='^' && mapa[enemy->posY][enemy->posX-1] !='<' && mapa[enemy->posY][enemy->posX-1] !='>' && mapa[enemy->posY][enemy->posX-1] !='v' && mapa[enemy->posY][enemy->posX-1] !='^'){

    if(*tickRate % rand()%2 == 0 && *tickRate != 0){
        if(enemy->posX < jugador.posX && mapa[enemy->posY][enemy->posX +1] != '#'){
            enemy->posX = enemy->posX +1;
            mapa[enemy->posY][enemy->posX-1] = ' ';
        }else if(enemy->posX > jugador.posX && mapa[enemy->posY][enemy->posX -1] != '#'){
            enemy->posX = enemy->posX -1;
            mapa[enemy->posY][enemy->posX+1] = ' ';
        }else{
        }
            //shootingEnemy(mapa, enemy, tickRate, jugador, bullet);
    }
    }
}
    else if(enemy->id == 3){
        mapa[enemy->posY][enemy->posX] = 02;




        if(bossAttackTimeLeft == 0 && bossCooldown != 0){

        if(*tickRate % 5 == 0){
            bossCooldown = bossCooldown -1;
            if(enemy->orientation == 1){
            if(mapa[enemy->posY -1][enemy->posX] == '#' || mapa[enemy->posY -1][enemy->posX] == '$'){
                enemy->orientation = 3;
            }else{
            enemy->posY = enemy->posY -1;
            }
            if(mapa[enemy->posY+1][enemy->posX] != '#'){
            mapa[enemy->posY+1][enemy->posX] = ' ';
            }
           }
            if(enemy->orientation == 3){
            if(mapa[enemy->posY +1][enemy->posX] == '#' || mapa[enemy->posY +1][enemy->posX] == '$'){
                enemy->orientation = 1;
            }else{
            enemy->posY = enemy->posY +1;
            }
            if(mapa[enemy->posY-1][enemy->posX] != '#'){
            mapa[enemy->posY-1][enemy->posX] = ' ';
            }
            }
            if(enemy->orientation == 2){
            if(mapa[enemy->posY][enemy->posX +1] == '#' || mapa[enemy->posY ][enemy->posX+1] == '$'){
                enemy->orientation = 4;
            }else{
            enemy->posX = enemy->posX +1;
            }
            if(mapa[enemy->posY][enemy->posX -1] != '#'){
            mapa[enemy->posY][enemy->posX -1] = ' ';
            }
            }
            if(enemy->orientation == 4){
            if(mapa[enemy->posY][enemy->posX-1] == '#' || mapa[enemy->posY][enemy->posX-1] == '$'){
                enemy->orientation = 2;
            }else{
            enemy->posX = enemy->posX -1;
            }
            if(mapa[enemy->posY][enemy->posX +1] != '#'){
            mapa[enemy->posY][enemy->posX +1] = ' ';
            }
            }
            }
            if(*tickRate % rand()%15 == 0 && *tickRate != 0){
                enemy->orientation = rand()%5;
                while(enemy->orientation ==0){
                    enemy->orientation = rand() % 5;
                }
            }
        }else{
            mapa[enemy->posY][enemy->posX] = 01;
            if(bossAttackTimeLeft == 0){
            bossAttackTimeLeft = 15;
            }
            if(bossAttackTimeLeft > 5){

            for(int i = 2 ; i<enemy->posY; i++){
                if(mapa[i][enemy->posX] != '#' && mapa[i][enemy->posX] != mapa[13][93] && mapa[i][enemy->posX] != mapa[14][93]){
                mapa[i][enemy->posX] = '|';
                }
            }
            for(int i = enemy->posY + 1 ; i<fil-2; i++){
                if(mapa[i][enemy->posX] != '#' && mapa[i][enemy->posX] != mapa[13][93] && mapa[i][enemy->posX] != mapa[14][93]){
                mapa[i][enemy->posX] = '|';
                }
            }

            for(int i = 2 ; i<enemy->posX; i++){
                if(mapa[enemy->posY][i] != '#' && mapa[enemy->posY][i] != mapa[13][93] && mapa[enemy->posY][i] != mapa[14][93]){
                mapa[enemy->posY][i] = '-';
                }
            }
            for(int i = enemy->posX + 1 ; i<col-2; i++){
                if(mapa[enemy->posY][i] != '#' && mapa[enemy->posY][i] != mapa[13][93] && mapa[enemy->posY][i] != mapa[14][93]){
                mapa[enemy->posY][i] = '-';
                }
            }
            }else{
            for(int i = 2 ; i<enemy->posY; i++){
                if(mapa[i][enemy->posX] != '#' && mapa[i][enemy->posX] != mapa[13][93] && mapa[i][enemy->posX] != mapa[14][93]){
                mapa[i][enemy->posX] = '$';
                }
            }
            for(int i = enemy->posY + 1 ; i<fil-2; i++){
                if(mapa[i][enemy->posX] != '#' && mapa[i][enemy->posX] != mapa[13][93] && mapa[i][enemy->posX] != mapa[14][93]){
                mapa[i][enemy->posX] = '$';
                }
            }

            for(int i = 2 ; i<enemy->posX; i++){
                if(mapa[enemy->posY][i] != '#' && mapa[enemy->posY][i] != mapa[13][93] && mapa[enemy->posY][i] != mapa[14][93]){
                mapa[enemy->posY][i] = '$';
                }
            }
            for(int i = enemy->posX + 1 ; i<col-2; i++){
                if(mapa[enemy->posY][i] != '#' && mapa[enemy->posY][i] != mapa[13][93] && mapa[enemy->posY][i] != mapa[14][93]){
                mapa[enemy->posY][i] = '$';
                }
            }


            }
            bossAttackTimeLeft = bossAttackTimeLeft -1;
            bossCooldown = 10;
        }
    }

}
}

//FUNCION DE DISPARO DE LOS ENEMIGOS (PROBABLEMENTE SE DISCONTINUE)

void shootingEnemy(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet)
{

            if(enemy.posY<jugador.posY)
            {

                if(balaCreada == 0 && mapa[enemy.posY][enemy.posX] == mapa[enemy.posY][jugador.posX]){
                bullet->posX = enemy.posX;
                bullet->posY = enemy.posY + 1;
                balaCreada = 1;
            }
                if(mapa[bullet->posY + 1][bullet->posX] != '#' && mapa[bullet->posY][bullet->posX] != mapa[jugador.posY][jugador.posX]){
                bullet->posY = bullet->posY + 1;
                mapa[bullet->posY][bullet->posX] = 'o';
                }else{
                balaCreada = 0;
                mapa[bullet->posY][bullet->posX] = ' ';
                }
                if(mapa[bullet->posY-1][bullet->posX] == 'o'){
                mapa[bullet->posY-1][bullet->posX] = ' ';
                }
        }
}

//LA FUNCION QUE NO HACE NADA

void collision(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet)
{


}

//LEE UN ARCHIVO CON PLANTILLAS DE MAPAS

void leerArchivoPlantilla(){
    FILE * archi = fopen(nombreArchivo, "r");

    char mapa[fil][col];
    int i= 0;
    if(archi){
        while(!feof(archi)){
            fread(&mapa, fil*col, 1, archi);
            if(!feof(archi)){

                while(mapa[0][0] != arregloPlantillas[i].nivel && i < 3){
                    i++;
                }

                if(mapa[0][0] == arregloPlantillas[i].nivel){
                    nodoPlantillas * nuevoNodo = crearNodoPlantillas(mapa);
                    arregloPlantillas[i].listaMapas = agreagarAlPpioPlantillas(arregloPlantillas[i].listaMapas, nuevoNodo);
                    //printearMapa(arregloPlantillas[i].listaMapas->mapa);

                }else{
                    printf("Ocurrio un problema, revisa el archivo de mapas\n");

                }
            }

        }

        fclose(archi);
    }



}


//LA TIPICA

nodoPlantillas * inicarListaPlantilla(){
    return NULL;
}

//CREA UN NODO DE PLANTILLAS CON UN MAPA PASADO POR PARAMETRO

nodoPlantillas * crearNodoPlantillas(char mapa[fil][col]){
    nodoPlantillas * nuevoNodo = (nodoPlantillas*)malloc(sizeof(nodoPlantillas));

    nuevoNodo->sig = NULL;
    memcpy(nuevoNodo->mapa, mapa, fil*col);

    return nuevoNodo;
}

//LA TIPICA DEL PRINCIPIO

nodoPlantillas * agreagarAlPpioPlantillas(nodoPlantillas * lista, nodoPlantillas * nuevoNodo){
    if(lista == NULL){
        lista = nuevoNodo;
    }else{
        nuevoNodo->sig = lista;
        lista = nuevoNodo;
    }


    return lista;
}

//MUESTRA EL MAPA (ES PARA DEBUGEAR)

void printearMapa(char mapa[fil][col]){
     int i, j;
    printf("%c------------------------------------------------------------------------------------------------%c\n",218, 191);
    for(i = 0; i<fil; i++){
        printf("|");
        for(j = 0; j<col; j++){
          printf("%c", mapa[i][j]);
        }
    printf("|\n");
}
    printf("%c------------------------------------------------------------------------------------------------%c\n", 192, 217);

}

//SELECCIONA UN MAPA AL AZAR DE LA LISTA DE MAPAS CON PROBABILIDAD INCREMENTATIVA

void randomMapSelector(char mapaAux[fil][col], int currLevel){
    nodoPlantillas * seg;
    seg = arregloPlantillas[currLevel].listaMapas;
    int i = 5;
    int randomNumber = rand() % i;


    while(randomNumber != 0 && seg->sig != NULL){
        seg = seg->sig;
        randomNumber = rand() % i;
        i--;
    }
    memcpy(mapaAux, seg->mapa, fil*col);

}

//LE ASIGNA UNA POSICION ALEATORIA A LOS ENEMIGOS Y SE ASEGURA DE QUE NO APAREZCAN EN LAS PAREDES

void spawnearEnemigos(char mapa[fil][col], npc enemys[30], int cantidadEnemigos){

    for(int i = 0; i < cantidadEnemigos; i++){
        enemys[i].posY = rand() % ( 24 - 2) + 2;
        enemys[i].posX = rand() % ( 94 - 2) + 2;
        while(mapa[enemys[i].posY][enemys[i].posX] == '#'){
        enemys[i].posY = rand() % ( 24 - 2) + 2;
        enemys[i].posX = rand() % ( 94 - 2) + 2;

        }
        if(currLevel == 2 || modoCaos == 1){
            enemys[i].id = 3;
            enemys[i].hp= 10;
        }else{
        enemys[i].id = 1;
        enemys[i].hp= 1;
        }
        enemys[i].orientation = rand() % 5;

    }

}


nodoPartida * crearNodoPartida(char mapaAux[fil][col], int currLevel){
    nodoPartida * nuevoNodoPartida = (nodoPartida*) malloc(sizeof(nodoPartida));

    nuevoNodoPartida->nivel = currLevel;
    nuevoNodoPartida->sala = NULL;
    memcpy(nuevoNodoPartida->mapaNodo, mapaAux, fil*col);
    nuevoNodoPartida->sig = NULL;

    return nuevoNodoPartida;
}

nodoPartida * agregarAlFinal(nodoPartida * lista, nodoPartida * nuevoNodo){
    nodoPartida * ultimo = NULL;

    int sala = 1;

    if(lista == NULL){
        lista = nuevoNodo;
    }else{
        ultimo = buscarUltimo(lista, &sala);
        ultimo->sig = nuevoNodo;
    }
    lista->sala = sala;

    return lista;
}

nodoPartida * buscarUltimo(nodoPartida * lista, int * sala){
    nodoPartida * rta = lista;

    if(lista!=NULL){
        while(rta->sig!=NULL){
            rta = rta->sig;
            *sala = *sala + 1;
        }
    *sala = *sala+1;
    }
    if(*sala > 3){
        *sala = *sala - 3 * currLevel;
    }
    return rta;
}

void pasarDeHabitacion(char randomMap[fil][col], nodoPartida * partidaActual, npc enemys[30], int cantindadEnemigos){
    char mapa[fil][col];
    memcpy(mapa, randomMap, fil*col);
        randomMapSelector(randomMap, currLevel);
    while(memcmp(mapa, randomMap, fil * col) == 0){
        randomMapSelector(randomMap, currLevel);
    }


    nodoPartida * nuevaHabitacion = crearNodoPartida(randomMap, currLevel);
    partidaActual = agregarAlFinal(partidaActual,nuevaHabitacion);
    spawnearEnemigos(randomMap, enemys, cantindadEnemigos);
}

void defensa(arma * armaAux, player jugador, char mapa[fil][col], int * tickrate){

    //mapa[armaAux.posY][armaAux.posX] = ' ';
    if(*tickrate % 3 == 0){

    armaAux->posX = (jugador.posX + cos(angulo) * 2) + 0.5;
    armaAux->posY = (jugador.posY + sin(angulo) * 2) + 0.5;

    angulo++;

    mapa[armaAux->posY][armaAux->posX] = 'O';

    if(angulo == 360){
        angulo = 0;
    }

    }
}

void mostrarListaPartida(nodoPartida * partidaActual){
    nodoPartida * seg = partidaActual;
    while(seg != NULL){
    printearMapa(seg->mapaNodo);
    seg = seg->sig;
    }
}

void detectarColision(arma armaAux, npc * enemy, char mapa[fil][col], int * cantidadEnemigosActuales, player * jugador, proyectile balas[100]){

    if(armaAux.posY == enemy->posY && armaAux.posX == enemy->posX && enemy->hp >0){
        enemy->hp = enemy->hp -1;
        *cantidadEnemigosActuales = *cantidadEnemigosActuales - 1;
        if(currLevel != 2){
            puntosAgregados = puntosAgregados + 100;

        }

    }

    for(int i = 0; i<100; i++){
        if(balas[i].posY == enemy->posY && balas[i].posX == enemy->posX && enemy->hp >0){
            *cantidadEnemigosActuales = *cantidadEnemigosActuales -1;
            enemy->hp = enemy->hp -1;
            if(currLevel!=2){
            puntosAgregados = puntosAgregados + 100;
            }
            balas[i].posY = NULL;
            balas[i].posX = NULL;
            balas[i].dir = NULL;
        }

    }

    if(enemy->posY == jugador->posY && enemy->posX == jugador->posX && iFrames == 0){

        jugador->hp = jugador->hp - 1;
        iFrames = 50;

    }

    if(currLevel == 2){
        if(enemy->posY == jugador->posY || enemy->posX == jugador->posX){
        if(iFrames == 0 && bossAttackTimeLeft <=5 && bossAttackTimeLeft > 0){

        jugador->hp = jugador->hp - 1;
        iFrames = 50;
           }
        }


    }

    if(iFrames > 0){
    iFrames = iFrames-1;
    }


}

void endScreen(char mapa[fil][col]){

    memset(mapa, 32, fil*col);

    mapa[12][42] = 'G';
    mapa[12][43] = 'A';
    mapa[12][44] = 'N';
    mapa[12][45] = 'A';
    mapa[12][46] = 'S';
    mapa[12][47] = 'T';
    mapa[12][48] = 'E';
    mapa[12][49] = '.';

    printearMapa(mapa);
}

void defeatScreen(char mapa[fil][col]){
    memset(mapa, 32, fil*col);

    mapa[12][42] = 'H';
    mapa[12][43] = 'A';
    mapa[12][44] = 'S';
    mapa[12][45] = ' ';
    mapa[12][46] = 'M';
    mapa[12][47] = 'U';
    mapa[12][48] = 'E';
    mapa[12][49] = 'R';
    mapa[12][50] = 'T';
    mapa[12][51] = 'O';
    mapa[12][52] = '.';

    printearMapa(mapa);

}

void disparoJugador(player * jugador, proyectile balas[100]){
    if(GetKeyState('L') & 0x8000 && playerShootinCooldown == 0){
        crearBala(jugador, 2, balas);
        playerShootinCooldown = 10;
    }
    if(GetKeyState('I') & 0x8000 && playerShootinCooldown == 0){
        crearBala(jugador, 1, balas);
        playerShootinCooldown = 10;
    }
    if(GetKeyState('K') & 0x8000 && playerShootinCooldown == 0){
        crearBala(jugador, 3, balas);
        playerShootinCooldown = 10;
    }
    if(GetKeyState('J') & 0x8000 && playerShootinCooldown == 0){
        crearBala(jugador, 4, balas);
        playerShootinCooldown = 10;
    }
     if(GetKeyState('O') & 0x8000 && playerShootinCooldown == 0){
        balaAcutal = 0;
    }
    if(playerShootinCooldown > 0){
    playerShootinCooldown = playerShootinCooldown -1;
    }
}

void crearBala(player * jugador, int direccion, proyectile balas[100]){
    switch(direccion){
        case 2:
            balas[balaAcutal].posY = jugador->posY;
            balas[balaAcutal].posX = jugador->posX + 1;
            balas[balaAcutal].dir = 2;
            balaAcutal = balaAcutal + 1;
            if(balaAcutal == 100){
                balaAcutal = 0;
            }
            break;
        case 4:
            balas[balaAcutal].posY = jugador->posY;
            balas[balaAcutal].posX = jugador->posX -1;
            balas[balaAcutal].dir = 4;
            balaAcutal = balaAcutal + 1;
            if(balaAcutal == 100){
                balaAcutal = 0;
            }
            break;
        case 1:
            balas[balaAcutal].posY = jugador->posY -1;
            balas[balaAcutal].posX = jugador->posX;
            balas[balaAcutal].dir = 1;
            balaAcutal = balaAcutal + 1;
            if(balaAcutal == 100){
                balaAcutal = 0;
            }
            break;
        case 3:
            balas[balaAcutal].posY = jugador->posY +1;
            balas[balaAcutal].posX = jugador->posX;
            balas[balaAcutal].dir = 3;
            balaAcutal = balaAcutal + 1;
            if(balaAcutal == 100){
                balaAcutal = 0;
            }
            break;
    }
}

void handlerBalas(proyectile bala, char mapa[fil][col]){
    if(bala.posY != NULL){
    mapa[bala.posY][bala.posX] = 'o';

    if(bala.dir == 2 && mapa[bala.posY][bala.posX + 1] != '#' ){
        bala.posX = bala.posX + 1;
    }else if(bala.dir == 1 && mapa[bala.posY -1][bala.posX] != '#'){
        bala.posY = bala.posY -1;
    }else if(bala.dir == 3 && mapa[bala.posY + 1][bala.posX] != '#'){
        bala.posY = bala.posY + 1;
    }else if(bala.dir == 4 && mapa[bala.posY][bala.posX - 1] != '#'){
        bala.posX = bala.posX - 1;
    }else{
        bala.posY = NULL;
        bala.posX = NULL;
        bala.dir = NULL;
    }

    }
}

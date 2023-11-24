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
    int vel;
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

//PROTOTIPADO
player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation);
void runnpcs(char mapa[fil][col],npc * enemy, int * tickRate, player jugador, proyectile * bullet);
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
void detectarColision(arma armaAux, npc * enemy, char mapa[fil][col], int * cantidadEnemigosActuales, player * jugador);
//MAIN

int main(){



    int opcion,opcion2;
    opcion2 = 0;
    nodoArbol* arbol;
    arbol = NULL;
    char nombreArchivo[30]="archivoUsuarios.bin";
    do{
    printf("1_ Iniciar sesion como usuario\n");
    printf("2_ Iniciar sesion como administrador\n");
    printf("3_ Crear usuario\n");
    scanf("%i",&opcion);
    system("cls");
    switch (opcion)
    {
    case 1:
        opcion2=iniciarSesion(nombreArchivo);
        system("cls");
        break;
    case 2:
        opcion2=iniciarSesionAdmin(nombreArchivo);
        printf("%i", opcion2);
        system("pause");
        system("cls");
        break;
    case 3:
        nuevoUsuarioAlArchivo(nombreArchivo);
        system("cls");
        break;
    default:
        printf("Error, ingrese una opcion valida\n");
        break;
    }
    }while(opcion2==0);

    if(opcion2==2)
    {
        int id;
        do{
        printf("1_ Dar usuario de baja por ID\n");
        printf("2_ Ver lista de jugadores activos\n");
        printf("3_ Ver lista de jugadores dados de baja\n");
        printf("4_ Ver la lista de todos los jugadores\n");
        printf("5_ Ver jugador en especifico\n");
        printf("6_ Iniciar creador de mapas\n");
        printf("0_ Cerrar\n");
        scanf("%i",&opcion);
        switch(opcion){
        case 0:
            break;
        case 1:
            printf("Ingrese el id del usuario para darlo de baja\n");
            scanf("%i",&id);
            arbol=archivoArbol2(nombreArchivo,arbol);
            arbol=darDeBajaUsuario(arbol,id);
            guardarArbolEnArchivo(arbol,nombreArchivo);
            system("pause");
            break;
        case 2:
            arbol=archivoArbol2(nombreArchivo,arbol);
            mostrarInorderActivos(arbol);
            system("pause");
            break;
        case 3:
            arbol=archivoArbol2(nombreArchivo,arbol);
            mostrarInorderBaja(arbol);
            system("pause");
            break;
        case 4:
            arbol=archivoArbol2(nombreArchivo,arbol);
            mostrarInorder(arbol);
            system("pause");
            break;
        case 5:
            printf("Ingrese el id del usuario para verlo\n");
            scanf("%i",&id);
            arbol=archivoArbol2(nombreArchivo,arbol);
            mostrarEspecifico(arbol,id);
            system("pause");
            break;
        case 6:
            iniciarCreador();
            break;
        default:
            printf("Error, ingrese una opcion valida\n");
            break;



        }
        }while(opcion!=0);

    }


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

    memset(mapa, 32, sizeof(mapa));

    proyectile bullet;
    bullet.posX = 0;
    bullet.posY = 2;

    int * currentOrientiation = 94;

    int * tickRate;
    int cantindadEnemigos;
    cantindadEnemigos = (currLevel + 1) * 5;
    int cantidadEnemigosActuales = cantindadEnemigos-1;


    fflush(stdin);
    scanf("%i", &eleccion);

    while(eleccion!=1 && eleccion!=2 && eleccion!=3 && eleccion!=777)
    {
        printf("\nSu numero es incorrecto, porfavor ingrese 1,2 o 3\n");
        fflush(stdin);
        scanf("%i", &eleccion);
    }
    if(eleccion == 777){
        modoCaos = 1;
        eleccion = 1;
    }

    system("cls");

    //Organizamos el mapa y sus cosas




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
    for(int i = 0; i<cantindadEnemigos-1; i++){
    detectarColision(armaXD, &enemys[i], mapa, &cantidadEnemigosActuales, &jugador);
    runnpcs(mapa, &enemys[i], &tickRate, jugador, &bullet);
    }
    runPlayer(mapa, jugador, &currentOrientiation);
    runMap(mapa, jugador, &tickRate,ultimoNodo);
    sistemaDeVida(&jugador, eleccion, mapa);

    if(mapa[jugador.posY][jugador.posY] == mapa[13][93] || mapa[jugador.posY][jugador.posY] == mapa[14][93]){
        if(currLevel != 2){
        if(ultimoNodo->sala >= 3){
            currLevel = currLevel + 1;
        }
        cantindadEnemigos = (currLevel + 1) * 5;
        cantidadEnemigosActuales = cantindadEnemigos-1;
        if(currLevel == 2){
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
    printf("%i ", iFrames);
    //printf("\x1b[J");

    if(bossHP <= 0){
        Sleep(1000);
        system("cls");
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

        }
        printf("\n");
    }
    printf("%c------------------------------------------------------------------------------------------------%c", 192, 217);

}


// SISTEMA DE VIDA ( corazoncitos y espadas )

void sistemaDeVida (player * jugador, int eleccion, char mapa[fil][col])
{
    if(jugador->hp>0)
    {
        printf("                \n");
        int i=0;
        while(i<jugador->hp && eleccion==1)
        {
            int hp[30];
            hp[i]=03;
            printf("|%c|",hp[i]);
            i++;

        }

    }
        if(jugador->hp == 0){
            Sleep(1000);
            system("cls");
            defeatScreen(mapa);
            system("pause");
            return 0;
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

void runnpcs(char mapa[fil][col],npc * enemy, int * tickRate, player jugador, proyectile * bullet){

    if(currLevel == 2){
    bossHP = enemy->hp;
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

void detectarColision(arma armaAux, npc * enemy, char mapa[fil][col], int * cantidadEnemigosActuales, player * jugador){

    if(armaAux.posY == enemy->posY && armaAux.posX == enemy->posX && enemy->hp >0){
        enemy->hp = enemy->hp -1;
        *cantidadEnemigosActuales = *cantidadEnemigosActuales - 1;
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

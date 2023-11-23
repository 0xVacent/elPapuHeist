//ACA INICIA EL CODIGO :3
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>



//CREAMOS ESTRUCTURAS

typedef struct{
    int posY;
    int posX;
    int vel;
}proyectile;

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
const int fil = 28;
const int col = 96;
const int filMostrar = 28;
const int colMostrar = 96;
char nombreArchivo[20] = "mapas.dat";

//VARIABLE GLOGAL

int balaCreada  = 0;
plantillas arregloPlantillas[3];
int currLevel = 0;
nodoPartida * partidaActual = NULL;
int bossCooldown = 5;
int bossAttackTimeLeft = 0;
int modoCaos = 0;
//PROTOTIPADO
player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation);
npc runnpcs(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet);
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

//MAIN

int main(){

    int eleccion=1;
    system("color");
    player humano;
    player elfo;
    player orco;

    humano.attack=2;
    elfo.attack=3;
    orco.attack=1;

    humano.potion=1;
    elfo.potion=1;
    orco.potion=1;

    humano.hp=7;
    elfo.hp=6;
    orco.hp=10;


    srand(time(NULL));
    arregloPlantillas[0].nivel = '1';
    arregloPlantillas[1].nivel = '2';
    arregloPlantillas[2].nivel = '3';
    char mapa[fil][col];
    player jugador;
    jugador.posY = 12;
    jugador.posX = 6;
    npc enemys[30];

    memset(mapa, 32, sizeof(mapa));

    proyectile bullet;
    bullet.posX = 0;
    bullet.posY = 2;

    int * currentOrientiation = 94;

    int * tickRate;
    int cantindadEnemigos;
    cantindadEnemigos = (currLevel + 1) * 5;

    printf("Elige tu raza\n\n");
    printf("            Humano       Elfo         Orco ");
    printf("\n\n  ATAQUE      2           3            1  ");
    printf("\n\n  VIDA        7           6            9  \n\n");
    printf("Ingrese |1| para seleccionar humano\n");
    printf("Ingrese |2| para seleccionar elfo\n");
    printf("Ingrese |3| para seleccionar orco\n");

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




    leerArchivo();
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
    for(int i = 0; i<cantindadEnemigos-1; i++){
    enemys[i] = runnpcs(mapa, enemys[i], &tickRate, jugador, &bullet);
    }
    runPlayer(mapa, jugador, &currentOrientiation);
    runMap(mapa, jugador, &tickRate,ultimoNodo);
    sistemaDeVida(&humano,&elfo,&orco,eleccion);
    printf("%i", bullet.posY);

    if(mapa[jugador.posY][jugador.posY] == mapa[13][93] || mapa[jugador.posY][jugador.posY] == mapa[14][93]){
        if(ultimoNodo->sala >= 3){
            currLevel = currLevel + 1;
        }
        cantindadEnemigos = (currLevel + 1) * 5;
        if(currLevel == 2){
            cantindadEnemigos = 2;
        }
        pasarDeHabitacion(randomMap, partidaActual, enemys, cantindadEnemigos);

        jugador.posY = 12;
        jugador.posX = 6;
    }

    //printf("\x1b[J");
    }
    return 0;
}



//MUESTRA EL MAPA

void runMap(char mapa[fil][col], player jugador, int * tickRate, nodoPartida * ultimoNodo){
    int i, j;

    printf("%c------------------------------------NIVEL:%i-SALA:%i----------------------------------------------%c %i %i, %i\n", 218, currLevel + 1,ultimoNodo->sala, 191, jugador.posY, jugador.posX, *tickRate);
    for(i=0 ; i<filMostrar;i++){
            printf("|");

        for(j=0; j<colMostrar; j++){
            printf("%c", mapa[i][j]);
        }
        printf("|\n");
    }
    printf("%c------------------------------------------------------------------------------------------------%c", 192, 217);

}


// SISTEMA DE VIDA ( corazoncitos y espadas )

void sistemaDeVida (player* humano, player* elfo, player* orco, int eleccion)
{
    if(humano->hp>0)
    {
        printf("                \n");
        int i=0;
        while(i<humano->hp && eleccion==1)
        {
            int hp[30];
            hp[i]=03;
            printf("|%c|",hp[i]);
            i++;

        }

        printf("                \n");
        int l=0;
        while(l<humano->attack && eleccion==1){

            int espada[30];
            espada[l]=197;
            printf("|%c|",espada[l]);
            l++;
        }

        printf("                \n");
        int j=0;
        while(j<humano->potion && eleccion==1){
            int potion[30];
            potion[j]=208;
            printf("|%c|",potion[j]);
            j++;
        }

    }

    if(elfo->hp>0)
    {
        printf("                \n");
        int i=0;
        while(i<elfo->hp && eleccion==2)
        {
            int hp[30];
            hp[i]=03;
            printf("|%c|",hp[i]);
            i++;
        }

        printf("                \n");
        int l=0;
        while(l<elfo->attack && eleccion==2){

            int espada[30];
            espada[l]=197;
            printf("|%c|",espada[l]);
            l++;
        }
        printf("                \n");
        int j=0;
        while(j<elfo->potion && eleccion==2){
            int potion[30];
            potion[j]=208;
            printf("|%c|",potion[j]);
            j++;
        }
        }


    if(orco->hp>0)
    {
        printf("                \n");
        int i=0;
        while(i<orco->hp && eleccion==3)
        {
            int hp[30];
            hp[i]=03;
            printf("|%c|",hp[i]);
            i++;
        }

        printf("                \n");
        int l=0;
        while(l<orco->attack && eleccion==3){

            int espada[30];
            espada[l]=197;
            printf("|%c|",espada[l]);
            l++;
        }

        printf("                \n");
        int j=0;
        while(j<orco->potion && eleccion==3){
            int potion[30];
            potion[j]=208;
            printf("|%c|",potion[j]);
            j++;
        }
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

    if(GetKeyState('L') & 0x8000){
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
}


//HACEMOS COSAS CON LOS GUARDIAS

npc runnpcs(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet){


    if(enemy.id == 1){
    mapa[enemy.posY][enemy.posX] = '$';

    if(*tickRate % 5 == 0){

    if(enemy.orientation == 1){
    if(mapa[enemy.posY -1][enemy.posX] == '#' || mapa[enemy.posY -1][enemy.posX] == '$'){
        enemy.orientation = 3;
    }else{
    enemy.posY = enemy.posY -1;
    }
    if(mapa[enemy.posY+1][enemy.posX] != '#'){
    mapa[enemy.posY+1][enemy.posX] = ' ';
    }
   }
    if(enemy.orientation == 3){
    if(mapa[enemy.posY +1][enemy.posX] == '#' || mapa[enemy.posY +1][enemy.posX] == '$'){
        enemy.orientation = 1;
    }else{
    enemy.posY = enemy.posY +1;
    }
    if(mapa[enemy.posY-1][enemy.posX] != '#'){
    mapa[enemy.posY-1][enemy.posX] = ' ';
    }
    }
    if(enemy.orientation == 2){
    if(mapa[enemy.posY][enemy.posX +1] == '#' || mapa[enemy.posY ][enemy.posX+1] == '$'){
        enemy.orientation = 4;
    }else{
    enemy.posX = enemy.posX +1;
    }
    if(mapa[enemy.posY][enemy.posX -1] != '#'){
    mapa[enemy.posY][enemy.posX -1] = ' ';
    }
    }
    if(enemy.orientation == 4){
    if(mapa[enemy.posY][enemy.posX-1] == '#' || mapa[enemy.posY][enemy.posX-1] == '$'){
        enemy.orientation = 2;
    }else{
    enemy.posX = enemy.posX -1;
    }
    if(mapa[enemy.posY][enemy.posX +1] != '#'){
    mapa[enemy.posY][enemy.posX +1] = ' ';
    }
    }
    }
    if(*tickRate % rand()%15 == 0 && *tickRate != 0){
        enemy.orientation = rand()%5;
        while(enemy.orientation ==0){
            enemy.orientation = rand() % 5;
        }
    }
}

    // Enemigo que te sigue pegado a la pared O_O (Descartado Totalmente)

    else if(enemy.id == 2){
        mapa[enemy.posY][enemy.posX] = '$';

    if(mapa[enemy.posY][enemy.posX+1] !='<' && mapa[enemy.posY][enemy.posX+1] !='>' && mapa[enemy.posY][enemy.posX+1] !='v' && mapa[enemy.posY][enemy.posX+1] !='^' && mapa[enemy.posY][enemy.posX-1] !='<' && mapa[enemy.posY][enemy.posX-1] !='>' && mapa[enemy.posY][enemy.posX-1] !='v' && mapa[enemy.posY][enemy.posX-1] !='^'){

    if(*tickRate % rand()%2 == 0 && *tickRate != 0){
        if(enemy.posX < jugador.posX && mapa[enemy.posY][enemy.posX +1] != '#'){
            enemy.posX = enemy.posX +1;
            mapa[enemy.posY][enemy.posX-1] = ' ';
        }else if(enemy.posX > jugador.posX && mapa[enemy.posY][enemy.posX -1] != '#'){
            enemy.posX = enemy.posX -1;
            mapa[enemy.posY][enemy.posX+1] = ' ';
        }else{
        }
            shootingEnemy(mapa, enemy, tickRate, jugador, bullet);
    }
    }
}
    else if(enemy.id == 3){
        mapa[enemy.posY][enemy.posX] = 02;




        if(bossAttackTimeLeft == 0 && bossCooldown != 0){

        if(*tickRate % 5 == 0){
            bossCooldown = bossCooldown -1;
            if(enemy.orientation == 1){
            if(mapa[enemy.posY -1][enemy.posX] == '#' || mapa[enemy.posY -1][enemy.posX] == '$'){
                enemy.orientation = 3;
            }else{
            enemy.posY = enemy.posY -1;
            }
            if(mapa[enemy.posY+1][enemy.posX] != '#'){
            mapa[enemy.posY+1][enemy.posX] = ' ';
            }
           }
            if(enemy.orientation == 3){
            if(mapa[enemy.posY +1][enemy.posX] == '#' || mapa[enemy.posY +1][enemy.posX] == '$'){
                enemy.orientation = 1;
            }else{
            enemy.posY = enemy.posY +1;
            }
            if(mapa[enemy.posY-1][enemy.posX] != '#'){
            mapa[enemy.posY-1][enemy.posX] = ' ';
            }
            }
            if(enemy.orientation == 2){
            if(mapa[enemy.posY][enemy.posX +1] == '#' || mapa[enemy.posY ][enemy.posX+1] == '$'){
                enemy.orientation = 4;
            }else{
            enemy.posX = enemy.posX +1;
            }
            if(mapa[enemy.posY][enemy.posX -1] != '#'){
            mapa[enemy.posY][enemy.posX -1] = ' ';
            }
            }
            if(enemy.orientation == 4){
            if(mapa[enemy.posY][enemy.posX-1] == '#' || mapa[enemy.posY][enemy.posX-1] == '$'){
                enemy.orientation = 2;
            }else{
            enemy.posX = enemy.posX -1;
            }
            if(mapa[enemy.posY][enemy.posX +1] != '#'){
            mapa[enemy.posY][enemy.posX +1] = ' ';
            }
            }
            }
            if(*tickRate % rand()%15 == 0 && *tickRate != 0){
                enemy.orientation = rand()%5;
                while(enemy.orientation ==0){
                    enemy.orientation = rand() % 5;
                }
            }
        }else{
            mapa[enemy.posY][enemy.posX] = 01;
            if(bossAttackTimeLeft == 0){
            bossAttackTimeLeft = 15;
            }
            if(bossAttackTimeLeft > 5){
            for(int i = 2 ; i<enemy.posY; i++){
                if(mapa[i][enemy.posX] != '#' && mapa[i][enemy.posX] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[i][enemy.posX] = '|';
                }
            }
            for(int i = enemy.posY + 1 ; i<fil-2; i++){
                if(mapa[i][enemy.posX] != '#' && mapa[i][enemy.posX] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[i][enemy.posX] = '|';
                }
            }

            for(int i = 2 ; i<enemy.posX; i++){
                if(mapa[enemy.posY][i] != '#' && mapa[enemy.posY][i] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[enemy.posY][i] = '-';
                }
            }
            for(int i = enemy.posX + 1 ; i<col-2; i++){
                if(mapa[enemy.posY][i] != '#' && mapa[enemy.posY][i] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[enemy.posY][i] = '-';
                }
            }
            }else{
            for(int i = 2 ; i<enemy.posY; i++){
                if(mapa[i][enemy.posX] != '#' && mapa[i][enemy.posX] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[i][enemy.posX] = '$';
                }
            }
            for(int i = enemy.posY + 1 ; i<fil-2; i++){
                if(mapa[i][enemy.posX] != '#' && mapa[i][enemy.posX] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[i][enemy.posX] = '$';
                }
            }

            for(int i = 2 ; i<enemy.posX; i++){
                if(mapa[enemy.posY][i] != '#' && mapa[enemy.posY][i] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[enemy.posY][i] = '$';
                }
            }
            for(int i = enemy.posX + 1 ; i<col-2; i++){
                if(mapa[enemy.posY][i] != '#' && mapa[enemy.posY][i] != mapa[13][93] && mapa[i][enemy.posX] != mapa[14][93]){
                mapa[enemy.posY][i] = '$';
                }
            }


            }
            bossAttackTimeLeft = bossAttackTimeLeft -1;
            bossCooldown = 10;
        }
    }


    *tickRate = *tickRate + 1;

    return enemy;

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

void leerArchivo(){
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

void spawnearEnemigos(char mapa[fil][col], npc enemys[40], int cantidadEnemigos){


    for(int i = 0; i < cantidadEnemigos; i++){
        enemys[i].posY = rand() % ( 24 - 2) + 2;
        enemys[i].posX = rand() % ( 94 - 2) + 2;
        while(mapa[enemys[i].posY][enemys[i].posX] == '#'){
        enemys[i].posY = rand() % ( 24 - 2) + 2;
        enemys[i].posX = rand() % ( 94 - 2) + 2;

        }
        if(currLevel == 2 || modoCaos == 1){
            enemys[i].id = 3;
        }else{
        enemys[i].id = 1;
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
    randomMapSelector(randomMap, currLevel);
    nodoPartida * nuevaHabitacion = crearNodoPartida(randomMap, currLevel);
    partidaActual = agregarAlFinal(partidaActual,nuevaHabitacion);
    spawnearEnemigos(randomMap, enemys, cantindadEnemigos);
}

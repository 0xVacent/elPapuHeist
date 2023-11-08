//ACA INICIA EL CODIGO :3
#include <stdlib.h>
#include <stdio.h>



//CREAMOS ESTRUCTURAS
typedef struct{
    int posY;
    int posX;
}camara;

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

typedef struct
{
    int cuarto;
    char mapa[27][96];
}sala;

typedef struct{
    sala sala;
    struct nodoArbolM * izq;
    struct nodoArbolM * der;
}nodoArbolM;

typedef struct{
    int level;
    nodoArbolM * stages;
    struct nodoListaN * sig;
}nodoListaN;

typedef struct{
    int posY;
    int posX;
    int id;
    int orientation;
    int* tickrate;
    int attack;
    int hp;
}npc;

//DECLARAMOS CONSTANTES
const int fil = 27;
const int col = 96;
const int filMostrar = 27;
const int colMostrar = 96;

//VARIABLE GLOGAL

int balaCreada  = 0;

//PROTOTIPADO
player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation);
camara camaraSiguiendoJugador(char mapa[fil][col], camara camarita, player jugador);
npc runnpcs(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet);
void fillMap(char mapa[fil][col]);
void runPlayer(char mapa[fil][col], player jugador, int * currentOrietation);
void shootingEnemy(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet);

//MAIN

int main(){

    int eleccion=1;

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
    char mapa[fil][col];
    player jugador;
    jugador.posY = 12;
    jugador.posX = 48;
    camara camarita;
    camarita.posY = fil/2;
    camarita.posX = col/2;
    npc enemy;
    enemy.posY = 2;
    enemy.posX = 45;
    enemy.id = 2;
    enemy.orientation = 4;

    memset(mapa, 32, sizeof(mapa));

    proyectile bullet;
    bullet.posX = 0;
    bullet.posY = 2;

    int * currentOrientiation = 94;

    int * tickRate = 0;


    printf("Elige tu raza\n\n");
    printf("            Humano       Elfo         Orco ");
    printf("\n\n  ATAQUE      2           3            1  ");
    printf("\n\n  VIDA        7           6            9  \n\n");
    printf("Ingrese |1| para seleccionar humano\n");
    printf("Ingrese |2| para seleccionar elfo\n");
    printf("Ingrese |3| para seleccionar orco\n");

    fflush(stdin);
    scanf("%i", &eleccion);

    while(eleccion!=1 && eleccion!=2 && eleccion!=3)
    {
        printf("\nSu numero es incorrecto, porfavor ingrese 1,2 o 3\n");
        fflush(stdin);
        scanf("%i", &eleccion);
    }


    system("pause");
    system("cls");

    for(;;){
    printf("\x1b[H");
    system("");
    fillMap(mapa);
    jugador = runGameplay(mapa, jugador, &currentOrientiation);
    enemy = runnpcs(mapa, enemy, &tickRate, jugador, &bullet);
    camarita = camaraSiguiendoJugador(mapa, camarita, jugador);
    runPlayer(mapa, jugador, &currentOrientiation);
    runMap(mapa, camarita, jugador, &tickRate);
    sistemaDeVida(&humano,&elfo,&orco,eleccion);
    printf("%i", bullet.posY);
    //printf("\x1b[J");
    }
    return 0;
}

//MUESTRA EL MAPA

void runMap(char mapa[fil][col], camara camarita, player jugador, int * tickRate){
    int i, j;

    printf("%c------------------------------------------------------------------------------------------------%c %i %i, %i\n",218, 191, jugador.posY, jugador.posX, *tickRate);
    for(i=0 + camarita.posY ; i<filMostrar + camarita.posY ;i++){
            printf("|");

        for(j=0 + camarita.posX ; j<colMostrar + camarita.posX ; j++){
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
    if(GetKeyState('A') & 0x8000 && mapa[jugador.posY][jugador.posX - 1] != '#' mapa[jugador.posY][jugador.posX-1] !='$'){
        jugador.posX = jugador.posX - 1;
        mapa[jugador.posY][jugador.posX + 1] = ' ';
        *currentOrientation = 60;
    }

    return jugador;
}

//HACEMOS PAREDES

void fillMap(char mapa[fil][col]){
    int i,j;
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
//AHORA SI HACE LO QUE QUIERAS U-U


}

//CAMARA QUE SIGUE AL JUGADOR

camara camaraSiguiendoJugador(char mapa[fil][col], camara camarita, player jugador){
    camarita.posY = 0;
    camarita.posX = 0;
    return camarita;
}

//HACEMOS COSAS CON LOS GUARDIAS

npc runnpcs(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet){


    if(enemy.id == 1){
    mapa[enemy.posY][enemy.posX] = '$';

    if(*tickRate % 5 == 0){

    if(enemy.orientation == 1){
    enemy.posY = enemy.posY -1;
    if(mapa[enemy.posY+1][enemy.posX] != '#'){
    mapa[enemy.posY+1][enemy.posX] = ' ';
    }
    if(mapa[enemy.posY -1][enemy.posX] == '#'){
        enemy.orientation = 3;
    }
   }
    if(enemy.orientation == 3){
    enemy.posY = enemy.posY +1;
    if(mapa[enemy.posY-1][enemy.posX] != '#'){
    mapa[enemy.posY-1][enemy.posX] = ' ';
    }
    if(mapa[enemy.posY +1][enemy.posX] == '#'){
        enemy.orientation = 1;
    }
    }
    if(enemy.orientation == 2){
    enemy.posX = enemy.posX +1;
    if(mapa[enemy.posY][enemy.posX -1] != '#'){
    mapa[enemy.posY][enemy.posX -1] = ' ';
    }
    if(mapa[enemy.posY][enemy.posX +1] == '#'){
        enemy.orientation = 4;
    }
    }
    if(enemy.orientation == 4){
    enemy.posX = enemy.posX -1;
    if(mapa[enemy.posY][enemy.posX +1] != '#'){
    mapa[enemy.posY][enemy.posX +1] = ' ';
    }
    if(mapa[enemy.posY][enemy.posX-1] == '#'){
        enemy.orientation = 2;
    }
    }
    }
    if(*tickRate % rand()%15 == 0 && *tickRate != 0){
        enemy.orientation = rand()%4;
        if(enemy.orientation ==0){
            enemy.orientation = 1;
        }
    }
}

    // Enemigo que te sigue pegado a la pared O_O

    else if(enemy.id == 2){
        mapa[enemy.posY][enemy.posX] = '$';

    if(mapa[enemy.posY][enemy.posX+1] !='<' && mapa[enemy.posY][enemy.posX+1] !='>' && mapa[enemy.posY][enemy.posX+1] !='v' && mapa[enemy.posY][enemy.posX+1] !='^' && mapa[enemy.posY][enemy.posX-1] !='<' && mapa[enemy.posY][enemy.posX-1] !='>' && mapa[enemy.posY][enemy.posX-1] !='v' && mapa[enemy.posY][enemy.posX-1] !='^'){

    if(*tickRate % rand()%5 == 0 && *tickRate != 0){
        if(enemy.posX < jugador.posX){
            enemy.posX = enemy.posX +1;
            mapa[enemy.posY][enemy.posX-1] = ' ';
        }else if(enemy.posX > jugador.posX){
            enemy.posX = enemy.posX -1;
            mapa[enemy.posY][enemy.posX+1] = ' ';
        }else{
        }
            shootingEnemy(mapa, enemy, tickRate, jugador, bullet);
    }
    }
}

    *tickRate = *tickRate +1;

    return enemy;

}

void shootingEnemy(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet)
{

            if(enemy.posY<jugador.posY)
            {

                if(balaCreada == 0){
                bullet->posX = enemy.posX;
                balaCreada =1;
            }
                if(mapa[bullet->posY + 1][bullet->posX] != '#' && (bullet->posY + bullet->posX != jugador.posY + jugador.posX)){
                bullet->posY = bullet->posY + 1;
                mapa[bullet->posY][bullet->posX] = 'o';
                }else{
                mapa[bullet->posY][bullet->posX] = ' ';
                bullet->posY = enemy.posY + 1;
                balaCreada = 0;
                }
                if(mapa[bullet->posY-1][bullet->posX] == 'o'){
                mapa[bullet->posY-1][bullet->posX] = ' ';
                }

        }

}


void collision(char mapa[fil][col],npc enemy, int * tickRate, player jugador, proyectile * bullet)
{
    if()


}



nodoArbolM * iniciArbol()
{
    nodoArbolM * nodo=NULL;
    return nodo;
}
nodoListaN * inicLista()
{
    nodoListaN * nodo=NULL;
    return nodo;
}

/*nodoArbolM * crearNodoArbol(sala sala)
{
    nodoArbol * aux= (nodoArbol*) malloc(sizeof(nodoArbol));
    aux->sala=sala;
    aux->izq=NULL;
    aux->der=NULL;
    return aux;
}
*/
nodoArbolM * irAsala(nodoArbolM * mapa, int numSala)
{
nodoArbolM * rta = iniciArbol();
if(mapa !=NULL)
    {
       if(numSala==mapa->sala.cuarto)
       {
       rta=mapa;
       }
    if(numSala > mapa->sala.cuarto)
        {
            rta=irAsala(mapa->der, numSala);
        }
    else
        {
            rta=irAsala(mapa->izq, numSala);
        }
}
return rta;
}

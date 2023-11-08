#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int posY;
    int posX;
}camara;

typedef struct{
    int posY;
    int posX;
}player;

typedef struct{
    int posY;
    int posX;
    int visionRange;
    int orientation;
}npc;

const int fil = 256;
const int col = 256;
const int filMostrar = 24;
const int colMostrar = 96;




player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation);
camara camaraSiguiendoJugador(char mapa[fil][col], camara camarita, player jugador);
npc runGuards(char mapa[fil][col],npc guard, int * tickRate);
void fillMap(char mapa[fil][col]);
void runPlayer(char mapa[fil][col], player jugador, int * currentOrietation);
int main(){

    char mapa[fil][col];
    player jugador;
    jugador.posY = 243;
    jugador.posX = 128;
    camara camarita;
    camarita.posY = fil/2 - 12;
    camarita.posX = col/2 - 48;
    npc guard;
    guard.posY = 238;
    guard.posX = 122;
    guard.orientation = 4;

    memset(mapa, 32, sizeof(mapa));

    int * currentOrientiation = 94;

    int * tickRate = 0;

    for(;;){
    printf("\x1b[H");
    system("");
    fillMap(mapa);
    jugador = runGameplay(mapa, jugador, &currentOrientiation);
    guard = runGuards(mapa, guard, &tickRate);
    camarita = camaraSiguiendoJugador(mapa, camarita, jugador);
    runPlayer(mapa, jugador, &currentOrientiation);
    runMap(mapa, camarita, jugador, &tickRate);
    //printf("\x1b[J");
    }
    return 0;
}

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
void runPlayer(char mapa[fil][col], player jugador, int * currentOrietation){
    mapa[jugador.posY][jugador.posX] = *currentOrietation;
}
player runGameplay(char mapa[fil][col], player jugador, int * currentOrientation){
    if(GetKeyState('S') & 0x8000 && mapa[jugador.posY + 1][jugador.posX] != '#'){
        jugador.posY = jugador.posY + 1;
        mapa[jugador.posY - 1][jugador.posX] = ' ';
        *currentOrientation = 118;
    }
    if(GetKeyState('W') & 0x8000 && mapa[jugador.posY - 1][jugador.posX] != '#'){
        jugador.posY = jugador.posY - 1;
        mapa[jugador.posY + 1][jugador.posX] = ' ';
        *currentOrientation = 94;
    }
    if(GetKeyState('D') & 0x8000 && mapa[jugador.posY][jugador.posX + 1] != '#'){
        jugador.posX = jugador.posX + 1;
        mapa[jugador.posY][jugador.posX - 1] = ' ';
        *currentOrientation = 62;
    }
    if(GetKeyState('A') & 0x8000 && mapa[jugador.posY][jugador.posX - 1] != '#'){
        jugador.posX = jugador.posX - 1;
        mapa[jugador.posY][jugador.posX + 1] = ' ';
        *currentOrientation = 60;
    }

    return jugador;
}

void fillMap(char mapa[fil][col]){
    int i,j;
 //PAREDES BORDER NO BORRAR BAJO NINGUNA SIRCUNSTANCIA :3
    for(i = 0; i < fil; i++){
        for(j = colMostrar/2; j<colMostrar/2; j++){
            mapa[i][j] = '#';
        }
    }
    for(i = 0; i < fil; i++){
        for(j = col - colMostrar/2; j<col; j++){
            mapa[i][j] = '#';
        }
    }
    for(i = fil - filMostrar/2; i < fil; i++){
        for(j = 0; j<col; j++){
            mapa[i][j] = '#';
        }
    }
    for(i = 0; i < filMostrar/2; i++){
        for(j = 0; j<col; j++){
            mapa[i][j] = '#';
        }
    }
//AHORA SI HACE LO QUE QUIERAS U-U
   for(i = 2; i<126; i++){
        for(j = 66; j<70; j++){
            mapa[i][j] = '#';
        }
    }

}
camara camaraSiguiendoJugador(char mapa[fil][col], camara camarita, player jugador){
    camarita.posY = jugador.posY - 12;
    camarita.posX = jugador.posX - 48;
    return camarita;
}

npc runGuards(char mapa[fil][col],npc guard, int * tickRate){
    mapa[guard.posY][guard.posX] = '$';

    while(*tickRate % 5 == 0){

    if(guard.orientation == 1){
    guard.posY = guard.posY -1;
    if(mapa[guard.posY+1][guard.posX] != '#'){
    mapa[guard.posY+1][guard.posX] = ' ';
    }
    if(mapa[guard.posY -1][guard.posX] == '#'){
        guard.orientation = 3;
    }
   }
    if(guard.orientation == 3){
    guard.posY = guard.posY +1;
    if(mapa[guard.posY-1][guard.posX] != '#'){
    mapa[guard.posY-1][guard.posX] = ' ';
    }
    if(mapa[guard.posY +1][guard.posX] == '#'){
        guard.orientation = 1;
    }
    }
    if(guard.orientation == 2){
    guard.posX = guard.posX +1;
    if(mapa[guard.posY][guard.posX -1] != '#'){
    mapa[guard.posY][guard.posX -1] = ' ';
    }
    if(mapa[guard.posY][guard.posX +1] == '#'){
        guard.orientation = 4;
    }
    }
    if(guard.orientation == 4){
    guard.posX = guard.posX -1;
    if(mapa[guard.posY][guard.posX +1] != '#'){
    mapa[guard.posY][guard.posX +1] = ' ';
    }
    if(mapa[guard.posY][guard.posX-1] == '#'){
        guard.orientation = 2;
    }
    }
    mostrarCono(mapa,guard);
    *tickRate = *tickRate+1;

    }
    *tickRate = *tickRate+1;

    return guard;
}

void mostrarCono(char mapa [fil][col],npc guard)
{
    //CONO MIRANDO AL NORTE ^

    if(guard.orientation==1)
    {
    mapa[guard.posY-1][guard.posX-1]='\\';
    mapa[guard.posY][guard.posX-1]=' ';
    mapa[guard.posY-2][guard.posX-2]='\\';
    mapa[guard.posY-1][guard.posX-2]=' ';
    mapa[guard.posY-3][guard.posX-3]='\\';
    mapa[guard.posY-2][guard.posX-3]=' ';

    mapa[guard.posY-1][guard.posX+1]='/';
    mapa[guard.posY][guard.posX+1]=' ';
    mapa[guard.posY-2][guard.posX+2]='/';
    mapa[guard.posY-1][guard.posX+2]=' ';
    mapa[guard.posY-3][guard.posX+3]='/';
    mapa[guard.posY-2][guard.posX+3]=' ';

    mapa[guard.posY-4][guard.posX+3]=196;
    mapa[guard.posY-4][guard.posX+2]=196;
    //mapa[guard.posY-3][guard.posX+2]=' ';
    mapa[guard.posY-4][guard.posX+1]=196;
    //mapa[guard.posY-3][guard.posX+1]=' ';
    mapa[guard.posY-4][guard.posX]=196;
    //mapa[guard.posY-3][guard.posX]=' ';
    mapa[guard.posY-4][guard.posX-1]=196;
    //mapa[guard.posY-3][guard.posX-1]=' ';
    mapa[guard.posY-4][guard.posX-2]=196;
    //mapa[guard.posY-3][guard.posX-2]=' ';
    mapa[guard.posY-4][guard.posX-3]=196;
    }

    // CONO MIRANDO AL SUR v

    if(guard.orientation==3)
    {
    mapa[guard.posY+1][guard.posX-1]='/';
    mapa[guard.posY][guard.posX-1]=' ';
    mapa[guard.posY+2][guard.posX-2]='/';
    mapa[guard.posY+1][guard.posX-2]=' ';
    mapa[guard.posY+3][guard.posX-3]='/';
    mapa[guard.posY+2][guard.posX-3]=' ';

    mapa[guard.posY+1][guard.posX+1]='\\';
    mapa[guard.posY][guard.posX+1]=' ';
    mapa[guard.posY+2][guard.posX+2]='\\';
    mapa[guard.posY+1][guard.posX+2]=' ';
    mapa[guard.posY+3][guard.posX+3]='\\';
    mapa[guard.posY+2][guard.posX+3]=' ';

    mapa[guard.posY+4][guard.posX+3]=196;
    mapa[guard.posY+4][guard.posX+2]=196;
    //mapa[guard.posY+3][guard.posX+2]=' ';
    mapa[guard.posY+4][guard.posX+1]=196;
    //mapa[guard.posY+3][guard.posX+1]=' ';
    mapa[guard.posY+4][guard.posX]=196;
    //mapa[guard.posY+3][guard.posX]=' ';
    mapa[guard.posY+4][guard.posX-1]=196;
    //mapa[guard.posY+3][guard.posX-1]=' ';
    mapa[guard.posY+4][guard.posX-2]=196;
    //mapa[guard.posY+3][guard.posX-2]=' ';
    mapa[guard.posY+4][guard.posX-3]=196;
    }

    // CONO MIRANDO AL ESTE >

    if(guard.orientation==2)
    {
    mapa[guard.posY-1][guard.posX+1]='/';
        mapa[guard.posY-1][guard.posX]=' ';
    mapa[guard.posY-2][guard.posX+2]='/';
        mapa[guard.posY-2][guard.posX+1]=' ';
    mapa[guard.posY-3][guard.posX+3]='/';
        mapa[guard.posY-3][guard.posX+2]=' ';

    mapa[guard.posY+1][guard.posX+1]='\\';
    mapa[guard.posY+1][guard.posX]=' ';
    mapa[guard.posY+2][guard.posX+2]='\\';
    mapa[guard.posY+2][guard.posX+1]=' ';
    mapa[guard.posY+3][guard.posX+3]='\\';
    mapa[guard.posY+3][guard.posX+2]=' ';

    mapa[guard.posY+3][guard.posX+4]='|';
    mapa[guard.posY+2][guard.posX+4]='|';
    mapa[guard.posY+1][guard.posX+4]='|';
    mapa[guard.posY][guard.posX+4]='|';
    mapa[guard.posY-1][guard.posX+4]='|';
    mapa[guard.posY-2][guard.posX+4]='|';
    mapa[guard.posY-3][guard.posX+4]='|';
    }

    // CONO MIRANDO AL OESTE <

        if(guard.orientation==4)
    {
    mapa[guard.posY-1][guard.posX-1]='\\';
        mapa[guard.posY-1][guard.posX]=' ';
    mapa[guard.posY-2][guard.posX-2]='\\';
        mapa[guard.posY-2][guard.posX-1]=' ';
    mapa[guard.posY-3][guard.posX-3]='\\';
        mapa[guard.posY-3][guard.posX-2]=' ';

    mapa[guard.posY+1][guard.posX-1]='/';
    mapa[guard.posY+1][guard.posX]=' ';
    mapa[guard.posY+2][guard.posX-2]='/';
    mapa[guard.posY+2][guard.posX-1]=' ';
    mapa[guard.posY+3][guard.posX-3]='/';
    mapa[guard.posY+3][guard.posX-2]=' ';

    mapa[guard.posY+3][guard.posX-4]='|';
    mapa[guard.posY+2][guard.posX-4]='|';
    mapa[guard.posY+1][guard.posX-4]='|';
    mapa[guard.posY][guard.posX-4]='|';
    mapa[guard.posY-1][guard.posX-4]='|';
    mapa[guard.posY-2][guard.posX-4]='|';
    mapa[guard.posY-3][guard.posX-4]='|';
    }


}

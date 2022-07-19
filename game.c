/*  Feito por Lucas Lopes do Nascimento
    Trabalho final de Algoritmos e Programação - "Hardest Game Ever"
    Gostaria de agradecer o apoio de todos que acreditaram em mim especialmente minha mãe
    A partir de agora estão organizadas diversas funções que fazem este jogo funcionar, começando
    com funções básicas para carregar o mapa, ou imprimi-lo na tela, e avançando para outras funções
    mais importantes como a de movimentação e de detecção de colisão.
    Todo o código e organização do programa é autoral, obviamente.
*/

#include <stdio.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>

#define NICK_LENGHT 26
#define MAP_HEIGHT 20
#define MAP_WIDTH 82
#define MAX_H_ENEMY 20
#define MAX_V_ENEMY 20
#define PLAYER 'O'
#define EMPTY_SPACE '-'
#define HORIZONTAL_ENEMY 'H'
#define VERTICAL_ENEMY 'V'
#define COIN 'M'
#define BORDER 'X'
#define FINISH 'C'
#define UP 'U'
#define DOWN 'D'
#define LEFT 'L'
#define RIGHT 'R'

typedef struct {

    /*  A estrutura "Player" guarda as informações do jogador.
        Essa estrutura foi declarada em primeiro lugar porque
        faz parte de outra estrutura que será apresentada mais
        para frente.*/

    int xPos, yPos, score, killCount,
    initX, initY;
    int won;
    char name[NICK_LENGHT];

} Player;

typedef struct {

    /*  A estrutura "VerticalEnemy" é onde são guardadas as informações
        das entidades com movimentos verticais no código. Os "inimigos". */

    int xPos, yPos;
    char direction;

} VerticalEnemy;

typedef struct {

    /*  A Estrutura "HorizontalEnemy" é a mesma coisa da VerticalEnemy,
        só que para entidades com movimentos horizontais. Ambas foram
        declaradas aqui porque fazem parte de um array de outra estrutura. */

    int xPos, yPos;
    char direction;

} HorizontalEnemy;

typedef struct {

    /*  A Estrutura Game é a base de todo o jogo, ela é normalmente manipulada pela classe main(),
        e com ela eu posso ter todas as informações do jogo em  um lugar só, e bem organizado.
        muito útil na hora de pensar em quais parâmetros devem ser enviados para quais funções. */

    Player player;
    VerticalEnemy vEnemies[MAX_H_ENEMY];
    HorizontalEnemy hEnemies[MAX_V_ENEMY];
    int vEnemyCount;
    int hEnemyCount;
    char map[MAP_WIDTH][MAP_HEIGHT];

} Game;

FILE* openFile(char fileName[20]) {  // Funçãozinha só pra simplificar a abertura de arquivos, não é necessário mas eu prefiro assim.

    FILE *a = fopen(fileName, "r");
    return a;

}

Game loadGame(FILE *arq) { //Carrega o mapa do jogo.

    /*  Esse demônio aqui faz o carregamento do mapa. A função funciona da seguinte maneira:
        Para cada caractere que a função lê do arquivo de texto que contém o mapa, ela vai populando
        e escalonando no array que possui o mapa do jogo, que já é escalonado para o tamanho desejado.
        O escalonamento é feito da seguinte maneira: Cada caractere é salvo quatro vezes, para compensar
        pelo tamanho do mapa no arquivo. Ela em sí não printa nada na tela, isso é trabalho de outra função
        que só tem o mapa (que é um atributo da estrutura Game) como parâmetro. */

    int row, col, rowHelper, colHelper; //As variáveis helper são usadas para escalonar a matriz.
    char mapPiece;
    Game game;
    game.hEnemyCount = 0; game.vEnemyCount = 0;
    srand(time(NULL));

    for (col=0; col<10; col++) {

        for (row=0; row<41; row++){

            mapPiece = getc(arq);

            char vDirections[2] = {'D', 'U'};
            char hDirections[2] = {'L', 'R'};

            rowHelper = row;
            colHelper = col;

            if(mapPiece == PLAYER) { //Quando achar o player, inicializa as variáveis da estrutura player, que tá na estrutura game.

                game.map[rowHelper*2][colHelper*2] = mapPiece;
                game.player.won = 0;
                game.player.xPos = col*2; //Salvando as coordenadas iniciais do jogador.
                game.player.yPos = row*2;
                game.player.initX = game.player.xPos; //Salvando as coordenadas de respawn do jogador
                game.player.initY = game.player.yPos;
                game.player.score = 1000;
                game.player.killCount = 0;
                game.map[rowHelper*2+1][colHelper*2] = EMPTY_SPACE; //Por opção minha, resolvi não escalonar o jogador, porque
                game.map[rowHelper*2][colHelper*2+1] = EMPTY_SPACE; //achei melhor de jogar com um player menor.
                game.map[rowHelper*2+1][colHelper*2+1] = EMPTY_SPACE;

            } else if (mapPiece == VERTICAL_ENEMY){

                game.map[rowHelper*2][colHelper*2] = mapPiece;
                game.vEnemies[game.vEnemyCount].xPos = col*2;
                game.vEnemies[game.vEnemyCount].yPos = row*2;
                game.vEnemies[game.vEnemyCount].direction = vDirections[rand()%2]; //Cada vez que o mapa é carregado, a direção de cada inimigo é randômica.
                game.map[rowHelper*2+1][colHelper*2] = mapPiece;
                game.map[rowHelper*2][colHelper*2+1] = mapPiece;
                game.map[rowHelper*2+1][colHelper*2+1] = mapPiece;
                game.vEnemyCount++;

            } else if (mapPiece == HORIZONTAL_ENEMY){

                game.map[rowHelper*2][colHelper*2] = mapPiece;
                game.hEnemies[game.hEnemyCount].xPos = col*2;
                game.hEnemies[game.hEnemyCount].yPos = row*2;
                game.hEnemies[game.hEnemyCount].direction = hDirections[rand()%2];
                game.map[rowHelper*2+1][colHelper*2] = mapPiece;
                game.map[rowHelper*2][colHelper*2+1] = mapPiece;
                game.map[rowHelper*2+1][colHelper*2+1] = mapPiece;
                game.hEnemyCount++;

            } else {

                game.map[rowHelper*2][colHelper*2] = mapPiece;
                game.map[rowHelper*2+1][colHelper*2] = mapPiece;
                game.map[rowHelper*2][colHelper*2+1] = mapPiece;
                game.map[rowHelper*2+1][colHelper*2+1] = mapPiece;

            }

        }

    }

    return game;

}

void printMap(char map[MAP_WIDTH][MAP_HEIGHT]) { //Só princa o mapa na tela

    int row, col;

    for (col=0; col<MAP_HEIGHT; col++) {

        for (row=0; row<MAP_WIDTH; row++) {
            if (map[row][col]==FINISH){
                textcolor(2);putchxy(row, col, map[row][col]);
            }
            else if (map[row][col]==COIN){
                textcolor(6);putchxy(row, col, map[row][col]);
            }
            else if (map[row][col]==BORDER){
                textcolor(3);putchxy(row, col, map[row][col]);
            }
            else if (map[row][col]==PLAYER){
                textcolor(2);putchxy(row, col, map[row][col]);
            } else {
                textcolor(0);putchxy(row, col, map[row][col]);
            }
        }

    }

}


void showScore(Player p) { //Mostrador do score embaixo do mapa do jogo.
    /*  Essa função deve ser chamada cada vez que o score do jogador mudar. */

    gotoxy(1, 20);clreol();
    gotoxy(1, 20);textcolor(WHITE);printf("%s | Score: %d | %d Mortes", p.name, p.score, p.killCount);

}

void killPlayer(Player *player, char map[MAP_WIDTH][MAP_HEIGHT]) { //Função que mata o jogador.

    clrscr();

    map[player->yPos][player->xPos] = EMPTY_SPACE;

    player->xPos = player->initX;
    player->yPos = player->initY;

    map[player->yPos][player->xPos] = PLAYER;

    player->score = 1000;
    player->killCount++;

    printMap(map);
    showScore(*player); //Mandando o valor e não o ponteiro.

}

void movePlayer(char map[MAP_WIDTH][MAP_HEIGHT], Player *player, char direction) { //Função que movimenta o jogador.

    switch (direction) { //Checando a direção.

        case 'R':   if(map[player->yPos+1][player->xPos]!=BORDER){

                        if(map[player->yPos+1][player->xPos]==COIN){
                            player->score += 30;
                            showScore(*player);
                        }
                        if(map[player->yPos+1][player->xPos]==FINISH)
                            player->won = 1;

                            if (map[player->yPos+1][player->xPos]==VERTICAL_ENEMY ||
                            map[player->yPos+1][player->xPos]==HORIZONTAL_ENEMY)
                            killPlayer(player, map);

                        map[player->yPos+1][player->xPos] = PLAYER;
                        textcolor(2);putchxy(player->yPos+1, player->xPos, PLAYER);

                        map[player->yPos][player->xPos] = EMPTY_SPACE;
                        textcolor(0);putchxy(player->yPos, player->xPos, EMPTY_SPACE);
                        player->yPos++;

                    }
                    break;
        case 'L':   if(map[player->yPos-1][player->xPos]!=BORDER) {

                        if(map[player->yPos-1][player->xPos]==COIN) {
                            player->score += 30;
                            showScore(*player);
                        }
                        if(map[player->yPos-1][player->xPos]==FINISH)
                            player->won == 1;

                            if (map[player->yPos-1][player->xPos]==VERTICAL_ENEMY ||
                            map[player->yPos-1][player->xPos]==HORIZONTAL_ENEMY)
                            killPlayer(player, map);

                        map[player->yPos-1][player->xPos] = PLAYER;
                        textcolor(2);putchxy(player->yPos-1, player->xPos, PLAYER);
                        map[player->yPos][player->xPos] = EMPTY_SPACE;
                        textcolor(0);putchxy(player->yPos, player->xPos, EMPTY_SPACE);
                        player->yPos--;

                    }
                    break;
        case 'U':   if(map[player->yPos][player->xPos-1]!=BORDER){

                        if(map[player->yPos][player->xPos-1]==COIN) {
                            player->score += 30;
                            showScore(*player);
                        }
                        if(map[player->yPos][player->xPos-1]==FINISH)
                            player->won == 1;

                            if (map[player->yPos][player->xPos-1]==VERTICAL_ENEMY ||
                            map[player->yPos][player->xPos-1]==HORIZONTAL_ENEMY)
                            killPlayer(player, map);

                        map[player->yPos][player->xPos-1] = PLAYER;
                        textcolor(2);putchxy(player->yPos, player->xPos-1, PLAYER);
                        map[player->yPos][player->xPos] = EMPTY_SPACE;
                        textcolor(0);putchxy(player->yPos, player->xPos, EMPTY_SPACE);
                        player->xPos--;

                    }
                    break;
        case 'D':   if(map[player->yPos][player->xPos+1]!=BORDER){

                        if (map[player->yPos][player->xPos+1]==COIN) {
                            player->score  += 30;
                            showScore(*player);
                        }
                        if (map[player->yPos][player->xPos+1]==FINISH)
                            player->won == 1;

                        if (map[player->yPos][player->xPos+1]==VERTICAL_ENEMY ||
                            map[player->yPos][player->xPos+1]==HORIZONTAL_ENEMY)
                            killPlayer(player, map);

                        map[player->yPos][player->xPos+1] = PLAYER;
                        textcolor(2);putchxy(player->yPos, player->xPos+1, PLAYER);
                        map[player->yPos][player->xPos] = EMPTY_SPACE;
                        textcolor(0);putchxy(player->yPos, player->xPos, EMPTY_SPACE);
                        player->xPos++;
                    }
                    break;



    }

}

void verticalMovingEnemy(char map[MAP_WIDTH][MAP_HEIGHT], VerticalEnemy *enemy, Player *player) { //Função que movimenta as entidades com movimentos verticais.

    /*  Como o movimento deve ser contínuo para alguma direção, eu checo se está livre da
        borda no final. Como a direção é inicializada na hora de carregar o mapa é randômica,
        se a entidade estiver colada na borda do mapa e a direção for para a direção da borda
        o inimigo ainda sim vai se mexer, e vai comer um pedaço da borda do mapa. Bugfix em
        breve. Por agora só livrar os inimigos da borda por 1 quadradinho. */

    if(enemy->direction=='D') {

        if (map[enemy->yPos][enemy->xPos-1]==PLAYER ||
            map[enemy->yPos+1][enemy->xPos-1]==PLAYER)
            killPlayer(player, map);

        map[enemy->yPos][enemy->xPos] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos, enemy->xPos, EMPTY_SPACE);
        map[enemy->yPos+1][enemy->xPos] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos+1, enemy->xPos, EMPTY_SPACE);

        map[enemy->yPos][enemy->xPos+1] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos, enemy->xPos+1, VERTICAL_ENEMY);
        map[enemy->yPos][enemy->xPos+2] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos, enemy->xPos+2, VERTICAL_ENEMY);
        map[enemy->yPos+1][enemy->xPos+2] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+1, enemy->xPos+2, VERTICAL_ENEMY);
        map[enemy->yPos+1][enemy->xPos+1] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+1, enemy->xPos+1, VERTICAL_ENEMY);

        enemy->xPos++;

        //printMap(map);

    } else if(enemy->direction=='U') {

        if (map[enemy->yPos][enemy->xPos+2]==PLAYER ||
            map[enemy->yPos+1][enemy->xPos+2]==PLAYER)
            killPlayer(player, map);

        map[enemy->yPos][enemy->xPos+1] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos, enemy->xPos+1, EMPTY_SPACE);
        map[enemy->yPos+1][enemy->xPos+1] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos+1, enemy->xPos+1, EMPTY_SPACE);

        map[enemy->yPos][enemy->xPos] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos, enemy->xPos, VERTICAL_ENEMY);
        map[enemy->yPos][enemy->xPos-1] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos, enemy->xPos-1, VERTICAL_ENEMY);
        map[enemy->yPos+1][enemy->xPos-1] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+1, enemy->xPos-1, VERTICAL_ENEMY);
        map[enemy->yPos+1][enemy->xPos] = VERTICAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+1, enemy->xPos, VERTICAL_ENEMY);

        enemy->xPos--;

    }

    if( map[enemy->yPos][enemy->xPos-1]==BORDER)
        enemy->direction = 'D';
    else if(map[enemy->yPos][enemy->xPos+2]==BORDER)
        enemy->direction = 'U';


}

void horizontalMovingEnemy(char map[MAP_WIDTH][MAP_HEIGHT], HorizontalEnemy *enemy, Player *player) { //Função que movimenta as entidades com movimentos horizontais.

    /*  Como o movimento deve ser contínuo para alguma direção, eu checo se está livre da
        borda no final. Como a direção é inicializada na hora de carregar o mapa é randômica,
        se a entidade estiver colada na borda do mapa e a direção for para a direção da borda
        o inimigo ainda sim vai se mexer, e vai comer um pedaço da borda do mapa. Bugfix em
        breve. Por agora só livrar os inimigos da borda por 1 quadradinho. Mesma coisa. */

    if(enemy->direction=='R') {

        if (map[enemy->yPos-1][enemy->xPos]==PLAYER ||
            map[enemy->yPos-1][enemy->xPos+1]==PLAYER)
            killPlayer(player, map);

        map[enemy->yPos][enemy->xPos] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos, enemy->xPos, EMPTY_SPACE);
        map[enemy->yPos][enemy->xPos+1] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos, enemy->xPos+1, EMPTY_SPACE);

        map[enemy->yPos+1][enemy->xPos] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+1, enemy->xPos, HORIZONTAL_ENEMY);
        map[enemy->yPos+2][enemy->xPos] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+2, enemy->xPos, HORIZONTAL_ENEMY);
        map[enemy->yPos+2][enemy->xPos+1] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+2, enemy->xPos+1, HORIZONTAL_ENEMY);
        map[enemy->yPos+1][enemy->xPos+1] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos+1, enemy->xPos+1, HORIZONTAL_ENEMY);



        enemy->yPos++;

        //printMap(map);

    } else if(enemy->direction=='L') {

        if (map[enemy->yPos+2][enemy->xPos]==PLAYER ||
            map[enemy->yPos+2][enemy->xPos+1]==PLAYER)
            killPlayer(player, map);

        map[enemy->yPos+1][enemy->xPos] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos+1, enemy->xPos, EMPTY_SPACE);
        map[enemy->yPos+1][enemy->xPos+1] = EMPTY_SPACE;
        textcolor(0);putchxy(enemy->yPos+1, enemy->xPos+1, EMPTY_SPACE);

        map[enemy->yPos][enemy->xPos] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos, enemy->xPos, HORIZONTAL_ENEMY);
        map[enemy->yPos-1][enemy->xPos] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos-1, enemy->xPos, HORIZONTAL_ENEMY);
        map[enemy->yPos-1][enemy->xPos+1] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos-1, enemy->xPos+1, HORIZONTAL_ENEMY);
        map[enemy->yPos][enemy->xPos+1] = HORIZONTAL_ENEMY;
        textcolor(4);putchxy(enemy->yPos, enemy->xPos+1, HORIZONTAL_ENEMY);

        enemy->yPos--;

        //printMap(map);

    }

    if( map[enemy->yPos-1][enemy->xPos]==BORDER)
        enemy->direction = 'R';
    else if(map[enemy->yPos+2][enemy->xPos]==BORDER)
        enemy->direction = 'L';


}

void showResult(Player p) { //Função que mostra a tela quando o jogador vence.

    clrscr();
    textcolor(WHITE);
    gotoxy(38, 10);printf("VOCE COMPLETOU ESTE GRANDE DESAFIO NA SUA VIDA");
    gotoxy(38, 12);printf("Score: %d\n", p.score);
    Sleep(3000);

}

void main() {

    FILE *arq;
    Game game;
    char map1[15] = "nivel-[1].txt";
    char map2[15] = "nivel-[2].txt";
    char map3[15] = "nivel-[3].txt";
    char playerName[26];
    int sCont=0;
    int levelCont = 1;

    gotoxy(50, 10);printf("JOGO MEIA BOCA");
    gotoxy(53, 11);printf("Nickname");
    gotoxy(50, 12);gets(playerName);
    clrscr();
    gotoxy(52, 10);printf("JOGO MEIA BOCA");
    gotoxy(40, 30);printf("Lucas Nascimento, 2019 All Rights Reserved");    //Creditos, porque capitalismo.
    Sleep(750);

    while (!(GetKeyState(VK_ESCAPE) & 0x80)) {

        textcolor(WHITE);

        do {
            fflush(stdin);
            gotoxy(52, 10);printf("JOGO MEIA BOCA");
            gotoxy(46, 11);printf("Aperte enter para continuar");
            gotoxy(40, 30);printf("Lucas Nascimento, 2019 All Rights Reserved");    //Creditos, porque capitalismo.
            if(levelCont==1)
                arq = openFile(map1);
            else if(levelCont==2)
                arq = openFile(map2);
            else
                arq = openFile(map3);
            Sleep(200);

        } while ((!(GetKeyState(VK_RETURN) & 0x80)));

            clrscr();
            game = loadGame(arq);
            strcpy(game.player.name, playerName);   //Como o mapa acabou de ser carregado, a variável com o nome do jogador lá de cima
            printMap(game.map);                     //é simplesmente copiada para o nome do jogador reconhecido pelo jogo.
            showScore(game.player);



            while ((!(GetKeyState(VK_TAB) & 0x80)) && !game.player.won) {

                    if(sCont==10 && game.player.score> 0) {
                        /*  Esse if controla a perda de pontos do jogador por segundo.
                            como o jogo é atualizado a cada 100ms, eu vou incrementando uma
                            variável que é inicializada em 0, até ela chegar em 10, quando
                            chegar é porque passou um segundo. */

                        game.player.score -= 10;
                        sCont = 0;
                        showScore(game.player);

                    }

                    for (int i=0; i<game.vEnemyCount; i++) //Atualizando o movimento dos inimigos verticais.
                        verticalMovingEnemy(game.map, &game.vEnemies[i], &game.player);

                    for (int i=0; i<game.hEnemyCount; i++) //Atualizando o movimento dos inimigos horizontais.
                        horizontalMovingEnemy(game.map, &game.hEnemies[i], &game.player);

                    if (GetKeyState(VK_RIGHT) & 0x80) {

                        movePlayer(game.map, &game.player, 'R');

                    }
                    if (GetKeyState(VK_LEFT) & 0x80) {

                        movePlayer(game.map, &game.player, 'L');

                    }
                    if (GetKeyState(VK_UP) & 0x80) {

                        movePlayer(game.map, &game.player, 'U');

                    }
                    if (GetKeyState(VK_DOWN) & 0x80) {

                        movePlayer(game.map, &game.player, 'D');

                    }
                    sCont++;
                    Sleep(100);

            }

            fflush(stdin); //As vezes dá erro nos gets lá de cima.

            if (game.player.won) { //Se o player saiu do loop porque ganhou, mostra a tela de vencedor pra ele.
                showResult(game.player);
                if(levelCont==3)
                    levelCont=1;
                else
                    levelCont++;
            }

            clrscr();

    }

}

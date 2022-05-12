#include <stdio.h>
#include "conio2.h"
#include <string.h>
#include <time.h>
#include "windows.h"


#define SIZE 15  //board SIZE has to be odd number, in order to have center
#define BOARD_X 50//position of board from left
#define BOARD_Y 1//same but from above
#define LEGENDPX 1//position of legend from left
#define LEGENDPY 1//some but from above
#define ENTER 0x0d
#define LETTERMP2 2 //letter multiplier x2
#define LETTERMP3 3 //letter multiplier x3
#define WORDMP2 2    //word multiplier x2
#define WORDMP3 3   //word multiplier x3
#define ESCAPE 0x1b

bool checkLettersOnBoard(int m);
void enteringWord(int zn);
bool firstMove1();
void newGame();
void placeLetter(char brd[][SIZE]);

void boardPrint(char board[][SIZE]);
void boardPrintColor();
void boardPrintColorRed();
void boardPrintColorLightBlue();
void boardPrintColorCyan();
void print_my_letters(char mltrs[]);
void initializeBoard(char board[][SIZE]);
void printFrame();

void checkXY(int fx, int fy);//prevents cursor from going beyound board
void positionOnBoard();//prints cursor position
int convertUpper(int z);
bool asciiCheck(int z);//below explanation
bool checkIfLetter(int z, char mltrs[]);//below explanation
void resetPlacingTable();//resets placing table and tabhelp
void testing1();//showing word vertically
void placeWord1();
void testing2();//showing word horizontally
void placeWord2();

void losulosu();//initial distribution od tiles
void losulosu2();//randomized tiles distribution after move or if player chooses to replace tiles
void returnpomocna();//return letters that were in my_letters
void funkcjazwrotna();//below explanation
void legenda();//prints legenda
void returnkafelek1();//below
void returnkafelek2();//below
void replaceKafelki();
void funkcja();//function to replace tiles

bool hejka1();//below
bool hejka12();//checks if at least 1 letter in players word is on board
bool hejka2();//below
bool hejka22();//same as hejka12, but horizontally
bool checking1();//below
bool checks1();//prevents from putting same word on top of the word
bool checks2();

int simplePoints(char pt[], int length);
void verticalLetterMultiplier(int k);
void horizontalLetterMultiplier(int k);
void verticalLetterMultiplier3(int k);
void horizontalLetterMultiplier3(int k);
bool verticalWord2(int k);
bool horizontalWord2(int k);
bool verticalWord3(int k);
bool horizontalWord3(int k);
bool fiftyPoints();

bool firstMove1Check(int length);
bool firstMove2Check(int length);
bool firstMove2();


bool checkDifference1();//checks if letter that isnt in tabhelp is on board
bool checkDifference2();

char replacetab[7];//stores letters we want to replace
unsigned int punkty=0;// char to help multiply points
unsigned int points=0;

int x=BOARD_X+2*(SIZE/2)-1, y=BOARD_Y+(SIZE/2)+1;//
char board[SIZE][SIZE];
int corx;//coordinates, cursor position
int cory;
char tabhelp[7]; //stores letter that are in our my_letters if we enter them during 'i' option
char difference[7];//stores difference between tabhelp and placingtable
int pomocna=0;   //helps to operate on arrays
char placingTable[SIZE]
; // board that stores word we want to enter
unsigned int pt_length=strlen(placingTable);//number of elements in placing table
//int k=0;
int optiono=0;//responsible for changing word orientation
char letters1[]= {'e','e','e','e','e','e','e','e','e','e','e','e','a','a','a','a','a','a','a','a','a',
                 'i','i','i','i','i','i','i','i','i','o','o','o','o','o','o','o','o','n','n','n','n','n','n','r','r','r','r',
                 'r','r','t','t','t','t','t','t','l','l','l','l','s','s','s','s','u','u','u','u','d','d','d','d','g','g','g',
                 'b','b','c','c','m','m','p','p','f','f','h','h','v','v','w','w','y','y','k','j','x','q','z', '@', '@'
                };
char letters[100];

char my_letters[7]; //our letter to operate with
int main()
{
    int zn = 0;
    for(int i=0; i<strlen(letters1); i++){
    letters[i]=letters1[i];
    }
    losulosu();
    initializeBoard(board);//fills board with spaces
#ifndef __cplusplus
    Conio2_Init();
#endif
    SetConsoleTitle("Adam Kasperski, s175945");

    //settitle("First name, Last Name, Student number");
    _setcursortype(_NORMALCURSOR );
    do
    {
        clrscr();
        checkXY(x, y);//prevents cursor from going beyond board
        corx=x+1;//has to be +1, because of double jump
        cory=y;
        int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
        int tcory=cory-BOARD_Y-1;
        legenda();//prints legenda programu
        boardPrint(board);//print board with white background
        textbackground(BLACK);
        print_my_letters(my_letters);
        int positiony=tcory+1;
        char positionx=tcorx+65;
        char buffer[10];
        sprintf(buffer, "%c %d", positionx, positiony); //prints position of the cursor
        gotoxy(LEGENDPX, LEGENDPY+9);
        cputs("Position:");cputs(buffer);
        //positionOnBoard();
        gotoxy(1, 20);
        if(optiono==0){
            testing1();//if variable optiono is eq to 0, then showing word on board is vertical
        }
        else{
            testing2();//same, but horizontal
        }
        gotoxy(corx, cory);

        zn = getch();
        enteringWord(zn);
        if(zn=='i'&& strlen(placingTable)==0){
                for(int i=0; i<7; i++){difference[i]==0;}
                placeLetter(board);//we can type word we want to place
                if(points==0&&firstMove1Check(strlen(placingTable))==false){
                    returnpomocna();
                    resetPlacingTable();
                    gotoxy(1, BOARD_Y+10);
                    cputs("Cannot place word");getch();
                }
                else if(checking1()==false){
                    gotoxy(1, BOARD_Y+10);
                    cputs("You must enter at least 1 letter of your own");
                    getch();
                    resetPlacingTable();

                }
                else if(strlen(placingTable)==1&&points!=0){
                    returnpomocna();
                    resetPlacingTable();
                    gotoxy(1, BOARD_Y+10);
                    cputs("Too short");getch();
                }
        }

        if(zn=='o'){
            optiono=!optiono;//changes orientation from horizontal to vertical and vice versa
        }
        if(zn=='n'){
            newGame();
        }
        if(zn==ESCAPE){ //escape current action(placing word) and return letters
            returnpomocna();
            resetPlacingTable();
            pomocna=0;
            clrscr();
            zn='i';
        }
        if(zn=='w'){
            replaceKafelki();//we can start a move that we can replace out letters
        }
        if(zn == 0)
        {
            // if this is the case then we read
            zn = getch();               // the next code knowing that this
            if(zn == 0x48)
                y--;            // will be a special key
            else if(zn == 0x50)
                y++;
            else if(zn == 0x4b)
                x=x-2;
            else if(zn == 0x4d)
                x=x+2;
        }
        if(strlen(placingTable)==0){losulosu2();}
    }
    while (zn != 'q');
    return 0;
}

void enteringWord(int zn)
{
    if(zn==ENTER){
            if(optiono==0&&hejka1()==true){//hejka checks if we can place a word on board
                if(points==0){//if this is a first move
                    if(firstMove1()){//if word goes through middle of the board, return true
                        simplePoints(placingTable,strlen(placingTable));
                        for(int i=0; i<strlen(placingTable); i++){
                            verticalLetterMultiplier(i);
                        }
                        points+=punkty;
                        punkty=0;
                        placeWord1();
                        if(fiftyPoints()){
                            points+=50;
                        }
                        resetPlacingTable();
                        losulosu2();
                    }else{
                        gotoxy(1,1);
                        cputs("Word must go through middle of the board");
                        returnpomocna();
                        resetPlacingTable();
                        getch();
                      }
                }else if(hejka12()==true&&checks1()==true&&checkDifference1()==true){
                    simplePoints(placingTable, strlen(placingTable));
                    for(int i=0; i<strlen(placingTable); i++){
                        verticalLetterMultiplier(i);
                    }
                    for(int i=0; i<strlen(placingTable); i++){
                        verticalLetterMultiplier3(i);
                    }
                    for(int i=0; i<strlen(placingTable); i++){
                        if(verticalWord2(i)){
                            punkty*=2;
                        }
                    }
                    for(int i=0; i<strlen(placingTable); i++){
                        if(verticalWord3(i)){
                            punkty*=3;
                        }
                    }
                    points+=punkty;
                    punkty=0;
                    returnkafelek1();//returns kafelki that are already on board
                    placeWord1();//places word
                    if(fiftyPoints()){
                            points+=50;
                        }
                    resetPlacingTable();//resets placing table
                    losulosu2();//losuje letters in place of used letters
                    }
                else{
                    returnpomocna();
                    resetPlacingTable();
                    gotoxy(1,1);
                    cputs("Cant place tiles here!");
                    getch();
            }
            }
            else if(optiono==1&&hejka2()==true){
                if(points==0){//if this is a first move
                    if(firstMove2()){
                        simplePoints(placingTable,strlen(placingTable));
                        for(int i=0; i<strlen(placingTable); i++){
                            horizontalLetterMultiplier(i);
                        }
                        points+=punkty;
                        punkty=0;
                        placeWord2();
                        if(fiftyPoints()){
                            points+=50;
                        }
                        resetPlacingTable();
                        losulosu2();
                    }else{
                        gotoxy(1,1);
                        cputs("Word must go through middle of the board");
                        returnpomocna();
                        resetPlacingTable();
                        getch();
                      }
                }else if(hejka22()==true&&checks2()==true&&checkDifference2()==true){
                    simplePoints(placingTable, strlen(placingTable));
                    for(int i=0; i<strlen(placingTable); i++){
                        horizontalLetterMultiplier(i);
                    }
                    for(int i=0; i<strlen(placingTable); i++){
                        horizontalLetterMultiplier3(i);
                    }
                    for(int i=0; i<strlen(placingTable); i++){
                        if(horizontalWord2(i)){
                        punkty*=WORDMP2;
                        }
                    }
                    for(int i=0; i<strlen(placingTable); i++){
                        if(horizontalWord3(i)){
                        punkty*=WORDMP3;
                        }
                    }
                    points+=punkty;
                    punkty=0;
                    returnkafelek2();
                    placeWord2();
                    if(fiftyPoints()){
                        points+=50;
                    }
                    resetPlacingTable();
                    losulosu2();
                }else{
                    returnpomocna();//if we cannot place a word, return letters
                    resetPlacingTable();
                    gotoxy(1,1);
                    cputs("Cant place tiles here!");
                    getch();
                }
            }
            else{
                returnpomocna();//if we cannot place a word, return letters
                resetPlacingTable();
                gotoxy(1,1);
                cputs("Cant place tiles here!");
                getch();
            }
        }textbackground(BLACK);
}


bool firstMove1Check(int length){
    bool status=false;
    for(int i=0; i<strlen(placingTable); i++){
        if(placingTable[i]==tabhelp[i]||tabhelp[i]=='@'){
            status=true;
        }else{
            status=false;
            break;}
    }
    return status;
}

void moving(int zn)
{
    if(zn == 0xe0)
        {
            // if this is the case then we read
            zn = getch();               // the next code knowing that this
            if(zn == 0x48)
                y--;    // will be a special key
            else if(zn == 0x50)
                y++;
            else if(zn == 0x4b)
                x=x-2;
            else if(zn == 0x4d)
                x=x+2;
        }
}
bool firstMove1()
{
    bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(tcorx==(SIZE-1)/2&&tcory+i==(SIZE-1)/2){
            status=true;
            break;
        }
    }
    return status;
}
bool firstMove2()
{
    bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(tcorx+i==(SIZE-1)/2&&tcory==(SIZE-1)/2){
            status=true;
            break;
        }
    }
    return status;
}

void initializeBoard(char board[][SIZE])//sets board to spaces
{
    for(int i=0; i<SIZE; i++)
    {
        for(int j=0; j<SIZE; j++)
        {
            board[i][j]=' ';
        }
    }
}
void boardPrint(char board[][SIZE])//prints board
{
    printFrame();
    for(int x=0; x<SIZE; x++)
    {
        for(int y=0; y<SIZE; y++)
        {
            gotoxy(2*x+BOARD_X, y+BOARD_Y+1);//+1 is space for "ABC" frame
            textbackground(WHITE);           //2*x so we can get a square board
            textcolor(BLACK);
            putch(board[x][y]);

            if(x<SIZE-1) //cuts last col of spaces
            {
                gotoxy(2*x+BOARD_X+1, y+BOARD_Y+1);//+1 to insert spaces
                textbackground(WHITE);
                putch(' ');
            }
        }
    }
    textcolor(WHITE);
    boardPrintColor();
    boardPrintColorLightBlue();
    boardPrintColorRed();
    boardPrintColorCyan();
}
void boardPrintColor()
{
    textbackground(13);
    textcolor(WHITE);
    for(int x=0; x<SIZE; x++){
        for(int y=0; y<SIZE; y++){
            if(x==y&&x>0&&x<5){
                gotoxy(2*x+BOARD_X, y+BOARD_Y+1);
                putch(board[x][y]);

            }
            if(x==y&&x>9&&x<14){
                gotoxy(2*x+BOARD_X, y+BOARD_Y+1);
                putch(board[x][y]);
            }
            if(x==SIZE-y-1&&x>0&&x<5){
                gotoxy(2*x+BOARD_X, y+BOARD_Y+1);
                putch(board[x][y]);
            }
            if(x==SIZE-y-1&&x>9&&x<14){
                gotoxy(2*x+BOARD_X, y+BOARD_Y+1);
                putch(board[x][y]);
            }
        }
    }
}

void boardPrintColorRed()
{
    int pozycje[][2]= {{0, 0},{7, 0},{14, 0},{0, 7},{14,7},{0, 14},{7, 14},{14, 14}};
    int j=0;
    int x=0, y=0;
    for(int i=0; i<8; i++){
        gotoxy(BOARD_X+2*pozycje[i][j], BOARD_Y+1+pozycje[i][j+1]);
        textbackground(4);
        textcolor(WHITE);
        x=pozycje[i][j];
        y=pozycje[i][j+1];
        putch(board[x][y]);
        }
}
void boardPrintColorLightBlue()
{
    textbackground(LIGHTBLUE);
    int pozycje[][2]= {{5,5},{9,5},{5,9},{9,9}, {5, 1}, {9, 1}, {1, 5}, {13, 5}, {1, 9}, {13, 9}, {5, 13}, {9,13}};
    int j=0;
    int x=0, y=0;
    for(int i=0; i<12; i++){
        gotoxy(BOARD_X+2*pozycje[i][j], BOARD_Y+1+pozycje[i][j+1]);
        x=pozycje[i][j];
        y=pozycje[i][j+1];
        putch(board[x][y]);
    }
}
void boardPrintColorCyan()
{
    textbackground(CYAN);
    int pozycje[][2]= {{0,3},{0,11},{2,6},{2,8},{3,0},{3,7},{3,14},{6,6}, {6, 8}, {8, 6}, {8, 8},
        {6, 2}, {6, 12}, {7, 3}, {7, 11}, {8,2}, {8, 12}, {11, 0}, {11, 7}, {11, 14},
        {12, 6}, {12, 8}, {14, 3}, {14, 11}
    };
    int j=0;
    int x=0, y=0;
    for(int i=0; i<24; i++){
        gotoxy(BOARD_X+2*pozycje[i][j], BOARD_Y+1+pozycje[i][j+1]);
        x=pozycje[i][j];
        y=pozycje[i][j+1];
        putch(board[x][y]);
    }
}
void printFrame()
{
    textbackground(BLACK);
    gotoxy(BOARD_X, BOARD_Y);
    for (int i=65; i<65+SIZE; i++)
    {

        putch(i);
        putch(' ');
    }
    gotoxy(BOARD_X, BOARD_Y+SIZE+1);
    for (int i=65; i<80; i++)
    {

        putch(i);
        putch(' ');
    }

    for(int i=1; i<=SIZE; i++)
    {
        char buffer[110];
        gotoxy(BOARD_X-3, BOARD_Y+i);
        sprintf(buffer, "%d", i);
        cputs(buffer);
    }
    for(int i=1; i<=SIZE; i++)
    {
        gotoxy(BOARD_X+2*SIZE, BOARD_Y+i);
        char buffer[110];
        sprintf(buffer, "%d", i);
        cputs(buffer);
    }
}

void newGame()
{
    points=0;
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            board[i][j]=' ';
        }
    }
    for(int i=0; i<strlen(letters1); i++){
        letters[i]=letters1[i];
    }
    for(int i=0; i<7; i++){
        my_letters[i]=0;
    }
    losulosu();


}
void placeLetter(char brd[][SIZE])
{

    int h=0, i=0, m=0;
    gotoxy(LEGENDPX, LEGENDPY+9);
    cputs("What word would you like to enter?");
    while(h!=ENTER && i<SIZE){
        textbackground(BLACK);
        print_my_letters(my_letters);
        gotoxy(i+LEGENDPX, LEGENDPY+10);
        h=getch();
        if(h==ESCAPE){// if player enters escape, escape
            returnpomocna();
            resetPlacingTable();
            break;
        }
        h=convertUpper(h);//convert uppercase to lowercase
        if(asciiCheck(h)){ //checks if entered character is a letter or backspace or blank tile
            if(h==0x08){//bckspace
                if(i>0){
                    funkcjazwrotna();//zwraca ostatnio wprowadzona litere
                    gotoxy(LEGENDPX-1+i,11);
                    textbackground(BLACK);
                    cputs(" ");
                    placingTable[i-1]='\0';
                    i--;
                }
            }
            else if(h=='@'){
                for(int j=0; j<7; j++){
                    if(my_letters[j]=='@'){
                        h=getch();
                        if(asciiCheck(h)){
                            tabhelp[pomocna]=my_letters[j];
                            pomocna++;
                            my_letters[j]='.';
                            placingTable[i]=h;
                            textbackground(GREEN);
                            textcolor(WHITE);
                            putch(placingTable[i]);
                            i++;break;
                        }
                    }
                }
            }
            else{
                placingTable[i]=h;
                gotoxy(LEGENDPX+i, LEGENDPY+10);
                if(checkIfLetter(h, my_letters)){ //if player has that letter, print green background, else red background
                    //adds that letter to tabhelp
                    textbackground(GREEN);
                    textcolor(WHITE);
                    putch(placingTable[i]);
                    i++;
                }
                else{
                    textbackground(RED);
                    textcolor(WHITE);
                    putch(placingTable[i]);
                    i++;
                    difference[m]=h;
                    m++;
                }
            }
        }
    }
}

void positionOnBoard()
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int positiony=tcory+BOARD_Y;
    char positionx=tcorx+65;
    textbackground(WHITE);
    textcolor(BLACK);
    gotoxy(corx-4, cory);
    char buffer[10];
    sprintf(buffer, "%c %d", positionx, positiony); //prints position of the cursor
    cputs(buffer);
}
void testing1()
{
    textcolor(WHITE);
    int length = strlen(placingTable);
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<length; i++)
    {
        if(board[tcorx][tcory+i]==placingTable[i]||board[tcorx][tcory+i]==' ')
        {
            textbackground(GREEN);
            gotoxy(corx, cory+i);
            putch(placingTable[i]);
        }
        else
        {
            textbackground(RED);
            gotoxy(corx, cory+i);
            putch(placingTable[i]);
        }

    }
}
void funkcjazwrotna()//w funkcji placeletter, jesli gracz kliknie backspace, zwraca ostanio wpisana litere, jesli byla w puli
{
    int dl=strlen(placingTable);
    for(int i=0; i<7; i++){
        if(my_letters[i]=='.'&&placingTable[dl-1]==tabhelp[pomocna-1]){
            my_letters[i]=tabhelp[pomocna-1];
            tabhelp[pomocna-1]='\0';
            if(pomocna>0){
                pomocna--;
            }
            break;
        }if(my_letters[i]=='.'&&tabhelp[pomocna-1]=='@'){
            my_letters[i]=tabhelp[pomocna-1];
            tabhelp[pomocna-1]='\0';
            if(pomocna>0){
                pomocna--;
            }
         }
    }
}

void checkXY(int fx, int fy)
{
    int length=strlen(placingTable);
    if(strlen(placingTable)==0){
        if(fx<BOARD_X-1||fy<BOARD_Y+1 || fy>SIZE+BOARD_Y||fx>BOARD_X+2*SIZE-2){
            x=corx-1;
            y=cory;
        }
    }
    else{
        if(fx<BOARD_X-1||fy<BOARD_Y+1){
            x=corx-1;
            y=cory;
        }
        else if(optiono==0){
            if(fy+length-1>SIZE+BOARD_Y)
                y=SIZE-length+2;
            else if(fx>BOARD_X+2*SIZE-2)
                x=corx-1;
        }
        else if(optiono==1){
            if(fy>SIZE+BOARD_Y)
                y=cory;
            else if(fx+2*length-1>BOARD_X+2*SIZE-2)
                x=BOARD_X+2*SIZE-2*length-1;
        }
    }
}
void print_my_letters(char mltrs[])
{
    char txt[7];
    gotoxy(LEGENDPX, LEGENDPY+7);
    cputs("My letters:\n");
    for(int i=0; i<7; i++){
        gotoxy(LEGENDPX+2*i, LEGENDPY+8);
        putch(mltrs[i]);
    }
}

bool checkIfLetter(int z, char mltrs[])
{
    bool status=false;
    int i=0;
    do{
        if(z==mltrs[i]){
            status=true;
            tabhelp[pomocna]=mltrs[i];
            pomocna++;
            mltrs[i]='.';
            break;
        }
        else{

            status=false;
        }
        i++;
    }
    while(i<7);
    return status;
}


bool asciiCheck(int z)//checks if entered character is a letter or backspace
{
    bool status=false;
    if(z>='a'&&z<='z'||z==0x08||z=='@'){
        status=true;
    }
    return status;
}

int convertUpper(int z)
{
    if(z>='A'&& z<='Z'){
        z=z+32;
    }
    return z;
}

void resetPlacingTable()
{
    pomocna=0;
    for(int i=0; i<SIZE; i++){
        placingTable[i]=0;
        tabhelp[i]=0;
    }
    //placingTable[0]='\0';
}

void placeWord1()
{

    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        board[tcorx][tcory+i]=placingTable[i];
    }
}
void placeWord2()
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        board[tcorx+i][tcory]=placingTable[i];
    }
}
void testing2()
{
    int length=strlen(placingTable);
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    //for(int i=0; i<4;i)
    gotoxy(1, 15);
    for(int i=0; i<length; i++){
        if(placingTable[i]==board[tcorx+i][tcory]){
            gotoxy(corx+2*i, cory);
            textbackground(GREEN);
            textcolor(WHITE);
            putch(placingTable[i]);

        }else if(board[tcorx+i][tcory]==' '){
        gotoxy(corx+2*i, cory);
            textbackground(GREEN);
            textcolor(WHITE);
            putch(placingTable[i]);}
        else if(placingTable[i]!=board[tcorx+i][tcory]){
            gotoxy(corx+2*i, cory);
            textbackground(RED);
            textcolor(WHITE);
            putch(placingTable[i]);
        }
    }
}

void losulosu()//letters and my letters
{
    srand(time(NULL));
    int dl=strlen(letters);
    int i=0;
    while(i<7){
        int los=rand() % dl;
        if(letters[los]!='.'){
            my_letters[i]=letters[los];
            letters[los]='.';
            i++;
        }
    }
}

void losulosu2()
{
    srand(time(NULL));
    int dl=strlen(letters);
    for(int i=0; i<7; i++){
        if(my_letters[i]=='.'){
            int los=rand() % dl;
            if(letters[los]!='.'){
                my_letters[i]=letters[los];
                letters[los]='.';
            }
            else if(letters[los]=='.'){
                i--;
            }
        }
    }
}
void replaceKafelki()//////!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
    int h=0;
    gotoxy(LEGENDPX, LEGENDPY+7);
    cputs("Choose letters to replace:");
    do{
        h=getch();
        if(h>='1' && h<='7' && replacetab[h-49]!=my_letters[h-49]){//if entered character is between 1 and 7 and
            //replacetab doesnt contain given letter
            gotoxy(LEGENDPX+(h-49)*2, LEGENDPY+8);
            textbackground(13);
            putch(my_letters[h-49]);
            replacetab[h-49]=my_letters[h-49];
        }
        else if(h>='1' && h<='7'&& replacetab[h-49]==my_letters[h-49]){
            gotoxy(LEGENDPX+(h-49)*2, LEGENDPY+8);
            textbackground(0);
            putch(my_letters[h-49]);
            replacetab[h-49]=0;
        }
        else if(h==13){
            funkcja();
            break;
        }
        else if(h==ESCAPE){
            for(int i=0; i<7; i++){
                replacetab[i]=0;
            }
            break;
        }
    }
    while(h!='w');
    textbackground(BLACK);
}
void funkcja()
{
    for(int i=0; i<7; i++){
        if(replacetab[i]!=0){
            my_letters[i]='.';
            for(int j=0; j<strlen(letters); j++){
                if(letters[j]=='.'){
                    letters[j]=replacetab[i];
                    replacetab[i]=0;
                    j=strlen(letters);
                }
            }
        }
    }
    losulosu2();
}

void returnpomocna()
{
    pomocna=0;
    int j=0;
    for(int i=0; i<7; i++){
        if(my_letters[i]=='.'){
            my_letters[i]=tabhelp[j];
            j++;
        }
    }
}

void legenda()
{
    char txt[32];
    gotoxy(LEGENDPX, LEGENDPY);
    cputs("Adam Kasperski, s175956:a)b)c)d)e)f)g)h)");
    gotoxy(LEGENDPX, LEGENDPY+1);
    cputs("q=quit");
    gotoxy(LEGENDPX, LEGENDPY+2);
    cputs("i=enter a word");
    gotoxy(LEGENDPX, LEGENDPY+3);
    cputs("w=replace tiles");
    gotoxy(LEGENDPX, LEGENDPY+4);
    cputs("o=change word orientation");
    gotoxy(LEGENDPX, LEGENDPY+5);
    cputs("n=new game");
    gotoxy(LEGENDPX, LEGENDPY+6);
    cputs("esc=cancel current action");

    gotoxy(BOARD_X+2*SIZE+5, BOARD_Y);
    textbackground(CYAN);cputs(" ");textbackground(BLACK);cputs("=letter x2");
    gotoxy(BOARD_X+2*SIZE+5, BOARD_Y+1);
    textbackground(LIGHTBLUE);cputs(" ");textbackground(BLACK);cputs("=letter x3");
    gotoxy(BOARD_X+2*SIZE+5, BOARD_Y+2);
    textbackground(LIGHTMAGENTA);cputs(" ");textbackground(BLACK);cputs("=word x2");
    gotoxy(BOARD_X+2*SIZE+5, BOARD_Y+3);
    textbackground(RED);cputs(" ");textbackground(BLACK);cputs("=word x3");
    sprintf(txt, "Points:%d", points);
    gotoxy(BOARD_X+2*SIZE+5, BOARD_Y+4);
    cputs(txt);
}

void returnkafelek1()
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(tabhelp); i++){
        if(tabhelp[i]==board[tcorx][tcory+i]){
            for(int j=0; j<7; j++){
                if(my_letters[j]=='.'){
                    my_letters[j]=tabhelp[i];
                    j=7;
                }
            }
        }
    }
}
void returnkafelek2()
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(tabhelp); i++){
        if(tabhelp[i]==board[tcorx+i][tcory]){
            for(int j=0; j<7; j++){
                if(my_letters[j]=='.'){
                    my_letters[j]=tabhelp[i];
                    j=7;
                }
            }
        }
    }
}

bool hejka1()
{
    bool status;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(placingTable[i]==board[tcorx][tcory+i]||board[tcorx][tcory+i]==' '){
            status = true;
        }
        else{
            status=false;
            break;
        }
    }
    return status;
}
bool hejka12()
{
    bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(placingTable[i]==board[tcorx][tcory+i]){
            status = true;
            break;
        }
    }
    return status;

}
bool hejka22()
{
    bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(placingTable[i]==board[tcorx+i][tcory]){
            status = true;
            break;
        }
    }
    return status;

}
bool hejka2()
{
    bool status;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(placingTable[i]==board[tcorx+i][tcory]||board[tcorx+i][tcory]==' '){
            status = true;
        }
        else{
            status=false;
            break;
        }
    }
    return status;
}
bool checking1()//checks if at least one entered letter is in players pula
{
    bool status=false;
    for(int i=0; i<strlen(placingTable); i++){
        for(int z=0; z<strlen(tabhelp); z++)
            if(placingTable[i]==tabhelp[z]){
                status= true;
            }
    }
    return status;
}

int simplePoints (char pt[], int length)
{
    char ltr[]={"eaionrtlsudgbcmpfhvwykjxqz"};
    for(int k=0; k<length; k++){
      for(int i=0; i<27; i++){
        if(i<10 && pt[k]==ltr[i]){
          punkty+=1;
          i=27;
        }
        else if(i>=10 && i<12 && pt[k]==ltr[i]){
          punkty+=2;
          i=27;
        }
        else if(i>=12 && i<16 && pt[k]==ltr[i]){
          punkty+=3;
          i=27;
        }
        else if(i>=16 && i<21 && pt[k]==ltr[i]){
          punkty+=4;
          i=27;
        }
        else if(i==21 && pt[k]==ltr[i]){
          punkty+=5;
          i=27;
        }
        else if(i>21 && i<24 && pt[k]==ltr[i]){
          punkty+=8;
          i=27;
        }
        else if(i>=24 && i<26 && pt[k]==ltr[i]){
          punkty+=10;
          i=27;
        }
      }
}

    return punkty;
}
void verticalLetterMultiplier(int k)
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{0,3},{0,11},{2,6},{2,8},{3,0},{3,7},{3,14},{6,6}, {6, 8}, {8, 6}, {8, 8},
        {6, 2}, {6, 12}, {7, 3}, {7, 11}, {8,2}, {8, 12}, {11, 0}, {11, 7}, {11, 14},
        {12, 6}, {12, 8}, {14, 3}, {14, 11}
    };
    int m=0;
    char z[4]= {};
    for(int i=0; i<24; i++){
        for(int j=0; j<2; j++){
            if(tcorx==pozycje[i][j]&&tcory+k==pozycje[i][j+1]){
                z[m]=placingTable[k];m++;
                for(int i=1; i<LETTERMP2; i++){
                    simplePoints(z, strlen(z));
                }
            }break;
        }
    }
}
void horizontalLetterMultiplier(int k)
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{0,3},{0,11},{2,6},{2,8},{3,0},{3,7},{3,14},{6,6}, {6, 8}, {8, 6}, {8, 8},
        {6, 2}, {6, 12}, {7, 3}, {7, 11}, {8,2}, {8, 12}, {11, 0}, {11, 7}, {11, 14},
        {12, 6}, {12, 8}, {14, 3}, {14, 11}
    };
    int m=0;
    char z[4]= {};
    for(int i=0; i<24; i++){
        for(int j=0; j<2; j++){
            if(tcorx+k==pozycje[i][j]&&tcory==pozycje[i][j+1]){
                z[m]=placingTable[k];m++;
                for(int i=1; i<LETTERMP2; i++){
                    simplePoints(z, strlen(z));
                }
            }break;
        }
    }
}
void verticalLetterMultiplier3(int k)
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{5,5},{9,5},{5,9},{9,9}, {5, 1}, {9, 1}, {1, 5}, {13, 5}, {1, 9}, {13, 9}, {5, 13}, {9,13}};
    int m=0;
    char z[4]={};
    for(int i=0; i<12; i++){
        for(int j=0; j<2; j++){
            if(tcorx==pozycje[i][j]&&tcory+k==pozycje[i][j+1]){
                z[m]=placingTable[k];m++;
                for(int i=1; i<LETTERMP3; i++){
                    simplePoints(z, strlen(z));
                }
            }break;
        }
    }
}
void horizontalLetterMultiplier3(int k)
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{5,5},{9,5},{5,9},{9,9}, {5, 1}, {9, 1}, {1, 5}, {13, 5}, {1, 9}, {13, 9}, {5, 13}, {9,13}};
    int m=0;
    char z[4]={};
    for(int i=0; i<12; i++){
        for(int j=0; j<2; j++){
            if(tcorx+k==pozycje[i][j]&&tcory==pozycje[i][j+1]){
                z[m]=placingTable[k];m++;
                for(int i=1; i<LETTERMP3; i++){
                    simplePoints(z, strlen(z));
                }
            }break;
        }
    }
}
bool verticalWord2(int k)
{
    bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{1, 1},{2, 2},{3, 3},{4, 4},{10,4},{11, 3},{12, 2},{13, 1},
                        {1, 13}, {2, 12}, {3, 11}, {4, 10}, {10, 10}, {11, 11}, {12, 12}, {13, 13}};
    int j=0;
    for(int i=0; i<16; i++){
        if(tcorx==pozycje[i][j]&&tcory+k==pozycje[i][j+1]){
            status=true;
            break;
        }
    }
    return status;
}
bool horizontalWord2(int k)
{
    bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{1, 1},{2, 2},{3, 3},{4, 4},{10,4},{11, 3},{12, 2},{13, 1},
                        {1, 13}, {2, 12}, {3, 11}, {4, 10}, {10, 10}, {11, 11}, {12, 12}, {13, 13}};
    int j=0;
    for(int i=0; i<16; i++){
        if(tcorx+k==pozycje[i][j]&&tcory==pozycje[i][j+1]){
            status=true;
            break;
        }
    }
    return status;
}
bool verticalWord3(int k)
{
   bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{0, 0},{7, 0},{14, 0},{0, 7},{14,7},{0, 14},{7, 14},{14, 14}};
    int j=0;
    for(int i=0; i<8; i++){
        if(tcorx==pozycje[i][j]&&tcory+k==pozycje[i][j+1]){
            status=true;
            break;
        }
    }
    return status;
}
bool horizontalWord3(int k)
{
   bool status=false;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int pozycje[][2]= {{0, 0},{7, 0},{14, 0},{0, 7},{14,7},{0, 14},{7, 14},{14, 14}};
    int j=0;
    for(int i=0; i<8; i++){
        if(tcorx+k==pozycje[i][j]&&tcory==pozycje[i][j+1]){
            status=true;
            break;
        }
    }
    return status;
}
bool fiftyPoints()
{
    bool status;
    for(int i=0; i<7; i++){
        if(my_letters[i]=='.'){
            status=true;
        }else{
            status=false;
            break;
            }
    }
    return status;
}
bool checks1()
{
    bool status;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(board[tcorx][tcory+i]==placingTable[i]){
            status= false;
        }else if(board[tcorx][tcory+i]==' '){
            status= true;
            break;}
    }
    return status;
}
bool checks2()//check for overlaping words
{
    bool status;
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    for(int i=0; i<strlen(placingTable); i++){
        if(board[tcorx+i][tcory]==placingTable[i]){
            status= false;
        }else if(board[tcorx+i][tcory]==' '){
            status=true;
            break;}
    }
    return status;
}

bool checkDifference1()
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int m=0;
    bool status=true;
    if(strlen(difference)!=0){
    while(m<strlen(difference)){
        for(int i=0; i<SIZE; i++){
            if(difference[m]!=board[tcorx][tcory+i]){
                status=false;
            }else if(difference[m]==board[tcorx][tcory+i]){
                status=true;
                break;
                }
        }m++;
    }
    }
    return status;
}
bool checkDifference2()
{
    int tcorx=(corx-BOARD_X)-(corx-BOARD_X)/2;
    int tcory=cory-BOARD_Y-1;
    int m=0;
    bool status=true;
    if(strlen(difference)!=0){
    while(m<strlen(difference)){
        for(int i=0; i<SIZE; i++){
            if(difference[m]!=board[tcorx+i][tcory]){
                status=false;
            }else if(difference[m]==board[tcorx+i][tcory]){
                status=true;
                break;
                }
        }m++;
    }
    }
    return status;
}
bool checkLettersOnBoard(int m)
{
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if(difference[m]==board[i][j]){
                return true;
                break;
            }else{
                return false;}
        }
    }
}
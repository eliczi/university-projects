#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include"./sdl-2.0.7/include/SDL.h"
#include "SDL_ttf.h"

#define SIZE 4

bool gameOver();
void back(int boarding[][SIZE], int copy[][SIZE]);
void newgame(int board[][SIZE]);
void moveNumbersRight(int boarding[][SIZE]);
void moveNumbersLeft(int boarding[][SIZE]);
void moveNumbersUp(int boarding[][SIZE]);
void moveNumbersDown(int boarding[][SIZE], SDL_Renderer *renderer, SDL_Texture *texture2);
void random(int boarding[][SIZE]);
void boardingInit(int boarding[][SIZE]);
void printboard(SDL_Texture *texture2, SDL_Texture *texture4, SDL_Texture *texture8, SDL_Texture *texture16, SDL_Texture *texture32,
                                SDL_Texture *texture64,SDL_Texture *texture128, SDL_Texture *texture256,SDL_Texture *texture512, SDL_Texture *texture1024,
                                SDL_Texture *texture2048,int boarding[][SIZE], SDL_Renderer *renderer, SDL_Rect board[][SIZE], SDL_Rect boardf[][SIZE]);
void fillBoard(SDL_Rect brd[][SIZE]);
void copyBoard(int boarding[][SIZE], int copy[][SIZE]);
bool checkMoveDown(int boarding[][SIZE], int copy[][SIZE]);
bool checkMoveUp(int boarding[][SIZE]);
bool checkMoveLeft(int boarding[][SIZE]);
bool checkMoveRight(int boarding[][SIZE]);

void copyBoard(int boarding[][SIZE], int copy[][SIZE])
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        copy[i][j] = boarding[i][j];
                        
                }
        }
}
int score = 0;
int oneMoveScore = 0;
int boarding[SIZE][SIZE];

int main(int argc, char **argv) {
        int t1, t2;
        double  worldTime = 0, delta;
        SDL_Init(SDL_INIT_VIDEO);
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                printf("SDL_Init error: %s\n", SDL_GetError());
                return 1;
        }
        TTF_Init();
        SDL_Window *window = SDL_CreateWindow("2048", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 650, 440, SDL_WINDOW_SHOWN);
        
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        
        int copy[SIZE][SIZE];//console board
        int boardbc[SIZE][SIZE];
        boardingInit(boardbc);
        boardingInit(boarding);//initialize array to 0's
        
        SDL_Rect board[SIZE][SIZE];
        SDL_Rect boardf[SIZE][SIZE];
        SDL_Rect boardb[SIZE][SIZE];
        fillBoard(board);//board to display numbers
        fillBoard(boardf);//board for animation
        fillBoard(boardb);

        SDL_Surface *two = SDL_LoadBMP("./two.bmp");//initializing surfaces
        SDL_Surface *four = SDL_LoadBMP("./four.bmp");
        SDL_Surface *eight = SDL_LoadBMP("./eight.bmp");
        SDL_Surface *sixteen = SDL_LoadBMP("./sixteen.bmp");
        SDL_Surface *thirtytwo = SDL_LoadBMP("./thirtytwo.bmp");
        SDL_Surface *sixtyfour = SDL_LoadBMP("./sixtyfour.bmp");
        SDL_Surface *one28 = SDL_LoadBMP("./128.bmp");
        SDL_Surface *two56 = SDL_LoadBMP("./256.bmp");
        SDL_Surface *five12 = SDL_LoadBMP("./512.bmp");
        SDL_Surface *one024 = SDL_LoadBMP("./1024.bmp");
        SDL_Surface *two48 = SDL_LoadBMP("./2048.bmp");

        SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, two);//creating texture from bmps
        SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, four);
        SDL_Texture *texture8 = SDL_CreateTextureFromSurface(renderer, eight);
        SDL_Texture *texture16 = SDL_CreateTextureFromSurface(renderer, sixteen);
        SDL_Texture *texture32 = SDL_CreateTextureFromSurface(renderer, thirtytwo);
        SDL_Texture *texture64 = SDL_CreateTextureFromSurface(renderer, sixtyfour);
        SDL_Texture *texture128 = SDL_CreateTextureFromSurface(renderer, one28);
        SDL_Texture *texture256 = SDL_CreateTextureFromSurface(renderer, two56);
        SDL_Texture *texture512 = SDL_CreateTextureFromSurface(renderer, five12);
        SDL_Texture *texture1024 = SDL_CreateTextureFromSurface(renderer, one024);
        SDL_Texture *texture2048 = SDL_CreateTextureFromSurface(renderer, two48);

        SDL_SetRenderDrawColor(renderer, 205, 192, 180, 255); // filling tiles color to light brown?
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        SDL_RenderFillRect(renderer, &board[i][j]);
                        SDL_RenderFillRect(renderer, &boardf[i][j]);
                        SDL_RenderFillRect(renderer, &boardb[i][j]);
                }
        }

        char buffer[10];
        char scoreBuffer[40];

        TTF_Font *sans = TTF_OpenFont("OpenSans-Bold.ttf", 20); //load font and set its size
        TTF_Font *sans2 = TTF_OpenFont("OpenSans-Bold.ttf", 15);
        SDL_Color white = { 255, 255, 255 };//set color
        SDL_Surface *timeMessage = NULL;//time
        SDL_Surface *scoreMessage = NULL;//score
        SDL_Surface *gameOverSurface = NULL;
        SDL_Surface *headerSurface = NULL;
        
        SDL_Texture *scoreShow = SDL_CreateTextureFromSurface(renderer, scoreMessage);
        SDL_Texture *timeShow = SDL_CreateTextureFromSurface(renderer, timeMessage);
        SDL_Texture *headerTexture = NULL;
        
        headerSurface = TTF_RenderText_Solid(sans2, "Adam Kasperski, 175945", white);
        headerTexture = SDL_CreateTextureFromSurface(renderer, headerSurface);
        SDL_Rect timeRect = { 470, 50, 50,50}; //x, y, width, height of rect where time is
        SDL_Rect gameRect = { 440/2, 440/2};
        SDL_Rect scoreRect;//'-'score is
        scoreRect.x = 470;
        scoreRect.y = 100;
        SDL_Rect headerRect = { 460, 0 };
        
        bool isRunning = true;
        SDL_Event event;
        
        t1 = SDL_GetTicks();

        SDL_Rect boardBackground = { 0, 0, 440, 440 };
        char napis[] = "Score:";
        random(boarding);
        while (isRunning) {
                
                t2 = SDL_GetTicks();
                delta = (t2 - t1) * 0.001;
                t1 = t2;

                worldTime += delta;
                
                SDL_SetRenderDrawColor(renderer, 205, 192, 180, 255);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
                SDL_RenderFillRect(renderer, &boardBackground);
                SDL_RenderFillRect(renderer, &scoreRect);

                printboard(texture2, texture4, texture8, texture16, texture32, texture64, texture128, texture256, texture512, texture1024, texture2048,
                boarding, renderer, board, boardf);
                
                sprintf(scoreBuffer, "%s%d", napis, score);
                scoreMessage = TTF_RenderText_Solid(sans, scoreBuffer, white);
                scoreShow = SDL_CreateTextureFromSurface(renderer, scoreMessage);
                SDL_QueryTexture(scoreShow, NULL, NULL, &scoreRect.w, &scoreRect.h);
                SDL_RenderCopy(renderer, scoreShow, NULL, &scoreRect);
                
                sprintf_s(buffer, "%0.1lf", worldTime);
                timeMessage = TTF_RenderText_Solid(sans, buffer, white);
                timeShow = SDL_CreateTextureFromSurface(renderer, timeMessage);
                SDL_QueryTexture(timeShow, NULL, NULL, &timeRect.w, &timeRect.h);
                SDL_RenderCopy(renderer, timeShow, NULL, &timeRect);
                
                
                SDL_QueryTexture(headerTexture, NULL, NULL, &headerRect.w, &headerRect.h);
                SDL_RenderCopy(renderer, headerTexture, NULL, &headerRect);

                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(timeShow);
                SDL_DestroyTexture(scoreShow);
                SDL_Delay(50);
                if (gameOver())
                {
                        isRunning = false;
                        gameOverSurface = TTF_RenderText_Solid(sans, "GAME OVER", white);
                        SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
                        SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameRect.w, &gameRect.h);
                        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameRect);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(2000);
                }
                while (SDL_PollEvent(&event) != 0)
                        switch (event.type)
                        {
                        case SDL_QUIT:
                                isRunning = false;
                                break;
                        case SDL_KEYDOWN:
                                switch (event.key.keysym.sym)
                                {
                                case SDLK_ESCAPE:
                                        isRunning = false;
                                        break;
                                case SDLK_DOWN:
                                        copyBoard(boarding, copy);
                                        if (checkMoveDown(boarding, copy) == true)
                                        {
                                                moveNumbersDown(boarding, renderer, texture2);
                                                random(boarding);
                                        }

                                        break;
                                case SDLK_UP:
                                        copyBoard(boarding, copy);
                                        if (checkMoveUp(boarding))
                                        {
                                                moveNumbersUp(boarding);
                                                random(boarding);
                                        }
                                        break;

                                case SDLK_LEFT:
                                        copyBoard(boarding, copy);
                                        if (checkMoveLeft(boarding))
                                        {
                                                moveNumbersLeft(boarding);
                                                random(boarding);
                                        }
                                        break;
                                case SDLK_RIGHT:
                                        copyBoard(boarding, copy);
                                        if(checkMoveRight(boarding))
                                        { 
                                                moveNumbersRight(boarding);
                                                random(boarding);
                                        }
                                        break;
                                case SDLK_n:
                                        worldTime = 0;
                                        score = 0;
                                        newgame(boarding);
                                        random(boarding);
                                        break;
                                case SDLK_u:
                                        back(boarding, copy);
                                        score -= oneMoveScore;
                                        oneMoveScore = 0;
                                }
                        }
        }
        
        SDL_DestroyTexture(scoreShow);
        SDL_DestroyTexture(timeShow);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
bool checkMoveDown(int boardin[][SIZE], int copy[][SIZE])
{
        bool status=false;
        
        for (int i = 0; i < SIZE-1; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        if (boarding[j][i] != 0 && boarding[j][i+1] == 0)
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j][i + 1] == boarding[j][i])
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j][i + 2] == boarding[j][i] && (boarding[j][i + 1] == boarding[j][i]|| boarding[j][i + 1]==0))
                        {
                                status = true;
                        }
                }
        }
        return status;
}

bool checkMoveUp(int boarding[][SIZE])
{
        bool status = false;

        for (int i = SIZE-1; i >0; i--)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        if (boarding[j][i] != 0 && boarding[j][i -1] == 0)
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j][i -1] == boarding[j][i])
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j][i -2] == boarding[j][i] && (boarding[j][i -1] == boarding[j][i] || boarding[j][i-1] == 0))
                        {
                                status = true;
                        }
                }
        }
        return status;
}
bool checkMoveLeft(int boarding[][SIZE])
{
        bool status = false;
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = SIZE-1; j > 0; j--)
                {
                        if (boarding[j][i] != 0 && boarding[j-1][i] == 0)
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j-1][i] == boarding[j][i])
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j-2][i] == boarding[j][i] && (boarding[j-1][i] == boarding[j][i] || boarding[j-1][i] == 0))
                        {
                                status = true;
                        }
                }
        }
return status;
}
bool checkMoveRight(int boarding[][SIZE])
{
        bool status = false;
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE-1; j++)
                {
                        if (boarding[j][i] != 0 && boarding[j + 1][i] == 0)
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j + 1][i] == boarding[j][i])
                        {
                                status = true;
                        }
                        if (boarding[j][i] != 0 && boarding[j + 2][i] == boarding[j][i] && (boarding[j + 1][i] == boarding[j][i] || boarding[j + 1][i] == 0))
                        {
                                status = true;
                        }
                }
        }
        return status;
}
void back(int boarding[][SIZE], int copy[][SIZE])
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        boarding[i][j] = copy[i][j];
                }
        }
}
void newgame(int board[][SIZE])
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        boarding[i][j] = 0;
                }
        }
}
void printboard(SDL_Texture *texture2, SDL_Texture *texture4, SDL_Texture *texture8, SDL_Texture *texture16, SDL_Texture *texture32,
                                SDL_Texture *texture64,SDL_Texture *texture128, SDL_Texture *texture256, SDL_Texture *texture512, SDL_Texture *texture1024,
                                SDL_Texture *texture2048, int boarding[][SIZE], SDL_Renderer *renderer, SDL_Rect board[][SIZE], SDL_Rect boardf[][SIZE])
{
        SDL_SetRenderDrawColor(renderer, 205, 192, 180, 255);
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        if (boarding[i][j] == 2)
                        {
                                SDL_RenderCopy(renderer, texture2, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture2, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 4)
                        {
                                SDL_RenderCopy(renderer, texture4, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture4, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 8)
                        {
                                SDL_RenderCopy(renderer, texture8, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture8, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 16)
                        {
                                SDL_RenderCopy(renderer, texture16, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture16, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 32)
                        {
                                SDL_RenderCopy(renderer, texture32, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture32, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 64)
                        {
                                SDL_RenderCopy(renderer, texture64, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture64, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 128)
                        {
                                SDL_RenderCopy(renderer, texture128, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture128, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 256)
                        {
                                SDL_RenderCopy(renderer, texture256, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture256, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 512)
                        {
                                SDL_RenderCopy(renderer, texture512, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture512, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 1024)
                        {
                                SDL_RenderCopy(renderer, texture1024, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture1024, NULL, &boardf[i][j]);
                        }
                        if (boarding[i][j] == 2048)
                        {
                                SDL_RenderCopy(renderer, texture2048, NULL, &board[i][j]);
                                SDL_RenderCopy(renderer, texture2048, NULL, &boardf[i][j]);

                        }
                        if (boarding[i][j] == 0)
                        {
                                SDL_RenderFillRect(renderer, &board[i][j]);
                                SDL_RenderFillRect(renderer, &boardf[i][j]);
                        }
                }
        }
}

void boardingInit(int boarding[][SIZE])
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        boarding[i][j] = 0;
                }
        }
}
void random(int boarding[][SIZE])
{
        srand(time(NULL));
        int g, h;
        bool quit = false;
        while (!quit)
        {
                g = rand() % SIZE;
                h = rand() % SIZE;
                if (boarding[g][h] == 0)
                {
                        int z = rand() % 2;
                        if (z == 0)
                        {
                                boarding[g][h] = 2;
                        }
                        else {
                                boarding[g][h] = 4;
                        }
                        quit = true;
                }
        }
}


void moveNumbersDown(int boarding[][SIZE], SDL_Renderer *renderer, SDL_Texture *texture2)
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = SIZE - 1; j >= 0; j--)
                {
                        int m = j - 1;
                        while(m>=0)
                        {
                                if(boarding[i][m] == 0)
                                {
                                        m--;
                                        continue;
                                }
                                else if(boarding[i][m] == boarding[i][j])
                                {
                                        boarding[i][j] *= 2;
                                        boarding[i][m] = 0;
                                        oneMoveScore = boarding[i][j];
                                        score += boarding[i][j];
                                        m--;
                                        break;
                                }
                                else if(boarding[i][j] == 0 && boarding[i][m] != 0)
                                {
                                        boarding[i][j] = boarding[i][m];
                                        boarding[i][m] = 0;
                                        j++;
                                        m--;
                                        break;
                                }
                                else if(boarding[i][j] != 0)
                                {
                                        m--;
                                        break;
                                }
                                
                        }
                }
        }
}

void moveNumbersUp(int boarding[][SIZE])
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        int m = j + 1;
                        while(m<SIZE)
                        {
                                if (boarding[i][m] == 0)
                                {
                                        m++;
                                        continue;
                                }
                                else if (boarding[i][m] == boarding[i][j])
                                {
                                        boarding[i][j] *= 2;
                                        boarding[i][m] = 0;
                                        score += boarding[i][j];
                                        oneMoveScore = boarding[i][j];
                                        m++;
                                        break;
                                }
                                else if (boarding[i][j] == 0 && boarding[i][m] != 0)
                                {
                                        boarding[i][j] = boarding[i][m];
                                        boarding[i][m] = 0;
                                        m++;
                                        j--;
                                        break;
                                }
                                else if (boarding[i][j] != 0)
                                {
                                        m++;
                                        break;
                                }
                                
                        }
                }
        }
}

void moveNumbersRight(int boarding[][SIZE])
{
        for (int j = 0; j < SIZE; j++)
        {
                for (int i = SIZE-1; i >= 0; i--)
                {
                        int m = i - 1;
                        while(m>=0)
                        {
                                if (boarding[m][j] == 0)
                                {
                                        m--;
                                        continue;
                                }
                                else if (boarding[m][j] == boarding[i][j])
                                {
                                        boarding[i][j] *= 2;
                                        boarding[m][j] = 0;
                                        oneMoveScore = boarding[i][j];
                                        score += boarding[i][j];
                                        m--;
                                        break;
                                }
                                else if (boarding[i][j] == 0 && boarding[m][j] != 0)
                                {
                                        boarding[i][j] = boarding[m][j];
                                        boarding[m][j] = 0;
                                        i++;
                                        m--;
                                        break;
                                }
                                else if (boarding[i][j] != 0)
                                {
                                        m--;
                                        break;
                                }
                                
                        }
                }
        }
}
void moveNumbersLeft(int boarding[][SIZE])
{
        for (int j = 0; j < SIZE; j++)
        {
                for (int i = 0; i < SIZE; i++)
                {
                        int m = i + 1;
                        while(m<SIZE)
                        {
                                if (boarding[m][j] == 0)
                                {
                                        m++;
                                        continue;
                                }
                                else if (boarding[m][j] == boarding[i][j])
                                {
                                        boarding[i][j] *= 2;
                                        boarding[m][j] = 0;
                                        oneMoveScore = boarding[i][j];
                                        score += boarding[i][j];
                                        m++;
                                        break;
                                }
                                else if (boarding[i][j] == 0 && boarding[m][j] != 0)
                                {
                                        boarding[i][j] = boarding[m][j];
                                        boarding[m][j] = 0;
                                        i--;
                                        m--;
                                        break;
                                }
                                else if (boarding[i][j] != 0)
                                {
                                        m--;
                                        break;
                                }
                                
                        }
                }
        }
}

void fillBoard(SDL_Rect brd[][SIZE])
{
        for (int i = 0; i < SIZE; i++)
        {
                for (int j = 0; j < SIZE; j++)
                {
                        brd[i][j].w = 100;
                        brd[i][j].h = 100;
                }
        }
        for (int i = 0, k = 0; i < SIZE; i++, k += 110)
        {
                for (int j = 0, m = 0; j < SIZE; j++, m += 110)
                {
                        brd[i][j].y = m;
                        brd[i][j].x = k;
                }
        }
}

bool gameOver()
{
        bool status = true;
        for (int i = 0; i < 4; i++)
        {
                for (int j = 0; j < 4; j++)
                {
                        if (boarding[i][j] == 0)
                        {
                                status = false;
                        }
                        if (i+1 < SIZE&&boarding[i+1][j] == boarding[i][j])
                        {
                                status = false;
                        }       
                        if (i-1 >= 0 && boarding[i-1][j] == boarding[i][j])
                        {
                                status = false;
                        }
                        if (j+1 < SIZE&&boarding[i][j+1] == boarding[i][j])
                        {
                                status = false;
                        }
                        if (j-1 >= 0 && boarding[i][j-1] == boarding[i][j])
                        {
                                status=false;
                        }
                }
        }

        return status;
}
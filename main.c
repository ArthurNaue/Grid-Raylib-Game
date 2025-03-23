#include <stdio.h>
#include <stdlib.h>
#include "include/raylib.h"

#define iMapSize 24
#define iGridSize 16

//GAME SCREENS
typedef enum eGameScreen {LOGO = 0, TITLE, GAMEPLAY, GAMEOVER} eGameScreen;

//GAME MAP
int aMap[iMapSize][iMapSize] ={0};

//VECTOR TO MAP ARRAY INDEX FUNCTION
int check_true_position(int iAMapPosition){int iTruePosition=(iAMapPosition%iMapSize)*iGridSize;return iTruePosition;}

//MAIN PROGRAM FUNCTION
int main(void)
{
    aMap[0][0] = 1;

//------------------------------------\\\WINDOW\\\----------------------------------------//

    //WINDOW PARAMETERS
    const int iScreenWidth=iMapSize*iGridSize; const int iScreenHeight=iMapSize*iGridSize;
    //WINDOW INITIALIZATION
    InitWindow(iScreenWidth, iScreenHeight, "RAYLIB");
    //WINDOW CHOSEN
    eGameScreen eCurrentScreen=TITLE;
    //SET WINDOW FPS
    SetTargetFPS(60);

//------------------------------------\\\WINDOW\\\----------------------------------------//

//------------------------------------\\\GAME LOGIC\\\------------------------------------//

    int iPlayerPos[] = {0, 0};
    //GAME LOOP FUNCTION
    while (!WindowShouldClose())    
    {
	//VERIFY CURRENT SCREEN
        switch (eCurrentScreen)
        {
            case TITLE:{if(IsKeyPressed(KEY_SPACE)||IsGestureDetected(GESTURE_TAP)){eCurrentScreen=GAMEPLAY;}}break;
            case GAMEPLAY:
            {

//------------------------------------///PLAYER MOVEMENT///------------------------------//
		
		//PLAYER DIRECTION ENUM
		typedef enum eDirection {UP = 0, DOWN, LEFT, RIGHT} eDirection;

		//UPDATE MAP INDEX FOR PLAYER POSITION FUNCTION
		void update_player_position(eDirection eDirection)
		{
			int iNewPlayerPos[] = {iPlayerPos[0], iPlayerPos[1]};
			switch(eDirection)
			{
				case UP:{iNewPlayerPos[1]--;}break;
				case DOWN:{iNewPlayerPos[1]++;}break;
				case LEFT:{iNewPlayerPos[0]--;}break;
				case RIGHT:{iNewPlayerPos[0]++;}break;
				default:break;	
			}
			//BOUNDARY CHECKS
			switch(eDirection)
			{
				case UP:{if(iNewPlayerPos[1]<0){iNewPlayerPos[1]+=iMapSize;}}break;
				case DOWN:{if(iNewPlayerPos[1]>=iMapSize){iNewPlayerPos[1]-=iMapSize;}}break;
				case LEFT:{if(iNewPlayerPos[0]<0){iNewPlayerPos[0]+=iMapSize;}}break;
				case RIGHT:{if(iNewPlayerPos[0]>=iMapSize){iNewPlayerPos[0]-=iMapSize;}}break;
				default:break;	
			}
			int iNewMapPosition = aMap[iNewPlayerPos[1]][iNewPlayerPos[0]];
			switch(iNewMapPosition)
			{
				case 0:
					{
					       aMap[iPlayerPos[1]][iPlayerPos[0]]=0;
					       iPlayerPos[0]=iNewPlayerPos[0]; iPlayerPos[1]=iNewPlayerPos[1];
					       aMap[iPlayerPos[1]][iPlayerPos[0]]=1;
					}break;
				case 2:{eCurrentScreen = GAMEOVER;}break;
			}
		}
		if(IsKeyPressed(KEY_W)){update_player_position(UP);}
		else if(IsKeyPressed(KEY_A)){update_player_position(LEFT);}
		else if(IsKeyPressed(KEY_S)){update_player_position(DOWN);}
		else if(IsKeyPressed(KEY_D)){update_player_position(RIGHT);}

//------------------------------------///PLAYER MOVEMENT///------------------------------//
            
	    }break;
            default:break;
	}

//------------------------------------\\\GAME LOGIC\\\------------------------------------//

//------------------------------------\\\DRAW\\\----------------------------------------//

	//DRAW BACKGROUND COLORED RECTANGLE FUNCTION
	void draw_background(Color eColor){DrawRectangle(0, 0, iScreenHeight, iScreenWidth, eColor);}
	BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(eCurrentScreen)
            {
                case TITLE:
                {
		    draw_background(BLACK);
                    DrawRectangle((iScreenHeight/2)-100, (iScreenWidth/2)-50, 200, 100, WHITE);
                    DrawText("TESTE RAYLIB", (iScreenHeight/2)-80, (iScreenWidth/2)-10, 20, BLACK);
                } break;

//------------------------------------///MAIN GAME///------------------------------//

                case GAMEPLAY:
       		{
		    //DRAWING MAP INDEX SQUARE FUNCTION
	    	    void draw_square(Color eColor, int iPositionX, int iPositionY)	
		    {
			int aTrueSquarePosition[] = {check_true_position(iPositionX), check_true_position(iPositionY)};
			DrawRectangle(aTrueSquarePosition[0], aTrueSquarePosition[1], iGridSize, iGridSize, eColor);	
		    }
		    for(int i=0; i<iMapSize; i++) 
		    {
			    for(int j=0; j<iMapSize; j++)
			    {
				switch(aMap[i][j])
			    	{
			    	case 1:
					{
						//SET PLAYER POSITION TO MAP INDEX
						iPlayerPos[0]=j;
						iPlayerPos[1]=i;
						draw_square(GREEN, iPlayerPos[0], iPlayerPos[1]);
					}break;			
			    	default:break;
			    	}
			     }
		    }
		    //DRAW MAP LINES 
		    int iLines=0;
		    for (int i=0; i<=(iMapSize-1); i++) 
		    {
			DrawLine(0, iLines, iScreenWidth, iLines, BLACK);
		    	DrawLine(iLines, 0, iLines, iScreenHeight, BLACK);
		        iLines += iGridSize;	
		    }
                }break;

//------------------------------------///MAIN GAME///------------------------------//
	    	
		case GAMEOVER:
	    	{
		    draw_background(BLACK);
                    DrawRectangle((iScreenHeight/2)-100, (iScreenWidth/2)-50, 200, 100, WHITE);
                    DrawText("GAME OVER", (iScreenHeight/2)-80, (iScreenWidth/2)- 0, 20, BLACK);
		}break;
                default:break;
            }
        EndDrawing();
    }

//------------------------------------\\\DRAW\\\----------------------------------------//
    
    //CLOSE THE GAME
    CloseWindow();
    return 0;
}

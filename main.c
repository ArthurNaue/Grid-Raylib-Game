#include <stdio.h>
#include <stdlib.h>
#include "include/raylib.h"

#define iMapSize 10
#define iGridSize 32

//GAME SCREENS
typedef enum eGameScreen {LOGO=0, TITLE, GAMEPLAY, GAMEOVER, MAPCREATION} eGameScreen;

//GAME MAP
int aMap[iMapSize][iMapSize]={0};
int aMapCreation[iMapSize][iMapSize]={0};

//VECTOR TO MAP ARRAY INDEX FUNCTION
int check_true_position(int iAMapPosition){int iTruePosition=(iAMapPosition%iMapSize)*iGridSize;return iTruePosition;}

//MAIN PROGRAM FUNCTION
int main(void)
{
    //SPAWN PLAYER AT [0, 0]
    aMap[0][0]=1;

//------------------------------------\\\WINDOW\\\----------------------------------------//

    //WINDOW PARAMETERS
    const int iScreenWidth=iMapSize*iGridSize; const int iScreenHeight=iMapSize*iGridSize;
    //WINDOW INITIALIZATION
    InitWindow(iScreenWidth, iScreenHeight, "2D GRID GAME ENGINE");
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
            case TITLE:{if(IsKeyPressed(KEY_SPACE)||IsGestureDetected(GESTURE_TAP)){eCurrentScreen=GAMEPLAY;}else if(IsKeyPressed(KEY_M)){eCurrentScreen=MAPCREATION;}}break;
            case GAMEPLAY:
            {

//------------------------------------///PLAYER MOVEMENT///------------------------------//
		
		//PLAYER DIRECTION ENUM
		typedef enum eDirection {UP=0, DOWN, LEFT, RIGHT} eDirection;

		//UPDATE MAP INDEX FOR PLAYER POSITION FUNCTION
		void update_player_position(eDirection eDirection)
		{
			//VERIFY NEW POSITION MAP INDEX
			int iNewPlayerPos[]={iPlayerPos[0], iPlayerPos[1]};
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
			//VERIFY TYPE OF SQUARE IN NEW POSITION
			int iNewMapPosition=aMap[iNewPlayerPos[1]][iNewPlayerPos[0]];
			switch(iNewMapPosition)
			{
				case 0:
					{
					       aMap[iPlayerPos[1]][iPlayerPos[0]]=0;
					       iPlayerPos[0]=iNewPlayerPos[0]; iPlayerPos[1]=iNewPlayerPos[1];
					       aMap[iPlayerPos[1]][iPlayerPos[0]]=1;
					}break;
				case 2:{eCurrentScreen=GAMEOVER;}break;
			}
		}
		//UPDATE PLAYER POSITION BASED ON DIRECTION
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

	//DRAW MAP LINES FUNCTION
	void draw_lines()
	{
	    int iLines=0;
	    for (int i=0; i<=(iMapSize-1); i++) 
	    {
		DrawLine(0, iLines, iScreenWidth, iLines, BLACK);
	    	DrawLine(iLines, 0, iLines, iScreenHeight, BLACK);
	        iLines += iGridSize;	
	    }
      	}
	
	//DRAWING MAP INDEX SQUARE FUNCTION
	void draw_square(Color eColor, int iPositionX, int iPositionY){DrawRectangle(check_true_position(iPositionX), check_true_position(iPositionY), iGridSize, iGridSize, eColor);}

	//BLUEPRINT FOR BUTTON
	typedef struct sButton
	{
		int iMapIndex;
		int iColorIndex;
		Rectangle rButtonColision;
	}sButton;

            //START TO DRAW ON SCREEN
	    BeginDrawing();
	    //SET BACKGROUND TO WHITE
            ClearBackground(RAYWHITE);

            switch(eCurrentScreen)
            {
                case TITLE:
                {
		    draw_background(BLACK);
                    DrawRectangle((iScreenHeight/4), (iScreenWidth/4), iScreenHeight/2, iScreenWidth/2, WHITE);
                    DrawText("2D GRID GAME", (iScreenHeight/4), (iScreenWidth/2), ((iMapSize*iGridSize)/14), BLACK);
                } break;

//------------------------------------///MAIN GAME///------------------------------//

                case GAMEPLAY:
       		{
		    //MAP DRAWING
		    for(int i=0; i<iMapSize; i++) 
		    {
			    for(int j=0; j<iMapSize; j++)
			    {
				switch(aMap[i][j])
			    	{
			    	case 1:
					{
						//SET PLAYER POSITION TO MAP INDEX
						iPlayerPos[0]=j; iPlayerPos[1]=i;
						draw_square(GREEN, iPlayerPos[0], iPlayerPos[1]);
					}break;		
				case 2:{draw_square(RED, j, i);}break;	
				case 3:{draw_square(PURPLE, j, i);}break;	
			    	default:break;
			    	}
			     }
		    }
		    draw_lines();
	   }break;

//------------------------------------///MAIN GAME///------------------------------//
	    	
		case GAMEOVER:
	    	{
		    draw_background(BLACK);
                    DrawRectangle((iScreenHeight/4), (iScreenWidth/4), iScreenHeight/2, iScreenWidth/2, WHITE);
                    DrawText("GAME OVER", (iScreenHeight/4), (iScreenWidth/2), ((iMapSize*iGridSize)/12), BLACK);
		}break;
		case MAPCREATION:
		{

	            //CREATE BUTTON
		    sButton sColorButton;
  	            sColorButton.iMapIndex=aMapCreation[0][0]; sColorButton.iColorIndex=0; 
		    sColorButton.rButtonColision=(Rectangle){0, 0, iGridSize, iGridSize};

		    //MAP DRAWING
		    for(int i=0; i<iMapSize; i++) 
		    {
			    for(int j=0; j<iMapSize; j++)
			    {
				sColorButton.rButtonColision.x=check_true_position(j); sColorButton.rButtonColision.y=check_true_position(i);

				if(CheckCollisionPointRec(GetMousePosition(), sColorButton.rButtonColision)){if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){aMapCreation[i][j]++; if(aMapCreation[i][j]>=4){aMapCreation[i][j]=0;}}}

				switch(aMapCreation[i][j])
			    	{
			    		case 1:{draw_square(GREEN, j, i);}break;		
					case 2:{draw_square(RED, j, i);}break;	
					case 3:{draw_square(PURPLE, j, i);}break;	
			    		default:break;
			    	}
			     }
		    }
		    draw_lines(); 
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

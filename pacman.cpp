#include <GL/glut.h>

#include <math.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <pthread.h>
#include <png.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <unistd.h>

#include "png_load.h"
#include "load_and_bind_texture.h"
#include "textures.h"
#include "maze.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Interface.h"

int gameTick = 0, deathTick = 0, frightenTick = 0, timestamp, score = 0, pills = 244, lives = 3, eatenCount = 0, eatStamp, fruitCounter = 0, tempFruitCounter = 0, eatFruitStamp = 0, eatenFruitX, eatenFruitY, fruitSpawned = 0;
bool frighten, paused = false, dead = false;
float eatenX, eatenY;
Pacman pacman;

pthread_mutex_t lock;
pthread_mutex_t lock1;

pthread_mutex_t D_lock;			
pthread_mutex_t g_lock;			

pthread_mutex_t DetectPill_Lock;
pthread_mutex_t setMode_Lock;
pthread_mutex_t Lock2;	

/*
 * This section involves the declaration of each ghost character in the game. Each ghost is assigned initial starting 
 * positions with specified X and Y coordinates, and each is given a unique color to differentiate them on the game board.
 * All ghosts are stored together in an array, which allows for efficient batch processing and management through 
 * iterative operations, making it easier to update or modify their states collectively during the game runtime.
 */

Ghost ghosts[4] =	{
						Ghost(13.5f, 19.0f, RED),
						Ghost(11.5f, 16.0f, BLUE),
						Ghost(13.5f, 16.0f, PINK),
						Ghost(15.5f, 16.0f, YELLOW),
					};

typedef enum {BEGIN, PLAY, DIE, OVER} gameState; 	// Enum defining each of the states the game can be in

gameState stateGame = BEGIN; 						// Initially start the game in begin mode (duh)

/*
 * In each move, Pac-Man evaluates the tile he occupies. If it contains a large pill (denoted by 'O'), the following code 
 * executes to trigger frighten mode for the ghosts. Upon entering frighten mode, the ghosts reverse their direction to 
 * enhance the gameplay dynamics, providing a strategic advantage to Pac-Man.
 */

void detectPill()
{
	//Lock
	
	pthread_mutex_lock(&DetectPill_Lock);
	int i = 0;
	
	switch(pacman.getTile(pacman.pacmanX, pacman.pacmanY))
	{
		case O:
		
			frighten = true; 			// Set the flag to know that frigthen mode is on
		    frightenTick = 0; 			// Set the counter to 0
			
			while(i<4)
		    {
		    	// Only change the ghosts mode if they are in CHASE or SCATTER mode
		    	
		    	if(ghosts[i].moveType == CHASE)
		        {		            
		            ghosts[i].eaten = false;
		            ghosts[i].moveType = FRIGHTEN;
		            
		            switch(ghosts[i].currentDir)
		            {
		            	case LEFT:
		            	
		            		ghosts[i].currentDir = RIGHT;
		            		break;
		            		
		            	case UP:
		            	
		            		ghosts[i].currentDir = DOWN;
		            		break;
		            		
		            	case RIGHT:
		            	
		            		ghosts[i].currentDir = LEFT;
		            		break;
		            		
		            	default:
		            	
		            		ghosts[i].currentDir = UP;
		            		break;
		            }
		        }
		        
		        if(ghosts[i].moveType == SCATTER) 
		        {
		        	ghosts[i].eaten = false;
		            ghosts[i].moveType = FRIGHTEN;
		            
		            switch(ghosts[i].currentDir)
		            {
		            	case LEFT:
		            	
		            		ghosts[i].currentDir = RIGHT;
		            		break;
		            		
		            	case UP:
		            	
		            		ghosts[i].currentDir = DOWN;
		            		break;
		            		
		            	case RIGHT:
		            	
		            		ghosts[i].currentDir = LEFT;
		            		break;
		            		
		            	default:
		            	
		            		ghosts[i].currentDir = UP;
		            		break;
		            }
		        }
		        
		        i++;
		    }
		    
		    break;
		    
		case F:
		
			eatenFruitX = pacman.pacmanX;
    		eatenFruitY = pacman.pacmanY;	    
		
			break;
	}
	
    // unlock
    
    pthread_mutex_unlock(&DetectPill_Lock);
}



/*
 * During each move, Pac-Man checks if he shares a tile with any ghost. If so, the code assesses the current mode of the ghost 
 * and executes the appropriate response based on that mode. This decision-making process is crucial for determining 
 * interactions between Pac-Man and the ghosts, affecting game outcomes.
 */



void detectGhost()
{
	pthread_mutex_lock(&g_lock);  
	
    for(int i = 0; i < 4; i++) // Check pacmans position against all of the ghosts
    {
        
    }
    
    int i=0;
    
    while(i<4)
    {
    	if(pacman.getPacX() == ghosts[i].getGhostX()) // If he is on the same tile as any ghost
        {
        	if(pacman.getPacY() == ghosts[i].getGhostY())
        	{
        		if(ghosts[i].moveType != FRIGHTEN) // Check if they are in a mode which is damaging to pacman
		        {
		        	if(ghosts[i].moveType != DEATH)
		        	{
		        		stateGame = DIE; // Execute the DIE gameType as the ghost has caught pacman
		            	timestamp = gameTick;
		        	}
		        } 
		        else 
		        {
		            if(!ghosts[i].eaten) // Check the ghost hasnt already been eaten
		            {
		                eatenCount++;
		                eatStamp = gameTick;
		                
		                if(eatenCount == 1)		// Check what eaten number the ghost is in order to increment the score correctly
		                {
		                	score+=200;
		                }
		                else if(eatenCount == 2)
		                {
		                	score+=400;
		                }
		                else if(eatenCount == 3)
		                {
		                	score+=800;
		                }
		                else if(eatenCount == 4)
		                {
		                	score+=1000;
		                }
		                
		                eatenX = ghosts[i].ghostX; // Store the position of the eating in order to draw the eating score tex in the correct place
		                eatenY = ghosts[i].ghostY;
		                ghosts[i].eaten = true; // Set the flag for that specifc ghost to have been eaten so it cannot be eaten again in this frighten cycle
		            }
		            
		            ghosts[i].moveType = DEATH; // Set the ghosts mode to DEATh as it has been eaten
		        }
        	}
        }
        
        i++;
    }
    
    if(stateGame == DIE) // Check to see if pacman was indeed caught by the ghost and if so take away a life
    {
    	if(!dead)
    	{
    		lives--;
        	dead = true;
    	}
    }
    
    pthread_mutex_unlock(&g_lock);  
}

/*
 * This code adjusts the movement strategy of the ghosts based on the current game tick. Throughout the game, ghosts alternate
 * between different behavioral patterns, specifically CHASE and SCATTER modes, which occur in distinct waves. The adjustment
 * of movement techniques according to the game tick ensures dynamic gameplay and continuous challenges for the player as the
 * ghosts switch between aggressively pursuing Pac-Man and retreating to predefined corners of the maze.
 */



void setMode()
{

	pthread_mutex_lock(&setMode_Lock);	
	
    if(gameTick >= 4450) 
    {	
    	int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = CHASE; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    } 
    else if(gameTick >= 4200) 
    {
        int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = SCATTER; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    } 
    else if(gameTick >= 3200) 
    {
    	int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = CHASE; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    } 
    else if(gameTick >= 2950) 
    {
        int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = SCATTER; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    }
    else if(gameTick >= 1950) 
    {
        int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = CHASE; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    } 
    else if (gameTick >= 1600) 
    {
        int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = SCATTER; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    } 
    else if(gameTick >= 600) 
    {
        int i = 0;
    
    	while(i<4)
    	{
    		if (ghosts[i].moveType != PEN && ghosts[i].moveType != LEAVE && !frighten && ghosts[i].moveType != DEATH)  // As long as the ghosts are only in SCATTER or CHASE and frighten mode is not on
            {
                ghosts[i].moveType = CHASE; // Set the correct move type for all ghosts
            }
    		
    		i++;
    	}
    }
    
    pthread_mutex_unlock(&setMode_Lock);	
}

/*
 * This method encompasses all the operations that require verification or modification at every tick of the game. 
 * It acts as a central hub for updating game states, checking conditions, and implementing changes that need to 
 * occur in real-time as the game progresses. This ensures that the game maintains accurate state tracking and 
 * responds appropriately to player actions and game events at each tick.
 */



void idle()
{
	pthread_mutex_lock(&lock1);  // Lock before input 
    
    if(paused==false)
    {	
    	if(stateGame == BEGIN)
    	{
    		if (!(gameTick < 250)) // After a certain amount of time switch the game into PLAY mode
            {
                stateGame = PLAY;
            }
    	}
    	else if (stateGame == PLAY)
    	{
 			setMode(); // Set the movement mode for Ghosts
            detectPill(); // Check if pacman has eaten a super pill
            pacman.checkTile(); // Check if pacman has eaten a regular pill to increase score if he is in a portal
            detectGhost(); // Check if pacman has hit a ghost
            pacman.move(); // Move pacman in the maze
            detectGhost();
            
            int i = 0;
            
            while(i < 4)  // Move the ghosts in the maze
            {
                ghosts[i].move(ghosts[0]);
                i++;
            }
            
            detectGhost();
            
            if (frightenTick <= 450) // If frighten mode is on and not expired, increment the counter
            {
            	if(frighten)
            	{
            		frightenTick++;
            	}
            } 
            else if (frighten) // If frighten mode is on but needs to expire, run correct expirary code
            {
                frighten = false;
                eatenCount = 0;
                frightenTick = 0;
                int i = 0;
                
                while (i < 4) 
                {
                    if (ghosts[i].moveType == FRIGHTEN) 
                    {
                        ghosts[i].eaten = false;
                    }
                    
                    i++;
                }
                
                setMode(); // Set the ghosts back into the correct mode after frighten mode has finished
            }
            
            if (pills <= 0) // If all the pills have been eaten reset the level so pacman can continue
            {
                fruitReset();
                resetMaze();
                pacman.reset();
                pills = 244;
                fruitSpawned = 0;
                fruitCounter = 0;
                tempFruitCounter = 0;
                int i = 0;
                
                while (i < 4) 
                {
                    ghosts[i].reset();
                    i++;
                }
                
                gameTick = 0;
                stateGame = BEGIN;
            }  	
    	}
    	else if (stateGame == DIE)
    	{
    		if (!(gameTick - timestamp < 150))  // Play pacmans death animation
            {
                deathTick++;
            }
    	}
        
        gameTick++;

        glutPostRedisplay(); // Force a redraw at the end
    }
    
    pthread_mutex_unlock(&lock1);  // Lock before input 
}

void* threadFunction(void* arg) 
{
	glutIdleFunc(idle);
	return NULL;
}

/*
 * This method is designed to manage the input from special keys on the keyboard. It captures and processes the specific 
 * key ('key') that has been pressed by the player. This functionality is crucial for interpreting and responding to 
 * specialized commands or actions dictated by user input, enabling interactive and responsive gameplay based on the 
 * keys pressed.
 */



void special(int key, int, int)
{
	pthread_mutex_lock(&lock);  // Lock before input 
    
    // Handle special keys
    
    if(key == GLUT_KEY_LEFT)
    {
    	pacman.setDirStore(LEFT); // If left key press set pacmans next direction to left
    }
    if(key == GLUT_KEY_UP)
    {
    	pacman.setDirStore(UP); // If up key press set pacmans next direction to up
    }
    if(key == GLUT_KEY_RIGHT)
    {
    	pacman.setDirStore(RIGHT); // If right key press set pacmans next direction to right
    }
    if(key == GLUT_KEY_DOWN)
    {
    	pacman.setDirStore(DOWN); // If down key press set pacmans next direction to down
    }
    
	pthread_mutex_unlock(&lock);  // Unlock the mutex
}

/*
 * This method is executed at the very start of the game to initialize various critical components. It sets up essential 
 * elements and configurations that are necessary for the game to function properly from the outset. The initialization 
 * covers loading assets, setting initial game states, and preparing the game environment, ensuring that all necessary 
 * conditions are met before gameplay begins. This foundational step is vital for a smooth and stable gaming experience.
 */



void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
    loadAndBindTextures();
}



/*
 * This method is responsible for rendering objects onto the screen. It orchestrates the visual presentation of the game by
 * drawing various game elements, such as characters, obstacles, and backgrounds, to the display. This function is crucial
 * for updating the visuals in real-time, ensuring that players see a continuous and coherent representation of the game
 * world as they interact with it. The method effectively translates game data into graphical output, enhancing the gaming
 * experience through dynamic and responsive visual feedback.
 */

 
void display()
{
	pthread_mutex_lock(&D_lock);

    glClear (GL_COLOR_BUFFER_BIT);
    
    if (stateGame == BEGIN)
    {
    	drawMaze();
        pacman.draw();
        
        int i = 0;
        
        while(i < 4)
        {
            ghosts[i].draw();
        	i++;
        }
        
        drawInterface();
        drawReady();
    }
    
    else if (stateGame == PLAY)
    {
    	drawMaze();
        pacman.draw();
        
        if(pills <= 174) 
        {
        	if(fruitSpawned == 0)
        	{
        		locationFruit();
        	}
        }
        
        if(pills <= 74) 
        {
        	if(fruitSpawned == 1)
        	{
        		locationFruit();	
        	}
        }
        
        if(fruitCounter > tempFruitCounter) 
        {
            eatFruitStamp = gameTick;
        }
        
        if(!(gameTick - eatFruitStamp > 200) )
        {
            eatFruitScore(eatenFruitX, eatenFruitY);
        }
        
        int i =0;
        
        while(i < 4)
        {
            ghosts[i].draw();
            
            if(frighten)  
            {
            	if(!(gameTick - eatStamp > 200))
            	{
            		ghosts[i].drawEatScore(eatenX, eatenY);
            	}
            }
            i++;
        }
        
        drawInterface();
        tempFruitCounter = fruitCounter;
    }
    
    else if (stateGame == DIE)
    {
    	fruitReset();
        drawMaze();
        
        if(gameTick - timestamp < 150) // Freeze pacman momentarily after he is hit by a ghost
        {
            pacman.draw();
            int i =0;
            
            while(i < 4)
            {
                ghosts[i].draw();
                i++;
            }
        }
        
        if(gameTick - timestamp >= 150  && deathTick < 110) // After that time has passed play pacmans death animation
        {
            pacman.death();
        }
        
        if(gameTick - timestamp >= 310) // After the death animation, reset pacman
        {
            pacman.reset();
            
            int i = 0;
            
            while(i < 4)
            {
                ghosts[i].reset(); // Reset the ghosts also
                i++;
            }
            
            gameTick = 0;
            eatFruitStamp = -999999999;
            deathTick = 0;
            
            if(lives <= 0)  // If pacman has lives left begin the game again
            {
                setHighScore(score); // If the game is finished check if the high score needs to be updated
                stateGame = OVER;
            } 
            else  // Otherwise show the gameover screen
            {
            	stateGame = BEGIN;   
            }
        }
        
        drawInterface();
    }
    
    else if (stateGame == OVER)
    {
    	drawMaze();
        drawInterface();
        drawGameOver();
    }

    glutSwapBuffers(); // Force a redraw at the end
    
    pthread_mutex_unlock(&D_lock);
}

/*
 * Method to reset all aspects of the game mechanics if the player wishes to reset a new game once their previous one
 * is over.
 */
 
void resetGame()
{
    gameTick = 0;
    deathTick = 0;
    score = 0;
    lives = 3;
    pills = 244;
    fruitCounter = 0;
    fruitSpawned = 0;
    tempFruitCounter = 0;
    stateGame = BEGIN;
}

/*
 * Method to handle when a keyboard key has been pressed
 * key the key that has been pressed
 */
 
void keyboard(unsigned char key, int, int)
{

	pthread_mutex_lock(&Lock2);
	
	if(key == 'q')
	{
        exit(1); 		// quit!
	}
	
	if (key == 'p')		// Key to pause the game
	{
		if(paused) 
        {
            paused = false;
        } 
        else 
        {
            paused = true;
        }
	}
	
	if (key == 'r')
	{
		if(stateGame == OVER) 
        {
            resetGame();
            pacman.reset();
            fruitReset();
            resetMaze();
            
            int i = 0;
            
            while(i<4)
            {
            	ghosts[i].reset(); // Reset the ghosts also
            	i++;
            }
        }
	}

    glutPostRedisplay(); // force a redraw
    
    pthread_mutex_unlock(&Lock2);
}

void* threadFunction1(void* arg) 
{
	glutSpecialFunc(special);
        
	return NULL;
}

void* threadFunction2(void* arg) 
{
	glutKeyboardFunc(keyboard);
	
	return NULL;
}

void* DisplayFunction(void* arg) 
{
	glutDisplayFunc(display);
	
	return NULL;
}

void* ghostThreadsFunctions(void * arg)
{
	detectGhost();
	
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t gameThread;
	pthread_t gameThread1;
	pthread_t keyBoardThread;
	pthread_t DisplayThread;
	pthread_t ghostThreads[4];
	
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&D_lock, NULL);
	pthread_mutex_init(&g_lock, NULL);
	pthread_mutex_init(&DetectPill_Lock,NULL);
	pthread_mutex_init(&setMode_Lock,NULL);
	pthread_mutex_init(&Lock2,NULL);

	glutInit(&argc, argv); 	// we can pass certain X windows attributes to GLUT

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

    glutInitWindowSize(950,950);
    glutCreateWindow("Pacman"); 	// a named window of default size and position
    
    init();
  
   	pthread_create(&DisplayThread, NULL, DisplayFunction, NULL);
	pthread_create(&keyBoardThread, NULL, threadFunction2, NULL);
	pthread_create(&gameThread1, NULL, threadFunction1, NULL);
	
	int i = 0;
	
	while (i<4)
	{
		pthread_create(&ghostThreads[i], NULL, ghostThreadsFunctions, (void*)&ghosts[i]);
		i++;
	}
    
    int result = pthread_create(&gameThread, NULL, threadFunction, NULL);
    
    glutMainLoop(); // go into the main loop and wait for window events...
    
    return 0; // safe to return 0 to shell unless error
}

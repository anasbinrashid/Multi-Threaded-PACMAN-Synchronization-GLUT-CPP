#ifndef PACMAN_H
#define PACMAN_H

int pacTick = 0; // Counter to manage timing and animations for Pacman's movements.

extern int score; // Global variable to keep track of the score.
extern int pills; // Global counter for the remaining pills on the board.
extern int deathTick; // Counter for the timing of death animations.
extern bool dead; // Flag to check if Pacman is currently dead.
extern int eatenCount; // Counter for the number of ghosts Pacman has eaten.
extern int fruitCounter; // Counter for the number of fruits Pacman has eaten.

bool begin = true; // Flag to indicate the start of the game or a reset.

typedef enum {LEFT, UP, RIGHT, DOWN, NONE} direction; // Enumeration for possible movement directions.

// Class definition for Pacman.
class Pacman
{
public:
    float pacmanX; // X-coordinate of Pacman's position.
    float pacmanY; // Y-coordinate of Pacman's position.
    direction currentDir; // Current direction of Pacman's movement.
    direction dirStore; // Temporary storage for direction input to handle smooth direction changes.
    float angle; // Angle to rotate Pacman's sprite for rendering.

    // Constructor to initialize Pacman's properties.
    Pacman()
    {
        pacmanX = 13.5f; // Starting X position.
        pacmanY =  7.0f; // Starting Y position.
        currentDir = NONE; // Pacman starts without movement.
        dirStore = NONE; // No stored direction initially.
    }

    // Function to draw Pacman at his current position.
    void draw()
    {
        glPushMatrix(); // Save the current transformation of the matrix.

        unsigned int pac_tex; // Texture ID for Pacman's current sprite.
        int tickDiv; // Variable to manage the animation frames based on time.

        if(!dead) // Calculate the frame only if Pacman is not dead.
        {
            tickDiv = pacTick % 30; // Cycle through frames every 30 ticks.
        }
        
        if(currentDir == NONE) // If Pacman is not moving, reset the animation.
        {
            tickDiv = 0;
        }
        
        if(gameTick < 250) // Use a special texture if the game is just beginning.
        { 
            pac_tex = pac_2_tex; // Texture showing Pacman slightly open.
        } 
        else 
        {
            if (tickDiv < 10) // First animation frame.
            {
                pac_tex = pac_0_tex; // Pacman closed mouth.
            } 
            else if (tickDiv < 20) // Second animation frame.
            {
                pac_tex = pac_1_tex; // Pacman slightly open.
            } 
            else // Third animation frame.
            {
                pac_tex = pac_2_tex; // Pacman fully open.
            }
        }

        switch(currentDir) // Rotate Pacman based on his current direction.
        { 
            case LEFT:
                angle = 0.0f; // Facing left requires no rotation.
                break;
            
            case UP:
                angle = 270.0f; // Rotate Pacman 270 degrees to face up.
                break;
            
            case RIGHT:
                angle = 180.0f; // Rotate 180 degrees to face right.
                break;
            
            case DOWN:
                angle = 90.0f; // Rotate 90 degrees to face down.
                break;
            
            default:
                currentDir = NONE; // If direction is none, keep Pacman facing left.
        }

        translateBottomLeft(); // Adjust origin to bottom left of the screen.
        translateToMazeCoords(pacmanX, pacmanY); // Translate position to maze coordinates.

        glTranslatef(-3.0f, -3.0f, 0.0f); // Adjust for Pacman's size for proper texture alignment.

        drawTex(pac_tex, 14, 14, angle); // Draw Pacman with the appropriate texture and rotation.

        glPopMatrix(); // Restore the previous transformation matrix.

        pacTick++; // Increment Pacman's tick counter for animations.
    }

    // Function to reset all of Pacman's variables to their initial state.
    void reset()
    {
        pacmanX = 13.5f; // Reset X position.
        pacmanY =  7.0f; // Reset Y position.
        currentDir = NONE; // Reset direction to none.
        dirStore = NONE; // Clear stored direction.
        angle = 0.0f; // Reset angle to zero.
        begin = true; // Indicate the start or restart of the game.
        dead = false; // Pacman is not dead.
        eatenCount = 0; // Reset count of eaten ghosts.
    }

    // Function to handle Pacman's death animation and resetting his position.
    void death()
    {
        glPushMatrix(); // Save current transformation state.

        unsigned int pac_tex; // Texture ID for death animation sprites.

        // Select the appropriate death sprite based on deathTick.
        if (deathTick < 10)
        {                   
            pac_tex = dead_0_tex; // First sprite in death animation.
        } 
        else if (deathTick % 110 < 20) 
        {
            pac_tex = dead_1_tex; // Second sprite.
        } 
        else if (deathTick % 110 < 30) 
        {
            pac_tex = dead_2_tex; // Third sprite.
        } 
        else if (deathTick % 110 < 40) 
        {
            pac_tex = dead_3_tex; // Fourth sprite.
        } 
        else if (deathTick % 110 < 50) 
        {
            pac_tex = dead_4_tex; // Fifth sprite.
        } 
        else if (deathTick % 110 < 60) 
        {
            pac_tex = dead_5_tex; // Sixth sprite.
        } 
        else if (deathTick % 110 < 70) 
        {
            pac_tex = dead_6_tex; // Seventh sprite.
        } 
        else if (deathTick % 110 < 80) 
        {
            pac_tex = dead_7_tex; // Eighth sprite.
        } 
        else if (deathTick % 110 < 90) 
        {
            pac_tex = dead_8_tex; // Ninth sprite.
        } 
        else if (deathTick % 110 < 100) 
        {
            pac_tex = dead_9_tex; // Tenth sprite.
        } 
        else 
        {
            pac_tex = dead_10_tex; // Final sprite in the sequence.
        }

        translateBottomLeft(); // Adjust origin to bottom left of screen.
        translateToMazeCoords(pacmanX, pacmanY); // Translate position to maze coordinates.

        glTranslatef(-3.0f, -3.0f, 0.0f); // Adjust for the size of the sprite.

        drawTex(pac_tex, 14, 14, 0); // Draw the death sprite with no rotation.

        glPopMatrix(); // Restore previous transformation state.
    }

    // Function to check the type of tile Pacman is currently on and react accordingly.
    void checkTile()
    {
        switch(getTile(pacmanX, pacmanY)) 
        {
            case o: // If on a small pill tile.
                maze[(int) floor(pacmanX)][(int) floor(pacmanY)] = e; // Mark the tile as empty.
                score += 10; // Increment score by 10.
                pills--; // Decrement the number of pills left.
                break;
            
            case O: // If on a large pill tile.
                maze[(int) floor(pacmanX)][(int) floor(pacmanY)] = E; // Mark the tile as empty and triggered.
                score += 50; // Increment score by 50.
                pills--; // Decrement the number of pills left.
                break;
            
            case P: // If on a portal tile.
                if (getPacX() >=26.5) // Check if on the right-side portal.
                {
                    pacmanX = 1.0f; // Move to the left-side of the maze.
                } 
                else 
                {
                    pacmanX = 26.5f; // Move to the right-side of the maze.
                }
                break;
            
            case F: // If on a fruit tile.
                maze[(int) floor(pacmanX)][(int) floor(pacmanY)] = e; // Mark the tile as empty.
                fruitCounter++; // Increment the fruit counter.
                
                if(fruitCounter == 1) 
                { 
                    score += 100; // First fruit eaten, score 100.
                } 
                else 
                {
                    score += 300; // Subsequent fruits eaten, score 300.
                }
        }
    }

    // Function to return Pacman's current X position as an integer.
    int getPacX()
    {
        return (int)floor(pacmanX);
    }

    // Function to return Pacman's current Y position as an integer.
    int getPacY()
    {
        return (int)floor(pacmanY);
    }

    // Function to return the type of tile at the specified coordinates.
    tile getTile(float x, float y)
    {
        return maze[(int)floor(x)][(int)floor(y)];
    }

    // Function to check the tile type ahead of Pacman in the specified direction.
    tile getFollowingTile(direction dir)
    {
        switch(dir) 
        {
            case LEFT:
                return getTile(getPacX() - 1, getPacY()); // Check tile to the left.
            
            case UP:
                return getTile(getPacX(), getPacY() + 1); // Check tile above.
            
            case RIGHT:
                return getTile(getPacX() +1, getPacY()); // Check tile to the right.
            
            case DOWN:
                return getTile(getPacX(), getPacY() - 1); // Check tile below.
            
            default:
                return getTile(getPacX(), getPacY()); // Default case returns current tile.
        }
    }

    // Function to check if Pacman is precisely at the center of his current tile.
    bool isAtCenter()
    {
        return (int)(pacmanY * 10.0f) % 10 == 0 && (int)(pacmanX * 10.0f) % 10 == 0;
    }

    // Function to store the user's pressed direction for later execution.
    void setDirStore(direction pressedDir)
    {
        dirStore = pressedDir;
    }

    // Function to check if the next tile in the specified direction is a wall.
    bool isWall(direction dir)
    {
        if (getFollowingTile(dir) == W || getFollowingTile(dir) == G)
        {
            return true; // Return true if the next tile is a wall.
        }

        return false; // Return false if it is not a wall.
    }

    // Main movement function to translate Pacman based on his current direction and user inputs.
    void move()
    {
        if(isAtCenter()) // Only allow movement adjustments when at the center of a tile.
        { 
            if (!isWall(dirStore)) // If the new direction is not blocked, update current direction.
            { 
                currentDir = dirStore;
            } 
            else if (isWall(currentDir)) // If the current direction is blocked, stop Pacman.
            {
                currentDir = NONE;
            }
        }
        
        if(!isAtCenter() && dirStore != NONE && begin && !isWall(dirStore))
        { 
            // Special case for starting movement at the beginning of the game.
            currentDir = dirStore;
            begin = false;
        }

        switch (currentDir) // Translate Pacman's position based on the current direction.
        {
            case LEFT:
                pacmanX -= 0.1f; // Move left.
                pacmanY = round(pacmanY); // Round Y coordinate to stay in line.
                break;
            
            case UP:
                pacmanY += 0.1f; // Move up.
                pacmanX = round(pacmanX); // Round X coordinate to stay in line.
                break;
            
            case RIGHT:
                pacmanX += 0.1f; // Move right.
                pacmanY = round(pacmanY); // Round Y coordinate to stay in line.
                break;
            
            case DOWN:
                pacmanY -= 0.1f; // Move down.
                pacmanX = round(pacmanX); // Round X coordinate to stay in line.
                break;
            
            default:
                if(!begin) 
                { 
                    // Round position to align to the grid if not at the start.
                    pacmanX = round(pacmanX);
                    pacmanY = round(pacmanY);
                }
        }
    }
};

#endif


#ifndef TEXTURES_H
#define TEXTURES_H

// Maze Textures

unsigned int maze_tex, pill_tex, bigPill_tex;

// Pacman Textures

unsigned int pac_0_tex, pac_1_tex, pac_2_tex;
unsigned int dead_0_tex, dead_1_tex, dead_2_tex, dead_3_tex, dead_4_tex, dead_5_tex, dead_6_tex, dead_7_tex, dead_8_tex, dead_9_tex, dead_10_tex;

// Ghost Textures

unsigned int ghost_r_0_tex, ghost_r_1_tex;
unsigned int ghost_p_0_tex, ghost_p_1_tex;
unsigned int ghost_b_0_tex, ghost_b_1_tex;
unsigned int ghost_y_0_tex, ghost_y_1_tex;
unsigned int ghost_scared_0_tex, ghost_scared_1_tex, ghost_scared_2_tex, ghost_scared_3_tex;

// Eye Textures

unsigned int eye_up_tex, eye_right_tex, eye_down_tex, eye_left_tex;

// UI Textures

unsigned int num_0_tex, num_1_tex, num_2_tex, num_3_tex, num_4_tex, num_5_tex, num_6_tex, num_7_tex, num_8_tex, num_9_tex;
unsigned int score_200_tex, score_400_tex, score_800_tex, score_1600_tex;
unsigned int ready_tex;
unsigned int gameover_tex;
unsigned int life_tex;
unsigned int score_tex;
unsigned int high_tex;
unsigned int score_100_tex, score_300_tex;

// Fruit Textures

unsigned int cherry_tex;
unsigned int strawberry_tex;

/*
 * Load and bind all textures at once to save performance as all only need to be load and bound once at the beginning during initialisation
 */
 
void loadAndBindTextures()
{
    // Enable blending so textures can be transparent
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Bind map textures

    maze_tex = load_and_bind_texture("sprites/maze/maze.png");
    pill_tex = load_and_bind_texture("sprites/maze/p-0.png");
    bigPill_tex = load_and_bind_texture("sprites/maze/p-1.png");

    // Bind Pacman textures
    
    pac_0_tex = load_and_bind_texture("sprites/pacman/0.png");
    pac_1_tex = load_and_bind_texture("sprites/pacman/1.png");
    pac_2_tex = load_and_bind_texture("sprites/pacman/2.png");
    dead_0_tex = load_and_bind_texture("sprites/pacman/d-0.png");
    dead_1_tex = load_and_bind_texture("sprites/pacman/d-1.png");
    dead_2_tex = load_and_bind_texture("sprites/pacman/d-2.png");
    dead_3_tex = load_and_bind_texture("sprites/pacman/d-3.png");
    dead_4_tex = load_and_bind_texture("sprites/pacman/d-4.png");
    dead_5_tex = load_and_bind_texture("sprites/pacman/d-5.png");
    dead_6_tex = load_and_bind_texture("sprites/pacman/d-6.png");
    dead_7_tex = load_and_bind_texture("sprites/pacman/d-7.png");
    dead_8_tex = load_and_bind_texture("sprites/pacman/d-8.png");
    dead_9_tex = load_and_bind_texture("sprites/pacman/d-9.png");
    dead_10_tex = load_and_bind_texture("sprites/pacman/d-10.png");

    // Bind ghost eye textures
    
    eye_up_tex = load_and_bind_texture("sprites/eyes/up.png");
    eye_right_tex = load_and_bind_texture("sprites/eyes/right.png");
    eye_down_tex = load_and_bind_texture("sprites/eyes/down.png");
    eye_left_tex = load_and_bind_texture("sprites/eyes/left.png");

    // Bind ghost textures
    
    ghost_r_0_tex = load_and_bind_texture("sprites/ghosts/r-0.png");
    ghost_r_1_tex = load_and_bind_texture("sprites/ghosts/r-1.png");
    ghost_p_0_tex = load_and_bind_texture("sprites/ghosts/p-0.png");
    ghost_p_1_tex = load_and_bind_texture("sprites/ghosts/p-1.png");
    ghost_b_0_tex = load_and_bind_texture("sprites/ghosts/b-0.png");
    ghost_b_1_tex = load_and_bind_texture("sprites/ghosts/b-1.png");
    ghost_y_0_tex = load_and_bind_texture("sprites/ghosts/y-0.png");
    ghost_y_1_tex = load_and_bind_texture("sprites/ghosts/y-1.png");
    ghost_scared_0_tex = load_and_bind_texture("sprites/ghosts/s-0.png");
    ghost_scared_1_tex = load_and_bind_texture("sprites/ghosts/s-1.png");
    ghost_scared_2_tex = load_and_bind_texture("sprites/ghosts/s-2.png");
    ghost_scared_3_tex = load_and_bind_texture("sprites/ghosts/s-3.png");

    // Bind UI textures
    
    num_0_tex = load_and_bind_texture("sprites/ui/0.png");
    num_1_tex = load_and_bind_texture("sprites/ui/1.png");
    num_2_tex = load_and_bind_texture("sprites/ui/2.png");
    num_3_tex = load_and_bind_texture("sprites/ui/3.png");
    num_4_tex = load_and_bind_texture("sprites/ui/4.png");
    num_5_tex = load_and_bind_texture("sprites/ui/5.png");
    num_6_tex = load_and_bind_texture("sprites/ui/6.png");
    num_7_tex = load_and_bind_texture("sprites/ui/7.png");
    num_8_tex = load_and_bind_texture("sprites/ui/8.png");
    num_9_tex = load_and_bind_texture("sprites/ui/9.png");
    score_200_tex = load_and_bind_texture("sprites/ui/200.png");
    score_400_tex = load_and_bind_texture("sprites/ui/400.png");
    score_800_tex = load_and_bind_texture("sprites/ui/800.png");
    score_1600_tex = load_and_bind_texture("sprites/ui/1600.png");
    ready_tex = load_and_bind_texture("sprites/ui/ready.png");
    gameover_tex = load_and_bind_texture("sprites/ui/gameover.png");
    life_tex = load_and_bind_texture("sprites/ui/life.png");
    score_tex = load_and_bind_texture("sprites/ui/score_tex.png");
    high_tex = load_and_bind_texture("sprites/ui/high.png");
    score_100_tex = load_and_bind_texture("sprites/ui/100.png");
    score_300_tex = load_and_bind_texture("sprites/ui/300.png");

    // Fruit textures

    cherry_tex = load_and_bind_texture("sprites/fruits/cherry.png");
    strawberry_tex = load_and_bind_texture("sprites/fruits/strawberry.png");
}

/*
 * Draws the texture passed in given the height, length and angle
 *
 * texture the unsigned int of the texture file that is to be drawn
 * length the length of the texture
 * height the height of the texture
 * angle the angle at which the texture is to be displayed. For instance pacman will need to rotate depending on his direction heading
 */
 
void drawTex(unsigned int texture, int length, int height, float angle)
{
    // Begin new transformation matrix
    
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);   // Set background colour to white. This should never be seen but allows a visual debug to see if a texture hasn't been loaded or bound correctly.

    int widthCentre = length/2, verticalCentre = height/2;

    // Translate to center of sprite to rotate about its origin (for sprites such as pacman)
    
    glTranslatef((float)widthCentre,(float)verticalCentre,0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // Enable texturing and bind the specific sprite
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Specify texture coordinates
    
	glBegin(GL_QUADS);
	glTexCoord2f (0.0f, 0.0f);      // Lower left corner
	glVertex2i(-widthCentre, -verticalCentre);
	glTexCoord2f (1.0f, 0.0f);      // Lower right corner
	glVertex2i(widthCentre, -verticalCentre);
	glTexCoord2f (1.0f, 1.0f);      // Upper right corner
	glVertex2i(widthCentre, verticalCentre);
	glTexCoord2f (0.0f, 1.0f);      // Upper left corner
	glVertex2i(-widthCentre, verticalCentre);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Pop matrix to ignore above transformations on future objects
    
    glPopMatrix();
}

#endif 

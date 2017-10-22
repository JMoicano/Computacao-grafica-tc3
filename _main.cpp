#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Player.h"
 
//Key status
int keyStatus[256];

// Window dimensions
const GLint Width = 700;
const GLint Height = 700;

// Viewing dimensions
const GLint ViewingWidth = 500;
const GLint ViewingHeight = 500;
 
int animate = 0;

Player player(0, 0, 30);

void renderScene(void)
{
     // Clear the screen.
     glClear(GL_COLOR_BUFFER_BIT);
 
     player.Desenha();

     glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    keyStatus[key] = 1;
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}

void init(void)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Black, no opacity(alpha).
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix    
    glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
            (ViewingWidth/2),     // X coordinate of right edge            
            -(ViewingHeight/2),     // Y coordinate of bottom edge             
            (ViewingHeight/2),     // Y coordinate of top edge             
            100,     // Z coordinate of the “near” plane            
            -100);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
      
}

void idle(void)
{
    //Treat keyPress
    if(keyStatus[(int)('w')])
    {
        player.MoveEmY(0.01);
    }
    if(keyStatus[(int)('s')])
    {
        player.MoveEmY(-0.01);
    }
    if(keyStatus[(int)('a')])
    {
        player.RodaPlayer(0.01);
    }
    if(keyStatus[(int)('d')])
    {
        player.RodaPlayer(-0.01);
    }
    if(keyStatus[(int)('p')])
    {
         player.Pula();
    }
    
    glutPostRedisplay();
}
 
int main(int argc, char *argv[])
{
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Tranformations 2D");
 
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    
    init();
 
    glutMainLoop();
 
    return 0;
}

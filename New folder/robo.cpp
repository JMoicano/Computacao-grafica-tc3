#include "robo.h"
#include <math.h>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
     glColor3f(R,G,B);
 
     glBegin(GL_QUADS);
          glVertex2f( -width/2.0, 0.0);                        // Top Left
          glVertex2f( -width/2.0, height);               // Bottom Left
          glVertex2f( width/2.0, height);               // Bottom Right
          glVertex2f( width/2.0, 0.0);                // Top Right
     glEnd();
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    int i;
    GLfloat x, y;
    glColor3f(R,G,B);
    glPointSize(3);
    glBegin(GL_POINTS);
        for (i = 0; i < 360; i+=20)
        {
            x = radius * cos(M_PI*i/180);
            y = radius * sin(M_PI*i/180);
            glVertex3f(x, y, 0);
        }
    glEnd();
}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
        //Desenha braco da base
        glTranslatef(x, y, 0);
        glRotatef(theta1, 0,0,1);
        DesenhaRect(paddleHeight, paddleWidth, 0, 0, 1); 

        //Desenha braco do meio
        glTranslatef(0.0, paddleHeight, 0);
        glRotatef(theta2, 0,0,1);
        DesenhaRect(paddleHeight, paddleWidth, 1, 1, 0); 
    
        //Desenha braco do fim
        glTranslatef(0.0, paddleHeight, 0);
        glRotatef(theta3, 0,0,1);
        DesenhaRect(paddleHeight, paddleWidth, 0, 1, 0); 

    glPopMatrix();
 
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
        glTranslatef(x, y, 0);
        //Desenha base
        DesenhaRect(baseHeight, baseWidth, 1, 0, 0); 
        
        //Desenha roda 1
        glPushMatrix();
            glTranslatef(-baseWidth/2.0, 0, 0);
            glRotatef(thetaWheel, 0,0,1);
            DesenhaCirc(radiusWheel, 1, 1, 1);
        glPopMatrix();
        //Desenha roda 2
        glPushMatrix();
            glTranslatef(baseWidth/2.0, 0, 0);
            glRotatef(thetaWheel, 0,0,1);
            DesenhaCirc(radiusWheel, 1, 1, 1);
        glPopMatrix();

        //Desenha bracos
        DesenhaBraco(0.0, baseHeight, theta1, theta2, theta3);
        
    glPopMatrix();
 
}

void Robo::RodaBraco1(GLfloat inc)
{
    gTheta1 += inc;
}

void Robo::RodaBraco2(GLfloat inc)
{
    gTheta2 += inc;
}

void Robo::RodaBraco3(GLfloat inc)
{
    gTheta3 += inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    gX += dx;
    gThetaWheel -= dx*360/(2*M_PI*radiusWheel); 
}

void Robo::MoveEmY(GLfloat dy)
{
    gY += dy;
}

void Robo::MoveEmXDir(GLfloat dx){
    MoveEmX(direction*dx); 
}

GLfloat Robo::ObtemX(){
    return gX;
}

GLfloat Robo::ObtemY(){
    return gY;
}
    
void Robo::ChangeDirection()
{
    direction *= -1.0; 
}

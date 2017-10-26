#ifndef ROBO_H
#define	ROBO_H
#include <GL/glut.h>
#include <iostream>
#include <cmath>
// Dimensions
#define legHeight .7
#define legWidth .4
#define gunHeight 1
#define gunWidth .2
#define bodyScale .3
#define headScale .6

class Player {
    GLfloat gX;
    GLfloat gY;
    GLfloat gThetaLeg;
    GLfloat gThetaGun;
    GLfloat gThetaPlayer;
    GLfloat radius;
    GLfloat inJumpScale;
    GLfloat jumpInitTime;
    GLfloat lastTime;
    bool inJump;
    bool above;

private:
    void DesenhaRect(  GLint height, GLint width,
                    GLfloat R, GLfloat G, GLfloat B);
    void DesenhaCirc(  GLint radius, GLfloat R,
                    GLfloat G, GLfloat B);
    void DesenhaPernas(GLfloat x, GLfloat y, GLfloat gThetaLeg);
    void DesenhaPlayer(GLfloat gX, GLfloat gY, GLfloat radius,
                    GLfloat gThetaLeg, GLfloat gThetaGun, GLfloat gThetaPlayer);

public:
    Player(GLfloat posX, GLfloat posY, GLfloat r){
        gX = posX;
        gY = posY;
        radius = r;
        gThetaLeg = 90;
        gThetaGun = 0;
        gThetaPlayer = 0;
        inJumpScale = 1;
        inJump = false;
        above = false;
        lastTime = glutGet(GLUT_ELAPSED_TIME);
    };
    void Desenha(){
        DesenhaPlayer(gX, gY, radius, gThetaLeg, gThetaGun, gThetaPlayer);
    };
    void RodaPlayer(GLfloat inc);
    void RodaArma(GLfloat inc);
    void MoveEmX(GLfloat dx);
    void MoveEmY(GLfloat, bool[]);
    void Pula();
    void DeterminaAcima(bool);
    GLfloat ObtemX();
    GLfloat ObtemY();
    GLfloat ObtemRaio();
    bool EstaPulando();
    bool EstaAcima();

};

#endif	/* ROBO_H */


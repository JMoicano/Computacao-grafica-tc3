#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <list>

#include <GL/glut.h>

#include "Window.h"
#include "Circle.h"
#include "Player.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

Player *player;
Circle *arena[2];
list<Circle*> highObstacles;
list<Circle*> lowObstacles;

int lastXMouse;
int keyFlags[256];
bool above = false;
int aboveI = -1;

double velTiro, velJogador;

bool inJump = false;
bool canMove[4];

int jumpInitTime;

Window *window;

void readParams(char* fileName){
	XMLDocument file;

	string fileNameS = fileName;

	if(fileNameS[fileNameS.size() - 1] != '/'){
		fileNameS + "/";
	}

	fileNameS.append("config.xml");

	string endereco;

	//Read XML file using tinyxml2
	if(file.LoadFile(fileNameS.c_str())){
		cout << "Erro ao ler arquivo XML" << endl;
	}else{
		XMLElement* rootTag = file.FirstChildElement();
		string rootValue;


		//Read parameters
		for(XMLElement* it = rootTag->FirstChildElement(); it != NULL; it = it->NextSiblingElement()){

			rootValue = it->Value();
			
			//Arena file params
			if(rootValue.compare("arquivoDaArena") == 0){

				endereco = it->Attribute("caminho");
				endereco += it->Attribute("nome");
				endereco += + ".";
				endereco += it->Attribute("tipo");

			}else if(rootValue.compare("jogador") == 0){

				velTiro = it->FloatAttribute("velTiro");
				velJogador = it->FloatAttribute("vel");
			}
		}


	}

	if(file.LoadFile(endereco.c_str())){
		cout << "Erro ao ler arquivo da arena" << endl;
	}else{
		XMLElement* rootTag = file.FirstChildElement();
		string rootValue;


		//Read circle parameters
		for(XMLElement* it = rootTag->FirstChildElement(); it != NULL; it = it->NextSiblingElement()){

			rootValue = it->Value();
			
			//Circle params
			if(rootValue.compare("circle") == 0){
				string cor = it->Attribute("fill");

				int x, y;
				double radius;

				x = it->IntAttribute("cx");
				y = it->IntAttribute("cy");
				radius = it->FloatAttribute("r");

				Circle *c = new Circle(radius, x, -y, 0);

				if(cor.compare("red") == 0){
					c->setColor(1, 0, 0);
					highObstacles.push_back(c);
				}else if(cor.compare("black") == 0){
					c->setColor(0, 0, 0);
					lowObstacles.push_back(c);
				}else if(cor.compare("blue") == 0){
					window = new Window(2 * radius, 2 * radius);
					c->setColor(0, 0, 1);
					arena[0] = c;
				}else if(cor.compare("white") == 0){
					c->setColor(1, 1, 1);
					arena[1] = c;
				}else if(cor.compare("green") == 0){
					player = new Player(x, -y, radius);
				}
			}
		}
	}
}

void initWindow(void)
{
	for (int i = 0; i < 256; ++i)
	{
		keyFlags[i] = 0;
	}
	canMove[0] = canMove[1] = canMove[2] = canMove[3] = true;
	 // select background color 
	glClearColor (1, 1, 1, 0.0);
	 // inicializar sistema de viz. 
	glMatrixMode(GL_PROJECTION);
	glOrtho(arena[0]->getCenterX() - arena[0]->getRadius(), arena[0]->getCenterX() + arena[0]->getRadius(), arena[0]->getCenterY() - arena[0]->getRadius(),arena[0]->getCenterY() + arena[0]->getRadius(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
	glLoadIdentity();	
}

void keyPress(unsigned char key, int x, int y){
	keyFlags[key] = 1;
}

void keyUp(unsigned char key, int x, int y){
	keyFlags[key] = 0;
}

void passiveMotion(int x, int y){
	player->RodaArma((int)((lastXMouse - x) * player->ObtemRaio() / window->getWidth()) * 8);
	lastXMouse = x;
}

void drawCircle(Circle *circle){
		 // Draw circle as polygon 
	glColor3f (circle->getColorR(), circle->getColorG(), circle->getColorB());
	glBegin(GL_POLYGON);
	glVertex3f(circle->getCenterX(), circle->getCenterY(), 0.0); // center of circle
	int resolution = 64;
	for(int i = 0; i <= resolution;i++) { 
		glVertex3f(
		circle->getCenterX() + (circle->getRadius() * cos(i * 2 * M_PI / resolution) * sin(M_PI / 2)), 
		circle->getCenterY() + (circle->getRadius() * sin(i * 2 * M_PI / resolution) * sin(M_PI / 2)),
		circle->getCenterZ() + (circle->getRadius() * cos(M_PI / 2))
		);
	}
	glEnd();

}

void display(void)
{
	 // Clear pixels 
	glClear (GL_COLOR_BUFFER_BIT);
	
	//Draw arena
	drawCircle(arena[0]);
	drawCircle(arena[1]);
	
	//Draw obstacles
	for (list<Circle*>::iterator iter = highObstacles.begin(); iter != highObstacles.end(); ++iter)
	{
		drawCircle((*iter));
	}
	for (list<Circle*>::iterator iter = lowObstacles.begin(); iter != lowObstacles.end(); ++iter)
	{
		drawCircle((*iter));
	}

	//Draw player
	player->Desenha();

	/* Dont Wait! */
	glutSwapBuffers();
}

double dist(Circle *c1, Player *c2){
	double distance = sqrt(pow(c1->getCenterX() - c2->ObtemX(), 2) + pow(c1->getCenterY() - c2->ObtemY(), 2));
}

void checkCollision(Circle *c1, Player *c2, bool intern = false){
	double distance = dist(c1, c2);

	bool freeMove = intern ? distance < c1->getRadius() - c2->ObtemRaio() : distance > c1->getRadius() + c2->ObtemRaio();
	
	if(!freeMove){
		canMove[0] = canMove[0] && (intern ? c2->ObtemX() > c1->getCenterX() : c2->ObtemX() < c1->getCenterX());
		canMove[1] = canMove[1] && (intern ? c2->ObtemY() > c1->getCenterY() : c2->ObtemY() < c1->getCenterY());
		canMove[2] = canMove[2] && (intern ? c2->ObtemX() < c1->getCenterX() : c2->ObtemX() > c1->getCenterX());
		canMove[3] = canMove[3] && (intern ? c2->ObtemY() < c1->getCenterY() : c2->ObtemY() > c1->getCenterY());

	}else{
		canMove[0] = canMove[0] || intern;
		canMove[1] = canMove[1] || intern;
		canMove[2] = canMove[2] || intern;
		canMove[3] = canMove[3] || intern;
	}
}

void checkCollisionJumpable(Circle *c1, Player *c2, int i){
	double distance = dist(c1, c2);

	if(c2->EstaAcima() && aboveI != i){
		return;
	}

	if(!player->EstaPulando() && !player->EstaAcima()){
		checkCollision(c1, c2);
	}else{
		if(distance < c1->getRadius() + c2->ObtemRaio()){
			player->DeterminaAcima(true);
			aboveI = i;
			if(player->EstaPulando()){
				canMove[0] = canMove[0] || ( c2->ObtemX() > c1->getCenterX());
				canMove[1] = canMove[1] || ( c2->ObtemY() > c1->getCenterY());
				canMove[2] = canMove[2] || ( c2->ObtemX() < c1->getCenterX());
				canMove[3] = canMove[3] || ( c2->ObtemY() < c1->getCenterY());
			} else {
				canMove[0] = canMove[0] || ( c2->ObtemX() > c1->getCenterX());
				canMove[1] = canMove[1] || ( c2->ObtemY() > c1->getCenterY());
				canMove[2] = canMove[2] || ( c2->ObtemX() < c1->getCenterX());
				canMove[3] = canMove[3] || ( c2->ObtemY() < c1->getCenterY());
			}

		} else {
			player->DeterminaAcima(false);
			aboveI = -1;
		}
	}

}

void idle(void){
	checkCollision(arena[0], player, true);
	checkCollision(arena[1], player);
	for (list<Circle*>::iterator iter = highObstacles.begin(); iter != highObstacles.end(); ++iter)
	{
		checkCollision((*iter), player);
	}
	 int i = 0;
	for (list<Circle*>::iterator iter = lowObstacles.begin(); iter != lowObstacles.end(); ++iter, ++i)
	{
		checkCollisionJumpable((*iter), player, i);
	}
	
	// checkJump();

	// if(keyFlags['a']){
	// 	if(canMove[0]){
	// 		player->addCenterX(-delta);
	// 	}
	// }
	// if(keyFlags['s']){
	// 	if(canMove[1]){
	// 		player->addCenterY(-delta);
	// 	}
	// }
	// if(keyFlags['d']){
	// 	if(canMove[2]){
	// 		player->addCenterX(+delta);
	// 	}
	// }
	// if(keyFlags['w']){
	// 	if(canMove[3]){
	// 		player->addCenterY(+delta);
	// 	}
	// }
	// if(keyFlags['p']){
	// 	if(!inJump){
	// 		inJump = true;
	// 		jumpInitTime = glutGet(GLUT_ELAPSED_TIME);;
	// 	}
	// }
	//Treat keyPress
    if(keyFlags[(int)('w')])
    {
        player->MoveEmY(velJogador, canMove);
    }
    if(keyFlags[(int)('s')])
    {
        player->MoveEmY(-velJogador, canMove);
    }
    if(keyFlags[(int)('a')])
    {
        player->RodaPlayer(velJogador);
    }
    if(keyFlags[(int)('d')])
    {
        player->RodaPlayer(-velJogador);
    }
    if(keyFlags[(int)('p')])
    {
         player->Pula();
    }
	glutPostRedisplay();
}

int main(int argc, char** argv){
	readParams(argv[1]);
	argc--;
	argv = &argv[1];
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE |	GLUT_RGB);
	glutInitWindowSize (window->getWidth(), window->getHeight());
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("trabalhocg");
	initWindow();
	glutPassiveMotionFunc(passiveMotion);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
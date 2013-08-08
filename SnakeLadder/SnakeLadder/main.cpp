#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <GL/glut.h>
//#include "glm.h"

//Function definitions
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void specialUpKey(int key, int x, int y);
void drawCube(int number, float r, float g, float b, bool renderChar);
void drawBoard();
void drawPieces();

float xangle = 0.0;
float yangle = 0.0;

bool SLANTED_BOARD = true;
const int NUM_TILES = 100;
const int NUM_ROW = 10;

int plOneRow = 1;
int plOneCol = 1;
int plTwoRow = 1;
int plTwoCol = 1;

float plOneX = -14.0;
float plOneY = 0.0;		//slanted default y
float plOneZ = -11.5;	//slanted default z

float plTwoX = -12.5;
float plTwoY = 0.0;		//slanted default y
float plTwoZ = -11.5;	//slanted default z

const float pieceInc = 3.3f;

int main(int argc, char** argv){

	//Glut initilization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 360);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Snakes and Ladders");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	//Input configuration
	//glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKey);
	//glutSpecialUpFunc(specialUpKey);
	
	glutMainLoop();
	return 0;      
}

void init(){
	//Sets the clear color to white
	glClearColor(0.6,0.8,1.0,0.0);	//WHITE

	//Enables lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Enables z-depth
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

void reshape (int w, int h){
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

void display(){

	//Lighting position
	GLfloat position[] = {0.0, 0.0, 1.5, 1.0};
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		//Translates camera towards to user
		glTranslatef(0.0, 0.0, -5.0);

		//Sets lighting properties
		glPushMatrix();
			glLightfv(GL_LIGHT0, GL_POSITION, position);
			glTranslated(0.0, 0.0, 6);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glRotatef(xangle, 0.0, 1.0, 0.0);
		glRotatef(yangle, 1.0, 0.0, 0.0);

		drawBoard();
		drawPieces();

	glPopMatrix();
	
	//Forces rendering to the screen
	glutSwapBuffers();
}

void drawPieces(){
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
		glColor3f(0.8, 0.1, 0.1);
		glScalef(0.075, 0.075, 0.075);
		if(SLANTED_BOARD){
			glRotatef(270.0, 1.0, 0.0, 0.0);
			glTranslatef(plOneX, plOneY, plOneZ);
		}else{
			glTranslatef(plOneX, -14.0, 1.75);
		}
		glutSolidCone(1.0, 2.0, 10.0, 5.0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.1, 0.1, 0.8);
		glScalef(0.075, 0.075, 0.075);
		if(SLANTED_BOARD){
			glRotatef(270.0, 1.0, 0.0, 0.0);
			glTranslatef(plTwoX, plTwoY, plTwoZ);
		}else{
			glTranslatef(plTwoX, -14.0, 1.75);
		}
		glutSolidCone(1.0, 2.0, 10.0, 5.0);
	glPopMatrix();
}

void drawBoard(){
	glDisable(GL_LIGHTING);

	glPushMatrix();			
			//location of FIRST CUBE
			glTranslatef(-1.0, -1.0, 0.0);
			glScalef(0.25, 0.25, 0.25);
			bool right = true;

			for(int i = 1; i <= NUM_TILES; i++){
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				drawCube(i, 0.7, 0.5, 0.1, false);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glEnable(GL_POLYGON_OFFSET_LINE);
				glPolygonOffset(-1.f, -1.f);
				drawCube(i, 0.0, 0.0, 0.0, true);
				glDisable(GL_POLYGON_OFFSET_LINE);

				if(right){
					glTranslatef(1.0, 0.0, 0.0);
				}else{
					glTranslatef(-1.0, 0.0, 0.0);
				}
				if(i % NUM_ROW == 0){
					if(right){
						right = false;
						glTranslatef(-1.0, 0.0, 0.0);
					}else{
						right = true;
						glTranslatef(1.0, 0.0, 0.0);
					}
					if(SLANTED_BOARD){
						glTranslatef(0.0, 1.0, -1.0);
					}else{
						glTranslatef(0.0, 1.0, 0.0);
					}
				}
			}
		glPopMatrix();
}

void drawChars(int number){
	float textOffset = 0.01f;
	glColor3f(1.0, 1.0, 1.0);

	std::stringstream ss;
	ss << number;
	std::string str = ss.str();

	//FRONT
	glPushMatrix();
		glTranslatef(-0.25, -0.25, 0.5 + textOffset);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		glScalef(0.5, 0.5, 0.5);
		for(std::string::size_type i = 0; i < str.length(); ++i){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();

	//BACK
	glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(-0.25, -0.25, 0.5 + textOffset);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		glScalef(0.5, 0.5, 0.5);
		for(std::string::size_type i = 0; i < str.length(); ++i){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();

	//LEFT
	glPushMatrix();
		glRotatef(270.0, 0.0, 1.0, 0.0);
		glTranslatef(-0.25, -0.25, 0.5 + textOffset);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		glScalef(0.5, 0.5, 0.5);
		for(std::string::size_type i = 0; i < str.length(); ++i){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();

	//RIGHT
	glPushMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glTranslatef(-0.25, -0.25, 0.5 + textOffset);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		glScalef(0.5, 0.5, 0.5);
		for(std::string::size_type i = 0; i < str.length(); ++i){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();

	//TOP
	glPushMatrix();
		glRotatef(270.0, 1.0, .0, 0.0);
		glTranslatef(-0.25, -0.25, 0.5 + textOffset);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		glScalef(0.5, 0.5, 0.5);
		for(std::string::size_type i = 0; i < str.length(); ++i){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();

	//BOTTOM
	glPushMatrix();
		glRotatef(90.0, 1.0, .0, 0.0);
		glTranslatef(-0.25, -0.25, 0.5 + textOffset);
		glScalef(1/152.38, 1/152.38, 1/152.38);
		glScalef(0.5, 0.5, 0.5);
		for(std::string::size_type i = 0; i < str.length(); ++i){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
		}
	glPopMatrix();
}

void drawCube(int number, float r, float g, float b, bool renderChar)
{
    glBegin(GL_QUADS);
	glColor3f(r, g, b);

    // FRONT
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f( 0.5f, -0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // BACK
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);

    // LEFT
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // RIGHT
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, -0.5f, 0.5f);

    // TOP
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // BOTTOM
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, 0.5f);

    glEnd();

	if(renderChar){
		drawChars(number);
	}
}

void keyboard(unsigned char key, int x, int y){

	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y){

	switch(key){
		case 0x72: //'r'
			std::cout << "b plOne movement test" << std::endl;

			if(plOneCol % 10 == 0){
				plOneRow++;
				plOneY+= pieceInc;
				plOneZ+= pieceInc;
			}

			if(plOneRow % 2 == 1){
				plOneX+= pieceInc;	
				plOneCol++;
			}else{
				if(plOneCol == 1){
					plOneX+= pieceInc;	
					plOneRow++;
					plOneCol++;
				}else{
					plOneX-= pieceInc;
					plOneCol--;
				}
			}

			std::cout << "plOneCol : " << plOneCol << std::endl;
			std::cout << "plOneRow : " << plOneRow << std::endl;
			std::cout << "plOneX : " << plOneX << std::endl;
			std::cout << "plOneY : " << plOneY << std::endl;
			std::cout << "plOneZ : " << plOneZ << std::endl;
			break;
		//case 0x62:	//'b'
		//	std::cout << "r plTwo movement test" << std::endl;

		//	if(plTwoCol % 10 == 0){
		//		plTwoRow++;
		//	}

		//	if(plTwoRow % 2 == 1){
		//		plTwoX+= xPieceInc;	
		//		plTwoCol++;
		//	}else{
		//		if(plTwoCol == 1){
		//			plTwoX+= xPieceInc;	
		//			plTwoRow++;
		//			plTwoCol++;
		//		}else{
		//			plTwoX-= xPieceInc;
		//			plTwoCol--;
		//		}
		//	}

		//	std::cout << "plTwoCol : " << plTwoCol << std::endl;
		//	std::cout << "plTwoRow : " << plTwoRow << std::endl;
		//	std::cout << "plTwoX : " << plTwoX << std::endl;
		//	std::cout << "plTwoY : " << plTwoY << std::endl;
		//	std::cout << "plTwoZ : " << plTwoZ << std::endl;
		//	break;
		case 0x2C:
			std::cout << ", reset rotation angles" << std::endl;
			xangle = 0;
			yangle = 0;
			break;
		case 0x2E:
			std::cout << ". change board type" << std::endl;
			if(SLANTED_BOARD){
				SLANTED_BOARD = false;
			}else{
				SLANTED_BOARD = true;
			}
			break;
	}

	glutPostRedisplay();
}

void specialKey(int key, int x, int y){
	
	switch(key){
	case GLUT_KEY_LEFT:
		xangle+=5.0;
		break;
	case GLUT_KEY_RIGHT:
		xangle-=5.0;
		break;
	case GLUT_KEY_UP:
		yangle-=5.0;
		break;
	case GLUT_KEY_DOWN:
		yangle+=5.0;
		break;
	}

	glutPostRedisplay();		
}

void specialUpKey(int key, int x, int y){
	
	glutPostRedisplay();		
}
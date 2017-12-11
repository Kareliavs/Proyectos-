#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
#include <ctime>
#include <chrono>

using namespace std;

const float PI = 3.14, DEPTH = 6;
// Start and end of camera movement
const int ENDX = 10, STARTX = -500;
// Angle of branches, and depth of tree
float ANGLE = 40, depth = 2;
vector<string> *trees = new vector<string>();

//compila asi  g++ -std=c++11 L-System.cpp -lglut -lGL -lGLU -o b
double lastTime = 0, elapsedTime = 0, lastElapsedTime = 0;
float length = 0.001, num = 0, incr = 0.1;
bool poda=false;
float incremento = 0.0005;
float lineWidth = 5;
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)
///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{ 
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F1:
		if(depth<DEPTH){
			depth++;
			incremento=incremento/1000;
			cout<<"Aumento de profundidad"<<endl;
			}
		else cout<<"Profundidad limite"<<endl;
		break;
	case GLUT_KEY_F2:
		poda=!poda;
		cout<<"poda"<<poda<<endl;
		break;

	}
}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	
	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////
//(4)
///////////////////////////////////////////////////////////////////////////////
GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(1);
		break;
	default:
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// L-System
string str = "X";

void push(){
	glPushMatrix();
	if (lineWidth > 0)
		lineWidth -= 1;
}

void pop(){
	glPopMatrix();
	lineWidth += 1;

}

void rotL(){
	glRotatef(ANGLE, 1, 0, 0);
	glRotatef(ANGLE*4, 0, 1, 0);
	glRotatef(ANGLE, 0, 0, 1);
}
void rotR(){
	glRotatef(-ANGLE, 1, 0, 0);
	glRotatef(ANGLE*4, 0, 1, 0);
	glRotatef(-ANGLE, 0, 0, 1);
}
void leaf(){
	glPushAttrib(GL_LIGHTING_BIT);//guarda el material de iluminación actual
	//glColor3f(0.50, 1.0, 0.0);
	GLfloat ambient[4] = { 0.50, 1.0, 0.0 };    // reflexión ambiental
	GLfloat specular[4] = { 0.55, 1.0, 0.0 };   // reflexión especular
	GLfloat diffuse[4] = { 0.50, 0.9, 0.0 };   // reflexión difusa

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);    
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	// tamanho de reflejos especulares
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);

	glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(0.2, 0, 0.3);
		glVertex3f(0, 1, 0);
	glEnd();
	glPopAttrib();
}

void drawLine()
{
	glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff
			
	//glColor3f(0.55, 0.27, 0.07);
	GLfloat ambient[4] = {0.55, 0.09, 0.07};     // reflexión ambiental
	GLfloat specular[4] = {0.55, 0.27, 0.07};   // reflexión especular
	GLfloat diffuse[4] = {0.55, 0.27, 0.07};   // reflexión difusa
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glLineWidth(lineWidth);

	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, length, 0);
	glEnd();

	glTranslatef(0, length, 0);
	glPopAttrib();
}


void draw(){
	
	string ch = "";
	string LSystem = trees->at(depth);
	//cout<<"Regla: "<<LSystem<<endl;
	for (int i = 0; i < LSystem.length(); i++){
		ch = LSystem.at(i);

		if (ch.compare("D") == 0 || ch.compare("X") == 0){
			if(!poda)
			drawLine();
		} else if (ch.compare("[") == 0){
			push();
		} else if (ch.compare("]") == 0){
			pop();
		} else if (ch.compare("V") == 0){
			leaf();
		} else if (ch.compare("R") == 0){
			rotR();
		} else if (ch.compare("L") == 0){
			rotL();
		}
	}
}

void expand(float num){
	string ch = "";

	for (int i = 0; i < str.length(); i++){
		ch = str.at(i);
		//cout<<"ICI "<<ch<<endl;
		if (ch.compare("D") == 0){
			str.replace(i, 1, "DD");
			i = i + 1;
		} else if (ch.compare("X") == 0){
			
			if (num < 0.4){
				//cout<<"HERE "<<ch<<endl;
				//LSystem.replace(i, 1, "D[LX]D[RX]LX");
			str.replace(i, 1, "D[LXV]D[RXV]LX");

			} else {
				//LSystem.replace(i, 1, "D[RX]D[LX]RX");
				//cout<<"HERE BIGGER "<<ch<<endl;
				str.replace(i, 1, "D[RXV]D[LXV]RX");
			}
			i = i + 13;	//11
		} 

	} 
	trees->push_back(str);
}




void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f, 1.0f, 0.01f, 1000.0f);


	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);

	glPushMatrix();
	
	glPushAttrib(GL_LIGHTING_BIT); 
	GLfloat ambient[4] = { 0.82, 0.41, 0.12 };   // reflexion ambiental
	GLfloat diffuse[4] = { 0.82, 0.41, 0.12};   // reflexion difusa  
	// reflexion ambiental para el objeto
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	// reflexion difusa para el objeto
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	glBegin(GL_QUADS);
		glVertex3f(-500,0,-500);
		glVertex3f(-500, 0,500);	
		glVertex3f(500,0,500);
		glVertex3f(500,0,-500);
	glEnd();

	glPopMatrix();
	glPopAttrib();
	draw();
	glutSwapBuffers();
	glutPostRedisplay();
}
void animate(){
	if (lastTime == 0)
		lastTime = time(nullptr);
	
	elapsedTime = time(nullptr)-lastTime;

	// cambiar el angulo por el viento
	float numR = (float) rand()/RAND_MAX;
	

	if (ANGLE > 21.5){
		if (numR < 0.5){
			incr = -0.15;
		} else {
			incr = -0.1;
		}
	} else if (ANGLE < 18.5){ 
		if (numR > 0.5){ 
			incr = 0.15;
		} else {
			incr = 0.1;
		}
	}
	ANGLE += incr;

	if (depth < DEPTH)
		length += incremento;
	//cout<<"THIS> "<<elapsedTime-lastElapsedTime<<" "<<depth<<endl;
	if(elapsedTime-lastElapsedTime > 2000 && depth < DEPTH){
		depth++;
		lastElapsedTime = elapsedTime;
		incremento=incremento/1000;
		cout <<"a ";

	}
	elapsedTime = elapsedTime/5000;
	float t = (sin((elapsedTime*PI-PI/2))+1)/2;
	float p = (1-t)*STARTX + t*ENDX;
	
	/*if (cam)
		eyeX = p;*/
	glutPostRedisplay();

}

void light()
{

	// configurar la iluminacion
	glShadeModel(GL_SMOOTH);
	GLfloat lightP[4] = {0.0, 800.0, 0.0,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightP);

	// luz ambiental   
	GLfloat lightA[4] = {0.0,0.9,0.9,1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightA);

	// luz especular
	GLfloat lightS[4] = {0.9,0.9,0.9,1.0};
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightS);

	// luz difusa
	GLfloat lightD[4] = {0.9,0.9,0.9,1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightD);       

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);  
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 800); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("L-System");

	srand (time(NULL));
	num = (float) rand()/RAND_MAX;

	light();

	

	// Usar el buffering de profundidad para eliminar la superficie oculta. 
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);///draw

	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);
	glutKeyboardFunc(&window_key);
	glutSpecialFunc(&callback_special);

	glutIdleFunc(animate);
	for(int i = 0; i <= DEPTH; i++){
		expand(num);
	}
//	for(int i=0;i<trees->size();i++)cout<<"Element: "<<trees->at(i)<<endl;
//		cout<<"FIN"<<endl;
	glutMainLoop();
	return 0;
}

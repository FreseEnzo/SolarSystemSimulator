//Simulador de Sistema Solar utilizando C++ e openGL

//Bibliotecas
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#pragma comment(lib, "glew32.lib")

using namespace std;

//Classe planeta
class Planet{
public:

	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni,camOrbit;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni){
		radius = _radius;
		distance = _distance;
		orbit = _orbit;
		orbitSpeed = _orbitSpeed;
		axisTilt = _axisTilt;
		axisAni = _axisAni;
		
	}
	//Função que desenha a orbita da lua
	void drawSmallOrbit(void){
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distance, 100.0, 100.0);
		glPopMatrix();
	}
	//Função que desenha a lua
	void drawMoon(void){
		GLUquadricObj *quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(distance, 0.0, 0.0);
		gluSphere(quadric, radius, 20.0, 20.0);
		glPopMatrix();
	}

};

//Sol Planetas e Estrelas
Planet sun(5.0, 0, 0, 0, 0, 0);				//Sol
Planet mer(1.0, 7, 0, 4.74, 02.11, 0);		//Mercurio
Planet ven(1.5, 11, 0, 3.50, 177.0, 0);		//Venus
Planet ear(2.0, 16, 0, 2.98, 23.44, 0);		//Terra
Planet mar(1.2, 21, 0, 2.41, 25.00, 0);		//Marte
Planet jup(3.5, 28, 0, 1.31, 03.13, 0);		//Jupiter
Planet sat(3.0, 37, 0, 0.97, 26.70, 0);		//Saturno
Planet ura(2.5, 45.5, 0, 0.68, 97.77, 0);	//Urano
Planet nep(2.3, 53.6, 0, 0.54, 28.32, 0);	//Netuno
Planet lun(.40, 3, 0, 5.40, 0, 0);			//Lua      (Terra)
Planet pho(.20, 1.8, 0, 2.30, 0, 0);		//Phobos   (Marte)
Planet dei(.24, 2.4, 0, 3.60, 0, 0);		//Deimos   (Marte)
Planet eur(.24, 4, 0, 4.40, 0, 0);			//Europa   (Jupiter)
Planet gan(.24, 4.7, 0, 5.00, 0, 0);		//Ganymede (Jupiter)
Planet cal(.24, 5.3, 0, 2.30, 0, 0);		//Callisto (Jupiter)
Planet tit(.75, 3.7, 0, 2.40, 0, 0);		//Titan	   (Saturno)
Planet puc(.26, 2.9, 0, 7.00, 0, 0);		//Puck	   (Urano)
Planet tri(.36, 3.2, 0, 3.40, 0, 0);		//Triton   (Netuno)

int isAnimate = 0;
int bigOrbitActive = 1;
int smallOrbitActive = 1;
int moonsActive = 1;
int changeCamera = 0;
int changeIndividualCamera = 0;
int frameCount = 0;
int labelsActive = 0;
int zoom = 50;
int logoScene = 1;

//Posição do Spotlight logo acima do Sol
float lightPos[] = { 0.0, 0.0, -75.0, 1.0 };

//Texture mapping na esfera
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, staTexture, logTexture;

//Bitmap para o nome dos planetas
void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void setup(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//Import das texturas
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* sun = loadBMP("sol.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercurio.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("terra.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("marte.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturno.bmp");	satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("urano.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("netuno.bmp");  	nepTexture = loadTexture(nep);		delete nep;

	//Setup da iluminação
	glEnable(GL_LIGHTING);
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

//Funçao que desenha as orbitas
void orbitalTrails(void){
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distance, 100.0, 100.0);
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glutWireTorus(0.001, mar.distance, 100.0, 100.0);
	glutWireTorus(0.001, jup.distance, 100.0, 100.0);
	glutWireTorus(0.001, sat.distance, 100.0, 100.0);
	glutWireTorus(0.001, ura.distance, 100.0, 100.0);
	glutWireTorus(0.001, nep.distance, 100.0, 100.0);
	glPopMatrix();
}

//Desenha cena
void drawScene(void){
	frameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

//Switch para troca de cameras individuais
	float dist = 0, angle = 0, pi=3.1415;

	if (changeIndividualCamera > 9)
	{
		changeIndividualCamera = 1;
	}
	
	switch (changeIndividualCamera){
	case 0://Sol
		
	case 1://Mercurio
		angle = mer.orbit;
		dist = mer.distance + 4.4;
		break;
	case 2://Venus
		angle = ven.orbit;
		dist = ven.distance + 8;
		break;
	case 3://Terra
		angle = ear.orbit;
		dist = ear.distance + 8;
		break;
	case 4://Marte
		angle = mar.orbit;
		dist = mar.distance + 7.5;
		break;
	case 5://Jupiter
		angle = jup.orbit;
		dist = jup.distance + 11;
		break;
	case 6://Saturno
		angle = sat.orbit;
		dist = sat.distance + 12;
		break;
	case 7://Urano
		angle = ura.orbit;
		dist = ura.distance + 8;
		break;
	case 8://Netuno
		angle = nep.orbit;
		dist = nep.distance + 9;
		break;
	case 9://Sol
		angle = mer.orbit + 7;
		dist = mer.distance +2.6;
		break;
	};
	
	//Conversao das coordenadas esfericas para espaciais
	float camx, camy;
	camx = dist* cos(angle * 2 * pi / 360); 
	camy = dist * sin(angle * 2*pi / 360);

	if (changeCamera == 0)gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 2)gluLookAt(0.0, zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeIndividualCamera > 0)gluLookAt(camx, 3, -camy, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric *quadric;
	quadric = gluNewQuadric();

	//Sol
	glPushMatrix();
	glRotatef(sun.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(-1.2, 7.0, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Sol");
	}
	glPushMatrix();
	glRotatef(sun.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sun.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sun.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Mercurio
	glPushMatrix();
	glRotatef(mer.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mer.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Mercurio");
	}
	glPushMatrix();
	glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Venus
	glPushMatrix();
	glRotatef(ven.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ven.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Venus");
	}
	glPushMatrix();
	glRotatef(ven.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ven.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ven.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Terra e Lua
	glPushMatrix();
	glRotatef(ear.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Terra");
	}
	glPushMatrix();
	glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1){
		lun.drawSmallOrbit();
	}
	if (moonsActive == 1){
		lun.drawMoon();
	}
	glPopMatrix();

	//Marte e Luas
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mar.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Marte");
	}
	glPushMatrix();
	glRotatef(mar.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mar.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mar.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1){
		pho.drawSmallOrbit();
		dei.drawSmallOrbit();
	}
	if (moonsActive == 1){
		pho.drawMoon();
		dei.drawMoon();
	}
	glPopMatrix();

	//Jupiter e Luas
	glPushMatrix();
	glRotatef(jup.orbit, 0.0, 1.0, 0.0);
	glTranslatef(jup.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Jupiter");
	}
	glPushMatrix();
	glRotatef(jup.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(jup.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, jup.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1){
		eur.drawSmallOrbit();
		gan.drawSmallOrbit();
		cal.drawSmallOrbit();
	}
	if (moonsActive == 1){
		eur.drawMoon();
		gan.drawMoon();
		cal.drawMoon();
	}
	glPopMatrix();

	//Saturno e Lua
	glPushMatrix();
	glRotatef(sat.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sat.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Saturno");
	}
	glPushMatrix();
	glRotatef(sat.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sat.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sat.radius, 20.0, 20.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 30.0, 30.0);
	glutWireTorus(0.4, 5.0, 30.0, 30.0);
	glPopMatrix();
	if (smallOrbitActive == 1){
		tit.drawSmallOrbit();
	}
	if (moonsActive == 1){
		tit.drawMoon();
	}
	glPopMatrix();

	glColor3ub(255, 255, 255);		//FIXES SHADING ISSUE

	//Urano e Lua
	glPushMatrix();
	glRotatef(ura.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ura.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Urano");
	}
	glPushMatrix();
	glRotatef(ura.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ura.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ura.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1){
		puc.drawSmallOrbit();
	}
	if (moonsActive == 1){
		puc.drawMoon();
	}
	glPopMatrix();

	//Netuno e Lua
	glPushMatrix();
	glRotatef(nep.orbit, 0.0, 1.0, 0.0);
	glTranslatef(nep.distance, 0.0, 0.0);
	if (labelsActive == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Netuno");
	}
	glPushMatrix();
	glRotatef(nep.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(nep.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, nep.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1){
		tri.drawSmallOrbit();
	}
	if (moonsActive == 1){
		tri.drawMoon();
	}
	glPopMatrix();

	glutSwapBuffers();
}


void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}
 
void animate(int n){
	//Adiciona a orbita a determinada velocidade a cada chamada de funçao (recursivo)
	if (isAnimate){
		mer.orbit += mer.orbitSpeed;
		ven.orbit += ven.orbitSpeed;
		ear.orbit += ear.orbitSpeed;
		mar.orbit += mar.orbitSpeed;
		jup.orbit += jup.orbitSpeed;
		sat.orbit += sat.orbitSpeed;
		ura.orbit += ura.orbitSpeed;
		nep.orbit += nep.orbitSpeed;
		lun.orbit += lun.orbitSpeed;
		pho.orbit += pho.orbitSpeed;
		dei.orbit += dei.orbitSpeed;
		eur.orbit += eur.orbitSpeed;
		gan.orbit += gan.orbitSpeed;
		cal.orbit += cal.orbitSpeed;
		tit.orbit += tit.orbitSpeed;
		puc.orbit += puc.orbitSpeed;
		tri.orbit += tri.orbitSpeed;
		ear.camOrbit = ear.orbit;
		if (mer, ven, ear, mar, jup, sat, ura, nep, lun, pho, dei, eur, gan, cal, tit, puc, tri.orbit > 360.0){
			mer, ven, ear, mar, jup, sat, ura, nep, lun, pho, dei, eur, gan, cal, tit, puc, tri.orbit -= 360.0;
		}
		//Giro em torno do eixo planetario
		mer.axisAni += 10.0;
		ven.axisAni += 10.0;
		ear.axisAni += 10.0;
		mar.axisAni += 10.0;
		jup.axisAni += 10.0;
		sat.axisAni += 10.0;
		ura.axisAni += 10.0;
		nep.axisAni += 10.0;
		if (mer, ven, ear, mar, jup, sat, ura, nep.axisAni > 360.0){
			mer, ven, ear, mar, jup, sat, ura, nep.axisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}

//Implementação de zoom com scroll do mouse

void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}
//Recebe os inputs do teclado
void keyInput(unsigned char key, int x, int y){
	switch (key){
	case 27: exit(0); break; // Esc fecha
	case 'S': if (isAnimate) isAnimate = 0; else { isAnimate = 1; animate(1); } break;// Inicia
	case 's': if (isAnimate) isAnimate = 0; else { isAnimate = 1; animate(1); } break;// Inicia
	case 'o': if (smallOrbitActive) smallOrbitActive = 0; else smallOrbitActive = 1; glutPostRedisplay(); break;//Hab orbita da lua
	case 'O': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break;//Hab orbita dos planetas
	case 'm': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;//Hab/Des luas
	case 'M': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;//Hab/Des luas
	case 'l': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;//Ativ/Des legenda
	case 'L': if (labelsActive) labelsActive = 0; else labelsActive = 1; glutPostRedisplay(); break;//Ativ/Des legenda
	case '1': changeCamera = 0; changeIndividualCamera = 0; glutPostRedisplay(); break;//Troca de cameras
	case '2': changeCamera = 1; changeIndividualCamera = 0; glutPostRedisplay(); break;//Troca de cameras
	case '3': changeCamera = 2; changeIndividualCamera = 0; glutPostRedisplay(); break;//Troca de cameras
	case ' ': changeIndividualCamera++; changeCamera = 3; glutPostRedisplay(); break;//Troca de cameras


	}
}

void intructions(void){
	cout << "(S/s) para iniciar a simulacao" << endl;
	cout << "(ESC) para fechar" << endl;
	cout << "(O) para mostrar a orbita dos planetas." << endl;
	cout << "(o) para mostrar a orbita das luas." << endl;
	cout << "(M/m) para mostrar luas." << endl;
	cout << "(L/l) para mostrar legendas" << endl;
	cout << "(1), (2) e (3) para mudar cameras." << endl;
	cout << "(ESPACO) para trocar o foco dos planetas." << endl;
	cout << "(Scroll do mouse) para dar zoom" << endl;
}

//Funçao main
int main(int argc, char **argv){
	//Instruçoes no terminal
	intructions();
	//Inicializaçao glut
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	//Configurando janela
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Sistema Solar");
	//Chama as funçoes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();
}
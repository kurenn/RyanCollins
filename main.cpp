using namespace std;

// Sección de inclusión de librerías
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include <stdlib.h>       // Librería necesaria para usar la función exit() que termina la ejecución del programa
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <fstream>  // libreria para el manejo de archivos
#include <assert.h> // libreria para verificar la existencia de un archivo
#include <iostream> // libreria para el manejo de cin y cout
#include <string.h> // libreria para el manejo de strcmp
#include "Render.h"

//Declaracion
#define VISTA 1
#define HOMBRO_DERECHO 2
#define CODO_DERECHO 3
#define HOMBRO_IZQUIERDO 4
#define CODO_IZQUIERDO 5
#define CADERA_DERECHA 6
#define RODILLA_DERECHA 7
#define CADERA_IZQUIERDA 8
#define RODILLA_IZQUIERDA 9
#define CABEZA 10
#define PARTES 23 //constante para partes del cuerpo
#define DATOS 6 //constante para partes del cuerpo
#define LIMITE 18 //limite de movimiento en elementos

typedef struct nodo
{ float m[16];          // para guardar la matriz de transformación local
	void (*f)();          // para guardar el apuntador a la función que dibuja
	float r, g, b;       // para guardar color
	struct nodo *sibling;  // para guardar el apuntador al primer hermano
	struct nodo *child;    // para guardar el apuntador al primer hijo
	float x, y, z;         //para guardar los valores de la traslacion
	float rotX, rotY, rotZ;  //para guardar los valores de la rotacion
	} nodo;

// Sección de declaración de constantes globales
	const float DELTA = 5;    // Valor para el incremento/decremento del ángulo

// Sección de declaración de variables globales
	float anguloX = 0.0;        // Variable para manejar el ángulo de rotación a aplicar en X;
	float anguloY = 0.0;        // Variable para manejar el ángulo de rotación a aplicar en Y;
	float anguloZ = 0.0;		//Variable para manjer el angulo de rotacion a aplicar en Z;
	nodo elementos[24];
	int vista = 1;
	int hombro_derecho = 0;
	int codo_derecho = 0;
	int hombro_izquierdo = 0;
	int codo_izquierdo = 0;
	int cadera_derecha = 0;
	int rodilla_derecha = 0;
	int cadera_izquierda = 0;
	int rodilla_izquierda = 0;
	int cabeza_rot = 0;

//Limites para los movimientos (grados de libertad)
	int limiteUpDo_hombro_derecho = 0;
	int limiteUpDo_codo_derecho = 0;
	int limiteUpDo_hombro_izquierdo = 0;
	int limiteUpDo_codo_izquierdo = 0;
	int limiteUpDo_cadera_derecha = 0;
	int limiteUpDo_rodilla_derecha = 0;
	int limiteUpDo_cadera_izquierda = 0;
	int limiteUpDo_rodilla_izquierda = 0;
	int limiteUpDo_cabeza = 0;

	int limiteLeRi_hombro_derecho = 0;
	int limiteLeRi_codo_derecho = 0;
	int limiteLeRi_hombro_izquierdo = 0;
	int limiteLeRi_codo_izquierdo = 0;
	int limiteLeRi_cadera_derecha = 0;
	int limiteLeRi_rodilla_derecha = 0;
	int limiteLeRi_cadera_izquierda = 0;
	int limiteLeRi_rodilla_izquierda = 0;
	int limiteLeRi_cabeza = 0;

//Limites animacion
	int piernaDerecha = 0;
	int piernaIzquierda = 0;
	int brazoDerecho = 0;
	int brazoIzquierdo = 0;
	int brazos = 0;
	int paso = 1;   //para cambiar movimiento de la animacion

//Valida la ejecucion de la animacion del mono
	int ejecuta_animacion = -1;

	GLfloat pi180=3.14159265358979323846/180;
	GLdouble angulo = 1*pi180;
	GLfloat angx, angz, slice = 360;

	GLuint	texture[6];
	GLdouble x,z;
#define DEGREES_TO_RADIANS 3.14159/180.0

	double ang_x, ang_y, ang_z; 

	float mcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float light0[] = { 1.0f, 1.0f, 1.0f, 1.0f  };
	float light0_pos[] = { 0.0f, 1.0f,0.0f, 1.0f };
	static float ypoz = 0, zpoz = 0;

// Empieza la declaración de métodos y funciones

	void aplacaBrazos() {
		elementos[1].rotZ -= DELTA;
		elementos[6].rotZ += DELTA;
		brazos++;
	}

//Administra el movimiento del caballo
	void animaHorseman() {

		if (brazos < LIMITE) {
			aplacaBrazos();
		}

		else {

				//movimiento de las piernas
			if(piernaDerecha > -(LIMITE-8) && paso == -1) {
				elementos[11].rotX += DELTA;
				piernaDerecha--;
			}
			else if(piernaDerecha < (LIMITE-8) && paso == 1) {
				elementos[11].rotX -= DELTA;
				piernaDerecha++;
			}

			if(piernaIzquierda < (LIMITE-8) && paso == -1) {
				elementos[16].rotX -= DELTA;
				piernaIzquierda++;
			}

			else if(piernaIzquierda > -(LIMITE-8) && paso == 1) {
				elementos[16].rotX += DELTA;
				piernaIzquierda--;
			}

				//Delimita el cambio de movimiento en los pies
			if (piernaDerecha == (LIMITE-8))
				paso = -1;

			else if (piernaIzquierda == (LIMITE-8))
				paso = 1;

			//movimiento de los brazos
			if(brazoDerecho > -(LIMITE-8) && paso == -1) {
				elementos[1].rotX -= DELTA;
				brazoDerecho--;
			}
			else if(brazoDerecho < (LIMITE-8) && paso == 1) {
				elementos[1].rotX += DELTA;
				brazoDerecho++;
			}

			if(brazoIzquierdo < (LIMITE-8) && paso == -1) {
				elementos[6].rotX += DELTA;
				brazoIzquierdo++;
			}

			else if(brazoIzquierdo > -(LIMITE-8) && paso == 1) {
				elementos[6].rotX -= DELTA;
				brazoIzquierdo--;
			}
		}
	}

// funcion para el manejo de la animacion a traves de timer
	void animationTimer(int valor){

    if (ejecuta_animacion == 1) {
		animaHorseman();
	    glutTimerFunc(1500/24,animationTimer,1);
	}else{
		 glutTimerFunc(1500/24,animationTimer,1);
	}

	/* obligar a dibujar */
		glutPostRedisplay();
	}

	void processMenuEvents(int opcion){
		switch (opcion){
			case 1:
			vista = 1;
			hombro_derecho = codo_derecho = hombro_izquierdo = codo_izquierdo = cadera_derecha = cabeza_rot = rodilla_derecha = cadera_izquierda = rodilla_izquierda = 0;
			break;
			case 2:
			hombro_derecho = 1;
			vista = codo_derecho = hombro_izquierdo = codo_izquierdo = cadera_derecha = rodilla_derecha = cabeza_rot = cadera_izquierda = rodilla_izquierda = 0;
			break;
			case 3:
			codo_derecho = 1;
			hombro_derecho = vista = hombro_izquierdo = codo_izquierdo = cadera_derecha = cabeza_rot = rodilla_derecha = cadera_izquierda = rodilla_izquierda = 0;
			break;

			case 4:
			hombro_izquierdo = 1;
			hombro_derecho = codo_derecho = vista = codo_izquierdo = cadera_derecha = cabeza_rot = rodilla_derecha = cadera_izquierda = rodilla_izquierda = 0;
			break;

			case 5:
			codo_izquierdo = 1;
			hombro_derecho = codo_derecho = hombro_izquierdo = vista = cadera_derecha = cabeza_rot = rodilla_derecha = cadera_izquierda = rodilla_izquierda = 0;
			break;

			case 6:
			cadera_derecha = 1;
			hombro_derecho = codo_derecho = hombro_izquierdo = codo_izquierdo = vista = cabeza_rot = rodilla_derecha = cadera_izquierda = rodilla_izquierda = 0;
			break;

			case 7:
			rodilla_derecha = 1;
			hombro_derecho = codo_derecho = hombro_izquierdo = codo_izquierdo = cabeza_rot = cadera_derecha = vista = cadera_izquierda = rodilla_izquierda = 0;
			break;

			case 8:
			cadera_izquierda = 1;
			hombro_derecho = codo_derecho = hombro_izquierdo = cabeza_rot = codo_izquierdo = cadera_derecha = rodilla_derecha = vista = rodilla_izquierda = 0;
			break;

			case 9:
			rodilla_izquierda = 1;
			hombro_derecho = codo_derecho = hombro_izquierdo = cabeza_rot = codo_izquierdo = cadera_derecha = rodilla_derecha = cadera_izquierda = vista = 0;
			break;

			case 10:
			cabeza_rot = 1;
			hombro_derecho = codo_derecho = rodilla_izquierda = hombro_izquierdo = codo_izquierdo = cadera_derecha = rodilla_derecha = cadera_izquierda = vista = 0;
			break;

		}
	}

	void fondo(){
		glBindTexture(GL_TEXTURE_2D,texture[2]); // Se mapea la textura actual
		glTranslatef(-15, -15, -15);
		glBegin( GL_QUADS );
		glTexCoord2f(1.0f,1.0f); glVertex3f( 30, 0, 0  );
		glTexCoord2f(1.0f,0.0f); glVertex3f( 30, 30, 0 );
		glTexCoord2f(0.0f,0.0f); glVertex3f( 30, 30, 30);
		glTexCoord2f(0.0f,1.0f); glVertex3f( 30, 0, 30 );
		glEnd();

		glBindTexture(GL_TEXTURE_2D,texture[3]); // Se mapea la textura actual
		glBegin( GL_QUADS )   ;
		glTexCoord2f(1.0f,1.0f); glVertex3f( 30, 30, 0 );
		glTexCoord2f(1.0f,0.0f); glVertex3f( 0, 30, 0  );
		glTexCoord2f(0.0f,0.0f); glVertex3f( 0, 30, 30 );
		glTexCoord2f(0.0f,1.0f); glVertex3f( 30, 30, 30);
		glEnd();

		glBindTexture(GL_TEXTURE_2D,texture[2]); // Se mapea la textura actual
		glBegin( GL_QUADS )   ;
		glTexCoord2f(1.0f,1.0f); glVertex3f( 0, 0, 30  );
		glTexCoord2f(1.0f,0.0f); glVertex3f( 0, 30, 30 );
		glTexCoord2f(0.0f,0.0f); glVertex3f( 30, 30, 30);
		glTexCoord2f(0.0f,1.0f); glVertex3f( 30, 0, 30 );
		glEnd();

		glBindTexture(GL_TEXTURE_2D,texture[2]); // Se mapea la textura actual
		glBegin( GL_QUADS )   ;
		glTexCoord2f(1.0f,1.0f); glVertex3f( 0, 0, 30  );
		glTexCoord2f(1.0f,0.0f); glVertex3f( 0, 30, 30 );
		glTexCoord2f(0.0f,0.0f); glVertex3f( 0, 30, 0  );
		glTexCoord2f(0.0f,1.0f); glVertex3f( 0, 0, 0   );
		glEnd();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D,texture[4]); // Se mapea la textura actual
		glBegin( GL_QUADS )   ;
		glTexCoord2f(1.0f,1.0f); glVertex3f( 0, 0, 30  );
		glTexCoord2f(1.0f,0.0f); glVertex3f( 30, 0, 30 );
		glTexCoord2f(0.0f,0.0f); glVertex3f( 30, 0, 0  );
		glTexCoord2f(0.0f,1.0f); glVertex3f( 0, 0, 0   );
		glEnd();

		glBindTexture(GL_TEXTURE_2D,texture[2]); // Se mapea la textura actual
		glBegin( GL_QUADS )   ;
		glTexCoord2f(1.0f,1.0f); glVertex3f( 0, 0, 0   );
		glTexCoord2f(1.0f,0.0f); glVertex3f( 0, 30, 0  );
		glTexCoord2f(0.0f,0.0f); glVertex3f( 30, 30, 0 );
		glTexCoord2f(0.0f,1.0f); glVertex3f( 30, 0, 0  );
		glEnd();
	}

	void tronco(){
		glScalef(1.0, 2.0, 0.5);
		glutSolidCube(1);
		glScalef(1.0, 0.5, 2.0);
	}

	void hombro(){
		glutSolidSphere(0.2, 30,30);
	}

	void cadera(){
		glutSolidSphere(0.2, 30,30);
	}

	void bicep(){
		glRotatef(90,0,0,1);

		GLdouble vertices1 [ ] [3]= {
			{0.1,	-0.25,	0},
				{0.1,	-0.25,	0},
				{0.1,	 0.25,	0},
				{0.1,	 0.25,	0}
		};

		for (int i= 0; i<slice;i++)
		{
			glBegin(GL_POLYGON);
			for (int j= 1; j<3;j++)
			{
				x = vertices1[j][0]*cos(angulo)+vertices1[j][2]*sin(angulo);
				z =  vertices1[j][0]*sin(angulo)*-1.0+vertices1[j][2]*cos(angulo);
				if (j%2 == 0)
				{
					glVertex3d(x,vertices1[j][1],z);
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
				}
				else
				{
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
					glVertex3d(x,vertices1[j][1],z);
				}
			}
			glEnd();

			for (int j= 1; j<3;j++)
			{   // Primer poligono
				x = vertices1[j][0];
				z=vertices1[j][2];
				vertices1[j][0] = x*cos(angulo)+z*sin(angulo);
				vertices1[j][2] = x*sin(angulo)*-1.0+z*cos(angulo);
			}
		}
		glRotatef(-90,0,0,1);
	}

	void antebrazo(){
		glRotatef(90,0,0,1);

		GLdouble vertices1 [ ] [3]= {
			{0.1,	-0.25,	0},
				{0.1,	-0.25,	0},
				{0.1,	 0.25,	0},
				{0.1,	 0.25,	0}
		};

		for (int i= 0; i<slice;i++)
		{
			glBegin(GL_POLYGON);
			for (int j= 1; j<3;j++)
			{
				x = vertices1[j][0]*cos(angulo)+vertices1[j][2]*sin(angulo);
				z =  vertices1[j][0]*sin(angulo)*-1.0+vertices1[j][2]*cos(angulo);
				if (j%2 == 0)
				{
					glVertex3d(x,vertices1[j][1],z);
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
				}
				else
				{
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
					glVertex3d(x,vertices1[j][1],z);
				}
			}
			glEnd();

			for (int j= 1; j<3;j++)
			{   // Primer poligono
				x = vertices1[j][0];
				z=vertices1[j][2];
				vertices1[j][0] = x*cos(angulo)+z*sin(angulo);
				vertices1[j][2] = x*sin(angulo)*-1.0+z*cos(angulo);
			}
		}
		glRotatef(-90,0,0,1);
	}

	void codo(){
		glutSolidSphere(0.2, 30,30);
	}

	void mano(){
		glutSolidSphere(0.2, 30,30);
	}

	void quadricep(){

		GLdouble vertices1 [ ] [3]= {
			{0.12,	-0.4,	0},
				{0.12,	-0.4,	0},
				{0.12,	 0.4,	0},
				{0.12,	 0.4,	0}
		};

		for (int i= 0; i<slice;i++)
		{
			glBegin(GL_POLYGON);
			for (int j= 1; j<3;j++)
			{
				x = vertices1[j][0]*cos(angulo)+vertices1[j][2]*sin(angulo);
				z =  vertices1[j][0]*sin(angulo)*-1.0+vertices1[j][2]*cos(angulo);
				if (j%2 == 0)
				{
					glVertex3d(x,vertices1[j][1],z);
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
				}
				else
				{
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
					glVertex3d(x,vertices1[j][1],z);
				}
			}
			glEnd();

			for (int j= 1; j<3;j++)
			{   // Primer poligono
				x = vertices1[j][0];
				z=vertices1[j][2];
				vertices1[j][0] = x*cos(angulo)+z*sin(angulo);
				vertices1[j][2] = x*sin(angulo)*-1.0+z*cos(angulo);
			}
		}
	}

	void rodilla(){
		glutSolidSphere(0.2, 30,30);
	}

	void chamorro(){
		GLfloat pi180=3.14159265358979323846/180;                  // Factor de conversión de grados a radianes
		GLdouble angulo = 1*pi180;
		GLfloat angx, angz, slice = 360;               // Variables para manejo de rotaciones y cantidad de "rebanadas" de la copa
		GLdouble x,z;                              // Variables auxiliares para la generación de las x's y z's
																							// Las y's permanecen constantes puesto que el eje de rotación es y
		GLdouble vertices1 [ ] [3]= {
			{0.12,	-0.4,	0},
				{0.12,	-0.4,	0},
				{0.12,	 0.4,	0},
				{0.12,	 0.4,	0}
		};

		for (int i= 0; i<slice;i++)
		{
			glBegin(GL_POLYGON);
			for (int j= 1; j<3;j++)
			{
				x = vertices1[j][0]*cos(angulo)+vertices1[j][2]*sin(angulo);
				z =  vertices1[j][0]*sin(angulo)*-1.0+vertices1[j][2]*cos(angulo);
				if (j%2 == 0)
				{
					glVertex3d(x,vertices1[j][1],z);
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
				}
				else
				{
					glVertex3d(vertices1[j][0],vertices1[j][1],vertices1[j][2]);
					glVertex3d(x,vertices1[j][1],z);
				}
			}
			glEnd();

			for (int j= 1; j<3;j++)
			{   // Primer poligono
				x = vertices1[j][0];
				z=vertices1[j][2];
				vertices1[j][0] = x*cos(angulo)+z*sin(angulo);
				vertices1[j][2] = x*sin(angulo)*-1.0+z*cos(angulo);
			}
		}
	}

	/*
	*
	*/
	void cuello(){
		glutSolidSphere(0.25,30,30);
	}

	void pie(){
		glScalef(1.0, 0.5, 2.5);
		glutSolidCube(0.25);
		glScalef(1.0, 2.0 , 0.4);
	}

	void cabeza(){
		glRotatef(15,1,0,0);
		glScalef(1.0, 1.0, 2.5);
		glTranslatef(-0.25, -0.2, -0.25);

	//Perfil derecho
		glBindTexture(GL_TEXTURE_2D,texture[0]);
		glBegin( GL_QUADS );
		glTexCoord2f(0.8f,1.0f); glVertex3f( 0.5, 0, 0);
		glTexCoord2f(1.0f,0.0f); glVertex3f( 0.5, 0.5, 0);
		glTexCoord2f(0.0f,0.0f); glVertex3f( 0.5, 0.5, 0.5);
		glTexCoord2f(-0.15f,0.9f); glVertex3f( 0.5, 0, 0.5);
		glEnd();

			//Cara de arriba
		glBegin( GL_QUADS ); // lateral derecha
		glVertex3f( 0.5, 0.5, 0);
		glVertex3f( 0, 0.5, 0);
		glVertex3f( 0, 0.5, 0.5);
		glVertex3f( 0.5, 0.5, 0.5);
		glEnd();

			//Cara frontal
		glBegin( GL_QUADS ); // superior
		glVertex3f( 0, 0, 0.5);
		glVertex3f( 0, 0.5, 0.5);
		glVertex3f( 0.5, 0.5, 0.5);
		glVertex3f( 0.5, 0, 0.5);
		glEnd();

		//Perfil izquierdo
		glBindTexture(GL_TEXTURE_2D,texture[1]); // Se mapea la textura actual
		glBegin( GL_QUADS );
		glTexCoord2f(1.0f,1.0f); glVertex3f( 0, 0, 0.5);
		glTexCoord2f(0.9f,0.0f); glVertex3f( 0, 0.5, 0.5);
		glTexCoord2f(0.0f,0.0f); glVertex3f( 0, 0.5, 0);
		glTexCoord2f(0.0f,1.0f); glVertex3f( 0, 0, 0);
		glEnd();

		//Cara de abajo
		glBegin( GL_QUADS ); // lateral izquiera
		glVertex3f( 0, 0, 0.5);
		glVertex3f( 0.5, 0, 0.5);
		glVertex3f( 0.5, 0, 0);
		glVertex3f( 0, 0, 0);
		glEnd();

			//Cara trasera
		glBegin( GL_QUADS ); // inferior
		glVertex3f( 0, 0, 0);
		glVertex3f( 0, 0.5, 0);
		glVertex3f( 0.5, 0.5, 0);
		glVertex3f( 0.5, 0, 0);

		glEnd();

		glRotatef(-15,1,0,0);
		glScalef(1.0, 1.0 , 0.4);
	}

//Metodo para leer archivo
	void leerarch(){
		ifstream entrada;

		entrada.open("datos.txt");
		assert(entrada);
		double valor;     // Auxiliar para ir leyendo los valores de las coordenadas x, y, z
		char nombre[30];

		//Doble for para vaciar en las matrices los valores de traslacion y color
		//i = nombreMatriz, j = renglonMatriz
		for (int i=0; i<PARTES; i++){
			entrada >> nombre;
			if (strcmp(nombre, "tronco") == 0) elementos[i].f = tronco;
			else if (strcmp(nombre, "hombro") == 0) elementos[i].f = hombro;
			else if (strcmp(nombre, "bicep") == 0) elementos[i].f = bicep;
			else if (strcmp(nombre, "codo") == 0) elementos[i].f = codo;
			else if (strcmp(nombre, "antebrazo") == 0) elementos[i].f = antebrazo;
			else if (strcmp(nombre, "mano") == 0) elementos[i].f = mano;
			else if (strcmp(nombre, "cadera") == 0) elementos[i].f = cadera;
			else if (strcmp(nombre, "quadricep") == 0) elementos[i].f = quadricep;
			else if (strcmp(nombre, "rodilla") == 0) elementos[i].f = rodilla;
			else if (strcmp(nombre, "chamorro") == 0) elementos[i].f = chamorro;
			else if (strcmp(nombre, "pie") == 0) elementos[i].f = pie;
			else if (strcmp(nombre, "cuello") == 0) elementos[i].f = cuello;
			else if (strcmp(nombre, "cabeza") == 0) elementos[i].f = cabeza;
			for(int j=0; j<DATOS; j++){
				entrada >> valor;
				switch (j) {
					case 0: elementos[i].x = valor; break;
					case 1: elementos[i].y = valor; break;
					case 2: elementos[i].z = valor; break;
					case 3: elementos[i].r = valor; break;
					case 4: elementos[i].g = valor; break;
					case 5: elementos[i].b = valor; break;
				}
			}
			if (i == 5 || i == 10 || i == 15 || i == 20 || i == 22)
				elementos[i].child = NULL;
			else
				elementos[i].child = &elementos[i+1];

			if (i == 1 || i == 6 || i == 11 || i == 16)
				elementos[i].sibling = &elementos[i+5];
			else
				elementos[i].sibling = NULL;
		}
		entrada.close();
	}


// recorrido a profundidad: primero los hijos, luego los hermanos
	void traverse (nodo *node)
	{
	// guardar la matriz actual porque las transformaciones a realizarse
	// sólo deben afectarle a él y a sus hijos
		glPushMatrix();

	// transformar relativo a su padre
		glMultMatrixf(node->m);

	//glLightfv(GL_LIGHT0, GL_DIFFUSE, specular);
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
		glColor3f(node->r, node->g, node->b);

		glRotatef(node->rotX, 1, 0, 0);
		glRotatef(node->rotY, 0, 1, 0);
		glRotatef(node->rotZ, 0, 0, 1);

	// dibujar el nodo
		node->f();

	// primer recorrer los hijos (si hay)
		if (node->child != NULL)
			traverse(node->child);

		glPopMatrix();
	// después recorrer los hermanos (si hay)
		if(node->sibling != NULL)
			traverse(node->sibling);

	}


// Método de desplegado
	void myDisplay()
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable( GL_DEPTH_TEST ); // Importante para que se vean bien las caras, probar que pasa si lo quito
		glLoadIdentity();
		glTranslatef(0, 0, -15);
		glRotatef(anguloX, 1, 0, 0);
		glRotatef(anguloY, 0, 1, 0);
		glRotatef(anguloZ, 0, 0, 1);
		traverse(elementos);
		fondo();
		glFlush();
		glutSwapBuffers();
	}

	void inicializaElementos()
	{


	//tronco
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, elementos[0].m);

		//resto de las partes del cuerpo
		for (int i = 1; i < PARTES; i++) {
			glLoadIdentity();
			glTranslatef(elementos[i].x, elementos[i].y, elementos[i].z);
			glGetFloatv(GL_MODELVIEW_MATRIX, elementos[i].m);
		}
		glColor3f(0.54,0.27,0.074);
	}


// Método de actualización del tamaño de la ventana
	void reshape(GLsizei width, GLsizei height)
	{
		glViewport(0,0,width,height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45,(GLfloat)width/(GLfloat)height,  0.01, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

// Método para el procesamiento de las teclas especiales
	void flechas(int key, int x, int y)
	{
		switch (key){
			case GLUT_KEY_F1: exit(0);               // Cuando se presiona F1, termina el programa
			break;
			case 's':
				ejecuta_animacion = ejecuta_animacion * -1;
				break;

			case GLUT_KEY_UP:
			if(vista){
				anguloX += DELTA;
			}else if(hombro_derecho && limiteUpDo_hombro_derecho < LIMITE){
				elementos[1].rotZ += DELTA;
				limiteUpDo_hombro_derecho++;

			}else if(hombro_izquierdo && limiteUpDo_hombro_izquierdo < LIMITE){
				elementos[6].rotZ -= DELTA;
				limiteUpDo_hombro_izquierdo++;

			}else if(cadera_derecha && limiteUpDo_cadera_derecha < LIMITE){
				elementos[11].rotX -= DELTA;
				limiteUpDo_cadera_derecha++;

			}else if(rodilla_derecha && limiteUpDo_rodilla_derecha < (LIMITE-LIMITE)) {
				elementos[13].rotX -= DELTA;
				limiteUpDo_rodilla_derecha++;

			}else if(cadera_izquierda && limiteUpDo_cadera_izquierda < LIMITE){
				elementos[16].rotX -= DELTA;
				limiteUpDo_cadera_izquierda++;

			}else if(rodilla_izquierda && limiteUpDo_rodilla_izquierda < (LIMITE-LIMITE)){
				elementos[18].rotX -= DELTA;
				limiteUpDo_rodilla_izquierda++;
						//Cabeza
			}else if(cabeza_rot && limiteUpDo_cabeza < LIMITE){
				elementos[22].rotX -= DELTA;
				limiteUpDo_cabeza++;

			}
			break;
			case GLUT_KEY_DOWN:
			if (vista){
				anguloX -= DELTA;
			}else if(hombro_derecho && limiteUpDo_hombro_derecho > -LIMITE){
				elementos[1].rotZ -= DELTA;
				limiteUpDo_hombro_derecho--;

			}else if(hombro_izquierdo && limiteUpDo_hombro_izquierdo > -LIMITE){
				elementos[6].rotZ += DELTA;
				limiteUpDo_hombro_izquierdo--;

			}else if(cadera_derecha && limiteUpDo_cadera_derecha > -LIMITE){
				elementos[11].rotX += DELTA;
				limiteUpDo_cadera_derecha--;

			}else if(rodilla_derecha && limiteUpDo_rodilla_derecha > -(LIMITE+10)){
				elementos[13].rotX +=DELTA;
				limiteUpDo_rodilla_derecha--;

			}else if(cadera_izquierda && limiteUpDo_cadera_izquierda >-LIMITE){
				elementos[16].rotX += DELTA;
				limiteUpDo_cadera_izquierda--;

			}else if(rodilla_izquierda && limiteUpDo_rodilla_izquierda > -(LIMITE+10)){
				elementos[18].rotX += DELTA;
				limiteUpDo_rodilla_izquierda--;

			}else if(cabeza_rot && limiteUpDo_cabeza > -LIMITE){
				elementos[22].rotX += DELTA;
				limiteUpDo_cabeza--;

			}
			break;
			case GLUT_KEY_LEFT:
			if(vista){
				anguloY -= DELTA;
			}else if(hombro_derecho && limiteLeRi_hombro_derecho < LIMITE){
				elementos[1].rotY -= DELTA;
				limiteLeRi_hombro_derecho++;

			}else if(codo_derecho && limiteLeRi_codo_derecho > -(LIMITE+10)){
				elementos[3].rotY -= DELTA;
				limiteLeRi_codo_derecho--;

			}else if(hombro_izquierdo && limiteLeRi_hombro_izquierdo < LIMITE){
				elementos[6].rotY += DELTA;
				limiteLeRi_hombro_izquierdo++;

			}else if(codo_izquierdo && limiteLeRi_codo_izquierdo < (LIMITE-LIMITE)){
				elementos[8].rotY -= DELTA;
				limiteLeRi_codo_izquierdo++;


			}else if (cabeza_rot && limiteLeRi_cabeza < LIMITE){
				elementos[22].rotY -= DELTA;
				limiteLeRi_cabeza++;

			}
			break;

			case GLUT_KEY_RIGHT:
			if (vista){
				anguloY += DELTA;
			}else if(hombro_derecho && limiteLeRi_hombro_derecho > -(LIMITE-10)){
				elementos[1].rotY += DELTA;
				limiteLeRi_hombro_derecho--;

			}else if(codo_derecho && limiteLeRi_codo_derecho < (LIMITE-LIMITE)){
				elementos[3].rotY += DELTA;
				limiteLeRi_codo_derecho++;

			}else if(hombro_izquierdo && limiteLeRi_hombro_izquierdo > -(LIMITE-10)){
				elementos[6].rotY -= DELTA;
				limiteLeRi_hombro_izquierdo--;

			}else if(codo_izquierdo && limiteLeRi_codo_izquierdo > -(LIMITE+10)){
				elementos[8].rotY += DELTA;
				limiteLeRi_codo_izquierdo--;

			}else if (cabeza_rot && limiteLeRi_cabeza > -LIMITE){
				elementos[22].rotY += DELTA;
				limiteLeRi_cabeza--;
			}
			break;
		}
		glutPostRedisplay();
	}


// Método de asignación de los callbacks
	void indicaMetodos()
	{
		glutDisplayFunc(myDisplay);
		glutReshapeFunc(reshape);
		glutSpecialFunc(flechas);
		glutCreateMenu(processMenuEvents);
		glutAddMenuEntry("Todo",VISTA);
		glutAddMenuEntry("Hombro Derecho",HOMBRO_DERECHO);
		glutAddMenuEntry("Codo Derecho",CODO_DERECHO);
		glutAddMenuEntry("Hombro Izquierdo",HOMBRO_IZQUIERDO);
		glutAddMenuEntry("Codo Izquierdo",CODO_IZQUIERDO);
		glutAddMenuEntry("Pierna Derecha",CADERA_DERECHA);
		glutAddMenuEntry("Rodilla Derecha",RODILLA_DERECHA);
		glutAddMenuEntry("Pierna Izquierdo",CADERA_IZQUIERDA);
		glutAddMenuEntry("Rodilla Izquierda",RODILLA_IZQUIERDA);
		glutAddMenuEntry("Cabeza",CABEZA);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

	void cargaImagenes(){
	//Lectura y Carga de Imagenes en el Arreglo
		texture[0] = LoadTexture("horse.tga");
		texture[1] = LoadTexture("horse2.tga");
		texture[2] = LoadTexture("dessert.tga");
		texture[3] = LoadTexture("sky.tga");
		texture[4] = LoadTexture("ground.tga");
		texture[5] = NULL;
	}

// Método de inicialización de las características de la ventana, del cursor y de OPENGL
	void inicializa()
	{
		glutInitWindowSize( 700, 700 );
		glutInitWindowPosition( 100, 100 );
		glutCreateWindow( "" );
		glutSetWindowTitle( "HorseMan" );
		glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
		glEnable(GL_TEXTURE_2D);
		GLfloat spotpos[]={1.0,2.0,3.0,1.0};
		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotpos);	

	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	}

// Método inicial, aquí empieza la ejecución del programa
	int main(int argc, char **argv)
	{
		glutInit(&argc, argv);
		inicializa();
		cargaImagenes();
		leerarch();
		indicaMetodos();
		inicializaElementos();
		glutTimerFunc(1500/24,animationTimer,1);
		glutMainLoop();
		return 0;
	}


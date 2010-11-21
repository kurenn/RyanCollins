// prueba1.cpp : main project file.

/*******************************************************************************************************
 *  Programa ejemplo de manejo de OPENGL y GLUT                                                        *
 *  Basado en el material desarrollado por el Dr. Rudomín y su equipo de trabajo                       *
 *  Despliega un cubo y tres esferas                                                                   *
 *  Utiliza un árbol jerárquico para desplegarlos                                                      *
 *  Selecciona el cursor con figura de reloj                                                           *
 *  Termina el programa si se presiona la tecla F1                                                     *
 *  Rota el cubo en cuatro direcciones diferentes, dependiendo si se presiona la tecla de flecha       *
 *  hacia arriba o flecha hacia abajo o flecha izquierda o flecha derecha                                                                 *
 *  No incluye comentarios de las funciones, métodos y constantes de OPENGL y GLUT                     *
 *  pues se espera que los alumnos analicen su funcionamiento consultando los APIs correspondientes    *
 *  API OPENGL : http://www.opengl.org/sdk/docs/man/                                                   *
 *  API GLUT: http://www.opengl.org/resources/libraries/glut/glut-3.spec.pdf                           *
 *                                                                                                     *
 *******************************************************************************************************
 *  Autor: Olivia M. Barrón Cano                                                                       *
 *  Fecha de creación:  Enero 2, 2010                                                                  *
 *  Fecha de última actualización: Octubre 12, 2010                                                    *
 *                                                                                                     *
 *******************************************************************************************************/

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
float mcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

// Empieza la declaración de métodos y funciones

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
	glScalef(2.0, 1.0, 1.0);
	glutSolidCube(0.25);
	glScalef(0.5, 1.0, 1.0);
}

void antebrazo(){
	glScalef(2.0, 1.0, 1.0);
	glutSolidCube(0.25);
	glScalef(0.5, 1.0, 1.0);
}

void codo(){
	glutSolidSphere(0.2, 30,30);
}

void mano(){
	glutSolidSphere(0.2, 30,30);
}

void quadricep(){
	glScalef(1.0, 3.0, 1.0);
	glutSolidCube(0.25);
	glScalef(1.0, 0.3333, 1.0);
}

void rodilla(){
	glutSolidSphere(0.2, 30,30);
}

void chamorro(){
	glScalef(1.0, 2.0, 1.0);
	glutSolidCube(0.25);
	glScalef(1.0, 0.3333, 1.0);
}

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
	glutSolidCube(0.5);
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
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
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
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable( GL_DEPTH_TEST ); // Importante para que se vean bien las caras, probar que pasa si lo quito
	glLoadIdentity();
	glTranslatef(0, 0, -8);
	glRotatef(anguloX, 1, 0, 0);
	glRotatef(anguloY, 0, 1, 0);
    traverse(elementos);
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
}


// Método de actualización del tamaño de la ventana
void reshape(GLsizei width, GLsizei height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(GLfloat)width/(GLfloat)height,  0.01, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Método para el procesamiento de las teclas especiales
void flechas(int key, int x, int y)
{
	switch (key){
		case GLUT_KEY_F1: exit(0);               // Cuando se presiona F1, termina el programa
			break;
		case GLUT_KEY_UP:
			if(vista){
				anguloX += DELTA;
			}else if(hombro_derecho){
				elementos[1].rotZ += DELTA;
				
			}else if(codo_derecho){
				elementos[3].rotZ += DELTA;
				
			}else if(hombro_izquierdo){
				elementos[6].rotZ -= DELTA;
				
			}else if(codo_izquierdo){
				elementos[8].rotZ -= DELTA;
				
			}else if(cadera_derecha){
				elementos[11].rotX -= DELTA;
				
			}else if(rodilla_derecha){
				elementos[13].rotX -= DELTA;
				
			}else if(cadera_izquierda){
				elementos[16].rotX -= DELTA;
				
			}else if(rodilla_izquierda){
				elementos[18].rotX -= DELTA;
				
			}else{
				elementos[22].rotX -= DELTA;
				
			}
			break;
		case GLUT_KEY_DOWN:
			if (vista){
				anguloX -= DELTA;
			}else if(hombro_derecho){
				elementos[1].rotZ -= DELTA;
				
			}else if(codo_derecho){
				elementos[3].rotZ -= DELTA;
				
			}else if(hombro_izquierdo){
				elementos[6].rotZ += DELTA;
				
			}else if(codo_izquierdo){
				elementos[8].rotZ += DELTA;
				
			}else if(cadera_derecha){
				elementos[11].rotX += DELTA;
				
			}else if(rodilla_derecha){
				elementos[13].rotX +=DELTA;
				
			}else if(cadera_izquierda){
				elementos[16].rotX += DELTA;
				
			}else if(rodilla_izquierda){
				elementos[18].rotX += DELTA;
				
			}else{
				elementos[22].rotX += DELTA;
				
			}
			break;
		case GLUT_KEY_LEFT:
			if(vista){
				anguloY -= DELTA;
			}else if(hombro_derecho){
				elementos[1].rotY -= DELTA;
				
			}else if(codo_derecho){
				elementos[3].rotY -= DELTA;
				
			}else if(hombro_izquierdo){
				elementos[6].rotY += DELTA;
				
			}else if(codo_izquierdo){
				elementos[8].rotY += DELTA;
				
			}else{
				elementos[22].rotY -= DELTA;
				
			}
			break;
			
		case GLUT_KEY_RIGHT:
		    if (vista){
		        anguloY += DELTA;
		    }else if(hombro_derecho){
		        elementos[1].rotY += DELTA;
				
		    }else if(codo_derecho){
		        elementos[3].rotY += DELTA;
				
		    }else if(hombro_izquierdo){
		        elementos[6].rotY -= DELTA;
				
		    }else if(codo_izquierdo){
		        elementos[8].rotY -= DELTA;
				
		    }else{
				elementos[22].rotY += DELTA;
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

// Método de inicialización de las características de la ventana, del cursor y de OPENGL
void inicializa()
{
	glutInitWindowSize( 700, 700 );
	glutInitWindowPosition( 100, 100 );
  glutCreateWindow( "" );
	glutSetWindowTitle( "" );
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// Método inicial, aquí empieza la ejecución del programa
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	inicializa();
	leerarch();
  indicaMetodos();
	inicializaElementos();
	glutMainLoop();
	return 0;
}
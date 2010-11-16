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

#define PARTES 23 //constante para partes del cuerpo
#define DATOS 6 //constante para partes del cuerpo

typedef struct nodo
{ float m[16];          // para guardar la matriz de transformación local
	void (*f)();          // para guardar el apuntador a la función que dibuja
	float r, g, b;       // para guardar color
	struct nodo *sibling;  // para guardar el apuntador al primer hermano
	struct nodo *child;    // para guardar el apuntador al primer hijo
	float x, y, z;      //para guardar la traslacion de la parte del cuerpo
} nodo;



// Sección de declaración de constantes globales
const float DELTA = 5;    // Valor para el incremento/decremento del ángulo

// Sección de declaración de variables globales
float anguloX = 0.0;        // Variable para manejar el ángulo de rotación a aplicar en X;
float anguloY = 0.0;        // Variable para manejar el ángulo de rotación a aplicar en Y;
nodo elementos[24];

// Empieza la declaración de métodos y funciones

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
	    if (i == 1 || i == 2 || i == 3 || i == 4 || i == 5 ||
	        i == 6 || i == 11 || i == 12 ||i == 13 || i == 14 || i == 16)
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

	glColor3f(node->r, node->g, node->b);

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
	switch (key)
	{ case GLUT_KEY_F1: exit(0);               // Cuando se presiona F1, termina el programa
			break;
		case GLUT_KEY_UP: anguloX += DELTA+1;       // Si se presiona la flecha hacia arriba, aumenta el ángulo de
			break;                 // rotación en X
		case GLUT_KEY_DOWN: anguloX -= DELTA;     // Si se presiona la flecha hacia abajo, disminuye el ángulo de
			break;               // rotación en X
		case GLUT_KEY_LEFT: anguloY += DELTA;       // Si se presiona la flecha hacia la izquierda, aumenta el ángulo de
			break;                 // rotación en Y
		case GLUT_KEY_RIGHT: anguloY -= DELTA;     // Si se presiona la flecha hacia la derecha, disminuye el ángulo de
			break;               // rotación en Y
	}
	glutPostRedisplay();
}

// Método de asignación de los callbacks
void indicaMetodos()
{
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutSpecialFunc(flechas);
}

// Método de inicialización de las características de la ventana, del cursor y de OPENGL
void inicializa()
{
	glutInitWindowSize( 700, 700 );
	glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "" );
	glutSetWindowTitle( "Ejemplo 4" );
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glShadeModel(GL_FLAT);
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


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
#include <GLUT/glut.h>
#include <stdlib.h>       // Librería necesaria para usar la función exit() que termina la ejecución del programa
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <fstream>  // libreria para el manejo de archivos
#include <assert.h> // libreria para verificar la existencia de un archivo
#include <iostream> // libreria para el manejo de cin y cout

//Matrices para almacenar datos de las partes del cuerpo
GLfloat troncoMatriz [4][3];
GLfloat hombroMatriz [4][3];
GLfloat bicepMatriz [4][3];
GLfloat codoMatriz [4][3];
GLfloat antebrazoMatriz [4][3];


typedef struct nodo
{ float m[16];          // para guardar la matriz de transformación local
	void (*f)();          // para guardar el apuntador a la función que dibuja
	float r, g, b;       // para guardar color
	struct nodo *sibling;  // para guardar el apuntador al primer hermano
	struct nodo *child;    // para guardar el apuntador al primer hijo
	float x, y, z;
	float rotX, rotY, rotZ; 
} nodo;



// Sección de declaración de constantes globales
const float DELTA = 5;    // Valor para el incremento/decremento del ángulo

// Sección de declaración de variables globales
float anguloX = 0.0;        // Variable para manejar el ángulo de rotación a aplicar en X;
float anguloY = 0.0;        // Variable para manejar el ángulo de rotación a aplicar en Y;
nodo elementos[24];

// Empieza la declaración de métodos y funciones

//Metodo para leer archivo
void leerarch(){
    ifstream entrada;

    entrada.open("datos.txt");
    assert(entrada);
    double valor;     // Auxiliar para ir leyendo los valores de las coordenadas x, y, z

    //Triple for para vaciar en las matrices los valores de traslacion, color, escalaIni y escalaFin
    //i = nombreMatriz, j = renglonMatriz, k = columnaMatriz
    //El orden de las líneas en el archivo a leer son: tronco, hombro, bicep, codo, antebrazo
    for (int i=0; i<23; i++){
        for(int j=0; j<3; j++){
            entrada >> valor;
            switch (j) {
                case 0: elementos[i].x = valor;
                case 1: elementos[i].y = valor;
                case 2: elementos[i].z = valor;
            }
        }
    }
    entrada.close();
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
// recorrido a profundidad: primero los hijos, luego los hermanos
void traverse (nodo *node)
{
	// guardar la matriz actual porque las transformaciones a realizarse
	// sólo deben afectarle a él y a sus hijos
	glPushMatrix();

	// transformar relativo a su padre
	glMultMatrixf(node->m);

	glColor3f(node->r, node->g, node->b);

	glRotatef(node->rotX, 1, 0, 0);
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
	//glRotatef(anguloY, 0, 1, 0);
    traverse(elementos);
	glFlush();
	glutSwapBuffers();
}


void inicializaElementos()
{
	//tronco
	glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[0].m);
	elementos[0].f = tronco;
	elementos[0].sibling = NULL;
	elementos[0].child = &elementos[1];

	//hombro derecho
	glLoadIdentity();
    glTranslatef(elementos[1].x, elementos[1].y, elementos[1].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[1].m);
	elementos[1].r = 252.0/255.0;
	elementos[1].g = 128.0/255.0;
	elementos[1].b = 0.0/255.0;
	elementos[1].rotX = 0;
	elementos[1].rotY = 0;
	elementos[1].rotZ = 0;
	elementos[1].f = hombro;
	elementos[1].sibling = &elementos[6];
	elementos[1].child = &elementos[2];

	//bicep derecho
	glLoadIdentity();
    glTranslatef(elementos[2].x, elementos[2].y, elementos[2].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[2].m);
	elementos[2].r = 252.0/255.0;
	elementos[2].g = 128.0/255.0;
	elementos[2].b = 0.0/255.0;
	elementos[2].rotX = 0;
	elementos[2].rotY = 0;
	elementos[2].rotZ = 0;
	elementos[2].f = bicep;
	elementos[2].sibling = &elementos[7];
	elementos[2].child = &elementos[3];

	//Codo derecho
	glLoadIdentity();
    glTranslatef(elementos[3].x, elementos[3].y, elementos[3].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[3].m);
	elementos[3].r = 252.0/255.0;
	elementos[3].g = 128.0/255.0;
	elementos[3].b = 0.0/255.0;
	elementos[3].rotX = 0;
	elementos[3].rotY = 0;
	elementos[3].rotZ = 0;
	elementos[3].f = codo;
	elementos[3].sibling = &elementos[8];
	elementos[3].child = &elementos[4];

	//Antebrazo derecho
	glLoadIdentity();
    glTranslatef(elementos[4].x, elementos[4].y, elementos[4].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[4].m);
	elementos[4].r = 252.0/255.0;
	elementos[4].g = 128.0/255.0;
	elementos[4].b = 0.0/255.0;
	elementos[4].rotX = 0;
	elementos[4].rotY = 0;
	elementos[4].rotZ = 0;
	elementos[4].f = antebrazo;
	elementos[4].sibling = &elementos[9];
	elementos[4].child = &elementos[5];

	//Mano derecho
	glLoadIdentity();
    glTranslatef(elementos[5].x, elementos[5].y, elementos[5].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[5].m);
	elementos[5].r = 252.0/255.0;
	elementos[5].g = 128.0/255.0;
	elementos[5].b = 0.0/255.0;
	elementos[5].rotX = 0;
	elementos[5].rotY = 0;
	elementos[5].rotZ = 0;
	elementos[5].f = mano;
	elementos[5].sibling = &elementos[10];
	elementos[5].child = NULL;

    //hombro izquierdo
	glLoadIdentity();
    glTranslatef(elementos[6].x, elementos[6].y, elementos[6].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[6].m);
	elementos[6].r = 1;
	elementos[6].g = 0;
	elementos[6].b = 0;
	elementos[6].rotX = 0;
	elementos[6].rotY = 0;
	elementos[6].rotZ = 0;
	elementos[6].f = hombro;
	elementos[6].sibling = &elementos[11];
	elementos[6].child = &elementos[7];

    //bicep izquierdo
	glLoadIdentity();
    glTranslatef(elementos[7].x, elementos[7].y, elementos[7].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[7].m);
	elementos[7].r = 252.0/255.0;
	elementos[7].g = 128.0/255.0;
	elementos[7].b = 0.0/255.0;
	elementos[7].rotX = 0;
	elementos[7].rotY = 0;
	elementos[7].rotZ = 0;
	elementos[7].f = bicep;
	elementos[7].sibling = NULL;
	elementos[7].child = &elementos[8];

//	//Codo izquierdo
	glLoadIdentity();
    glTranslatef(elementos[8].x, elementos[8].y, elementos[8].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[8].m);
	elementos[8].r = 252.0/255.0;
	elementos[8].g = 128.0/255.0;
	elementos[8].b = 0.0/255.0;
	elementos[8].rotX = 0;
	elementos[8].rotY = 0;
	elementos[8].rotZ = 0;
	elementos[8].f = codo;
	elementos[8].sibling = NULL;
	elementos[8].child = &elementos[9];
//
//    //Antebrazo izquierdo
	glLoadIdentity();
    glTranslatef(elementos[9].x, elementos[9].y, elementos[9].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[9].m);
	elementos[9].r = 252.0/255.0;
	elementos[9].g = 128.0/255.0;
	elementos[9].b = 0.0/255.0;
	elementos[9].rotX = 0;
	elementos[9].rotY = 0;
	elementos[9].rotZ = 0;
	elementos[9].f = antebrazo;
	elementos[9].sibling = NULL;
	elementos[9].child = &elementos[10];

//	//Mano izquierda
	glLoadIdentity();
    glTranslatef(elementos[10].x, elementos[10].y, elementos[10].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[10].m);
	elementos[10].r = 252.0/255.0;
	elementos[10].g = 128.0/255.0;
	elementos[10].b = 0.0/255.0;
	elementos[10].rotX = 0;
	elementos[10].rotY = 0;
	elementos[10].rotZ = 0;
	elementos[10].f = mano;
	elementos[10].sibling = NULL;
	elementos[10].child = NULL;


	//cadera derecha
	glLoadIdentity();
    glTranslatef(elementos[11].x, elementos[11].y, elementos[11].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[11].m);
	elementos[11].r = 1;
	elementos[11].g = 0;
	elementos[11].b = 0;
	elementos[11].rotX = 0;
	elementos[11].rotY = 0;
	elementos[11].rotZ = 0;
	elementos[11].f = cadera;
	elementos[11].sibling = &elementos[16];
	elementos[11].child = &elementos[12];

	//quadricep derecho
	glLoadIdentity();
    glTranslatef(elementos[12].x, elementos[12].y, elementos[12].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[12].m);
	elementos[12].r = 1;
	elementos[12].g = 0;
	elementos[12].b = 0;
	elementos[12].rotX = 0;
	elementos[12].rotY = 0;
	elementos[12].rotZ = 0;
	elementos[12].f = quadricep;
	elementos[12].sibling = &elementos[17];
	elementos[12].child = &elementos[13];

	//rodilla derecha
	glLoadIdentity();
    glTranslatef(elementos[13].x, elementos[13].y, elementos[13].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[13].m);
	elementos[13].r = 0;
	elementos[13].g = 1;
	elementos[13].b = 0;
	elementos[13].rotX = 0;
	elementos[13].rotY = 0;
	elementos[13].rotZ = 0;
	elementos[13].f = rodilla;
	elementos[13].sibling = &elementos[18];
	elementos[13].child = &elementos[14];

	//chamorro derecho
	glLoadIdentity();
    glTranslatef(elementos[14].x, elementos[14].y, elementos[14].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[14].m);
	elementos[14].r = 0;
	elementos[14].g = 1;
	elementos[14].b = 0;
	elementos[14].rotX = 0;
	elementos[14].rotY = 0;
	elementos[14].rotZ = 0;
	elementos[14].f = chamorro;
	elementos[14].sibling = &elementos[19];
	elementos[14].child = &elementos[15];

	//pie derecho
	glLoadIdentity();
    glTranslatef(elementos[15].x, elementos[15].y, elementos[15].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[15].m);
	elementos[15].r = 1;
	elementos[15].g = 0;
	elementos[15].b = 0;
	elementos[15].rotX = 0;
	elementos[15].rotY = 0;
	elementos[15].rotZ = 0;;
	elementos[15].f = pie;
	elementos[15].sibling = NULL;
	elementos[15].child = NULL;

	//cadera izquierda
	glLoadIdentity();
    glTranslatef(elementos[16].x, elementos[16].y, elementos[16].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[16].m);
	elementos[16].r = 1;
	elementos[16].g = 0;
	elementos[16].b = 0;
	elementos[16].rotX = 0;
	elementos[16].rotY = 0;
	elementos[16].rotZ = 0;;
	elementos[16].f = cadera;
	elementos[16].sibling = &elementos[21];
	elementos[16].child = &elementos[17];

	//quadricep izquierdo
	glLoadIdentity();
    glTranslatef(elementos[17].x, elementos[17].y, elementos[17].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[17].m);
	elementos[17].r = 1;
	elementos[17].g = 0;
	elementos[17].b = 0;
	elementos[17].rotX = 0;
	elementos[17].rotY = 0;
	elementos[17].rotZ = 0;;
	elementos[17].f = quadricep;
	elementos[17].sibling = NULL;
	elementos[17].child = &elementos[18];

//	//rodilla izquierda
	glLoadIdentity();
    glTranslatef(elementos[18].x, elementos[18].y, elementos[18].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[18].m);
	elementos[18].r = 0;
	elementos[18].g = 1;
	elementos[18].b = 0;
	elementos[18].rotX = 0;
	elementos[18].rotY = 0;
	elementos[18].rotZ = 0;;
	elementos[18].f = rodilla;
	elementos[18].sibling = NULL;
	elementos[18].child = &elementos[19];

	//chamorro izquierdo
	glLoadIdentity();
    glTranslatef(elementos[19].x, elementos[19].y, elementos[19].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[19].m);
	elementos[19].r = 0;
	elementos[19].g = 1;
	elementos[19].b = 0;
	elementos[19].rotX = 0;
	elementos[19].rotY = 0;
	elementos[19].rotZ = 0;;
	elementos[19].f = chamorro;
	elementos[19].sibling = NULL;
	elementos[19].child = &elementos[20];

	//pie izquierdo
	glLoadIdentity();
    glTranslatef(elementos[20].x, elementos[20].y, elementos[20].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[20].m);
	elementos[20].r = 1;
	elementos[20].g = 0;
	elementos[20].b = 0;
	elementos[20].rotX = 0;
	elementos[20].rotY = 0;
	elementos[20].rotZ = 0;;
	elementos[20].f = pie;
	elementos[20].sibling = NULL;
	elementos[20].child = NULL;

    //cuello
    glLoadIdentity();
    glTranslatef(elementos[21].x, elementos[21].y, elementos[21].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[21].m);
	elementos[21].r = 1;
	elementos[21].g = 0;
	elementos[21].b = 0;
	elementos[21].rotX = 0;
	elementos[21].rotY = 0;
	elementos[21].rotZ = 0;;
	elementos[21].f = cuello;
	elementos[21].sibling = NULL;
	elementos[21].child = &elementos[22];

	//cabeza
    glLoadIdentity();
    glTranslatef(elementos[22].x, elementos[22].y, elementos[22].z);
    glGetFloatv(GL_MODELVIEW_MATRIX, elementos[22].m);
	elementos[22].r = 1;
	elementos[22].g = 0;
	elementos[22].b = 0;
	elementos[22].rotX = 0;
	elementos[22].rotY = 0;
	elementos[22].rotZ = 0;;
	elementos[22].f = cabeza;
	elementos[22].sibling = NULL;
	elementos[22].child = NULL;
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
		case GLUT_KEY_UP: elementos[22].rotX += DELTA;       // Si se presiona la flecha hacia arriba, aumenta el ángulo de
			break;                 // rotación en X
		case GLUT_KEY_DOWN: elementos[22].rotX -= DELTA;     // Si se presiona la flecha hacia abajo, disminuye el ángulo de
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


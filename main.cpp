#include <iostream>
#include <stdlib.h>
#include "parser.h"
#include <GL/gl.h>
#include <GL/glut.h>
using namespace std;

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

float width = 1000;
float height = 600;
Modelo modelo;
//posicao do observador (camera)
GLdouble viewer[] = {2.0, 2.0, 3.0};

void init(void)
{

    glClearColor(0.0, 0.0, 0.0, 0.0);                   // cor para limpeza do buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
}

void desenhaMenu()
{
    glColor3f(1.0, 0.1, 0.34);
    glBegin(GL_POLYGON); // inicia o desenho do padrao na origem
    glVertex2f(2 * (width / 3) + 10, 50);
    glVertex2f(3000, 50);
    glVertex2f(3000, 100);
    glVertex2f(2 * (width / 3) + 10, 100);
    glEnd();
}

void desenhaObjeto()
{
    glColor3f(0.40, 0.6, 0.34);
    glBegin(GL_POLYGON); // inicia o desenho do padrao na origem
    glVertex2f(0, 50);
    glVertex2f(1000, 50);
    glVertex2f(1000, 100);
    glVertex2f(0, 100);
    glEnd();
}

void carregarModelos()
{
    OBJ obj;
    modelo = obj.lerArquivo("Modelos/teapot.obj");
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < modelo.faces.size(); i++)
    {
        for (int j = 0; j < modelo.faces[i].vertices.size(); i++)
        {
            cout << modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].x << " " << modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].y << " " << modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].z << endl;
            glVertex3f(modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].x,
                       modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].y,
                       modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].z);
        }
    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    glViewport(0, 0, 2 * (width / 3), height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, 500, 0, height); //gluPerspective(45.0, 0, 0.01, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //desenhaObjeto();

    /*gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);
*/

    glViewport(2 * (width / 3), 0, width / 3, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(2 * (width / 3), width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //desenhaMenu();

    //carregarModelos();

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
    //glutSwapBuffers(); //usando double buffer (para evitar 'flicker')
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0); //ESC
    if (key == 'x')
        viewer[0] -= 1.0;
    if (key == 'X')
        viewer[0] += 1.0;
    if (key == 'y')
        viewer[1] -= 1.0;
    if (key == 'Y')
        viewer[1] += 1.0;
    if (key == 'z')
        viewer[2] -= 1.0;
    if (key == 'Z')
        viewer[2] += 1.0;
    display();
}

int main(int argc, char **argv)
{
    carregarModelos();
    glutInit(&argc, argv);                                    //inicializa a glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores
    glutInitWindowSize(width, height);                        //dimens�es da janela
    glutInitWindowPosition(100, 100);                         //posicao da janela
    glutCreateWindow("Trabalho 3");                           //cria a janela
    init();
    glutDisplayFunc(display);   //determina fun��o callback de desenho
    glutKeyboardFunc(keyboard); //determina fun��o callback de teclado

    glEnable(GL_DEPTH_TEST); //habilita remo��o de superf�cies ocultas usando Z-Buffer

    //   glEnable (GL_CULL_FACE); //habilita remo��o de superf�cies ocultas usando Back Facxe Culling
    //   glCullFace(GL_FRONT);    // Remove as faces da frente
    //   glCullFace(GL_BACK);    // Remove as faces de tras

    glutMainLoop();
    return 0;
}
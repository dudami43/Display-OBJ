#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

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

char texto[100];
char objName[100], objNameInput[100];
char rotacaoTexto[4][10];
char translacaoTexto[3][10];
char escalaTexto[3][10];
char triangulos[50];
char ms[50];

float rotacao[4] = {0.0};
float translacao[3] = {0.0};
float escala[3] = {1.0};

void init(void)
{

    glClearColor(0.0, 0.0, 0.0, 0.0);                   // cor para limpeza do buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    for (int i = 0; i < 4; i++)
    {
        sprintf(rotacaoTexto[i], "0.0");
    }
    for (int i = 0; i < 3; i++)
    {
        sprintf(translacaoTexto[i], "0.0");
        sprintf(escalaTexto[i], "1.0");
    }

    sprintf(objName, "teste.obj");
    sprintf(triangulos, "Triangulos: 0");
    sprintf(ms, "0.0 ms");
}

void desenhaMenu()
{

    //ms
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glRasterPos2f(900, 20);
    for (char *i = ms; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    //numero triangulos
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glRasterPos2f(676, 20);
    for (char *i = triangulos; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    //escala
    glColor3f(1.0, 1.0, 1.0);
    sprintf(texto, "Escala:");
    glPushMatrix();
    glRasterPos2f(676, 452);
    for (char *i = texto; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    for (int j = 0; j < 3; j++)
    {
        glPushMatrix();
        glRasterPos2f(850 + (j * 50) + 2, 452);
        for (char *i = escalaTexto[j]; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        glPopMatrix();
    }

    // 3 campos de texto
    glColor3f(1.0, 1.0, 1.0);
    //campo 3 (escalaZ)
    glBegin(GL_POLYGON);
    glVertex2f(950, 450);
    glVertex2f(990, 450);
    glVertex2f(990, 470);
    glVertex2f(950, 470);
    glEnd();

    //campo 2 (escalaY)
    glBegin(GL_POLYGON);
    glVertex2f(900, 450);
    glVertex2f(940, 450);
    glVertex2f(940, 470);
    glVertex2f(900, 470);
    glEnd();

    //campo 1 (escalaX)
    glBegin(GL_POLYGON);
    glVertex2f(850, 450);
    glVertex2f(890, 450);
    glVertex2f(890, 470);
    glVertex2f(850, 470);
    glEnd();

    //rotacao
    glColor3f(1.0, 1.0, 1.0);
    sprintf(texto, "Rotacao:");
    glPushMatrix();
    glRasterPos2f(676, 482);
    for (char *i = texto; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    for (int j = 0; j < 4; j++)
    {
        glPushMatrix();
        glRasterPos2f(800 + (j * 50) + 2, 482);
        for (char *i = rotacaoTexto[j]; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        glPopMatrix();
    }

    //4 campos de texto
    glColor3f(1.0, 1.0, 1.0);
    //campo 4 (rotacaoZ)
    glBegin(GL_POLYGON);
    glVertex2f(950, 480);
    glVertex2f(990, 480);
    glVertex2f(990, 500);
    glVertex2f(950, 500);
    glEnd();

    //campo 3 (rotacaoY)
    glBegin(GL_POLYGON);
    glVertex2f(900, 480);
    glVertex2f(940, 480);
    glVertex2f(940, 500);
    glVertex2f(900, 500);
    glEnd();

    //campo 2 (rotacaoX)
    glBegin(GL_POLYGON);
    glVertex2f(850, 480);
    glVertex2f(890, 480);
    glVertex2f(890, 500);
    glVertex2f(850, 500);
    glEnd();

    //campo 1 (rotacaoAngulo)
    glBegin(GL_POLYGON);
    glVertex2f(840, 480);
    glVertex2f(800, 480);
    glVertex2f(800, 500);
    glVertex2f(840, 500);
    glEnd();

    //translacao
    glColor3f(1.0, 1.0, 1.0);
    sprintf(texto, "Translacao:");
    glPushMatrix();
    glRasterPos2f(676, 512);
    for (char *i = texto; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    for (int j = 0; j < 3; j++)
    {
        glPushMatrix();
        glRasterPos2f(850 + (j * 50) + 2, 512);
        for (char *i = translacaoTexto[j]; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
        glPopMatrix();
    }

    // 3 campos de texto
    glColor3f(1.0, 1.0, 1.0);
    //campo 3 (translacaoZ)
    glBegin(GL_POLYGON);
    glVertex2f(950, 510);
    glVertex2f(990, 510);
    glVertex2f(990, 530);
    glVertex2f(950, 530);
    glEnd();

    //campo 2 (translacaoY)
    glBegin(GL_POLYGON);
    glVertex2f(900, 510);
    glVertex2f(940, 510);
    glVertex2f(940, 530);
    glVertex2f(900, 530);
    glEnd();

    //campo 1 (translacaoX)
    glBegin(GL_POLYGON);
    glVertex2f(850, 510);
    glVertex2f(890, 510);
    glVertex2f(890, 530);
    glVertex2f(850, 530);
    glEnd();

    //nome arquivo
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glRasterPos2f(676, 542);
    for (char *i = objName; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    //botao hidden
    glColor3f(0.84, 0.8, 0.86);
    glBegin(GL_POLYGON);
    glVertex2f(970, 540);
    glVertex2f(990, 540);
    glVertex2f(990, 560);
    glVertex2f(970, 560);
    glEnd();

    ///botao abrir arquivo texto
    glColor3f(0.0, 0.0, 0.0);
    sprintf(texto, "Importar");
    glPushMatrix();
    glRasterPos2f(872, 574);
    for (char *i = texto; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

    //botao abrir arquivo
    glColor3f(0.84, 0.8, 0.86);
    glBegin(GL_POLYGON);
    glVertex2f(870, 570);
    glVertex2f(990, 570);
    glVertex2f(990, 590);
    glVertex2f(870, 590);
    glEnd();

    //campo texto abrir arquivo
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(676, 570);
    glVertex2f(860, 570);
    glVertex2f(860, 590);
    glVertex2f(676, 590);
    glEnd();

    //FUNDO
    glColor3f(0.47, 0.02, 0.78);
    glBegin(GL_POLYGON);
    glVertex2f(2 * (width / 3), 0);
    glVertex2f(width, 0);
    glVertex2f(width, height);
    glVertex2f(2 * (width / 3), height);
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

    gluOrtho2D(-20, 20, -20, 20);
    //glFrustum(-3.0, 3.0, -3.225, 3.225, 2.0, 20.0);
    carregarModelos();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //desenhaObjeto();
    /*gluLookAt(viewer[0], viewer[1], viewer[2], // define posicao do observador
              0.0, 0.0, 0.0,                   // ponto de interesse (foco)
              0.0, 1.0, 0.0);*/

    glViewport(2 * (width / 3), 0, width / 3, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(2 * (width / 3), width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaMenu();

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
#include <Model/OBJ.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <stdlib.h>
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);



//posicao do observador (camera)
GLdouble viewer[] = {2.0, 2.0, 3.0};
Model model_teste;

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // proje��o paralela

   // glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // proje��o perspectiva

   glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // proje��o perspectiva

    //gluPerspective(70.0,1.0,2.0,20.0); // proje��o perspectiva
    glMatrixMode(GL_MODELVIEW);

   //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);                          // vetor de "view up"

    glFlush();
 //   glutSwapBuffers(); //usando double buffer (para evitar 'flicker')
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); //ESC
    if (key == 'x') viewer[0] -= 1.0;
    if (key == 'X') viewer[0] += 1.0;
    if (key == 'y') viewer[1] -= 1.0;
    if (key == 'Y') viewer[1] += 1.0;
    if (key == 'z') viewer[2] -= 1.0;
    if (key == 'Z') viewer[2] += 1.0;
    display();
}

void carregarModelos()
{
    OBJ obj("teapot.obj");
    obj.load(model_teste);
}


int main(int argc, char **argv) {
    carregarModelos();
    glutInit(&argc,argv); //inicializa a glut
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores
    glutInitWindowSize(500, 500); //dimens�es da janela
    glutInitWindowPosition(100, 100); //posicao da janela
    glutCreateWindow("Trabalho 3"); //cria a janela
    init();
    glutDisplayFunc(display); //determina fun��o callback de desenho
    glutKeyboardFunc(keyboard); //determina fun��o callback de teclado

    glEnable(GL_DEPTH_TEST); //habilita remo��o de superf�cies ocultas usando Z-Buffer


 //   glEnable (GL_CULL_FACE); //habilita remo��o de superf�cies ocultas usando Back Facxe Culling
 //   glCullFace(GL_FRONT);    // Remove as faces da frente
 //   glCullFace(GL_BACK);    // Remove as faces de tras

    glutMainLoop();
    return 0;
}

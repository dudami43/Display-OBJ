//
// INF390 - Computação Gráfica
//
// Visualizaçao 3D
// Desenha um cubo centrado na origem com faces de -1 a 1
// coloridas com diferentes cores
//
// Permite que a posicao do observador seja alterada usando o teclado
// X -> soma uma unidade na coordenada x do observador
// x -> subtrai  uma unidade na coordenada x do observador
// Y -> soma uma unidade na coordenada y do observador
// y -> subtrai  uma unidade na coordenada y do observador
// Z -> soma uma unidade na coordenada z do observador
// z -> subtrai  uma unidade na coordenada z do observador
//
// Marcus V. A. Andrade - 25/09/2008
//

#include <GL/gl.h>
#include <GL/glut.h>

#include <stdlib.h>
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

//funcao para desenhar um cubo colorido (desenha as suas 6 faces)
void colorcube();

//tamanho da janela em pixels
int largurajanela = 400, alturajanela = 300;

//vertices do cubo
/* ------------------------------------------
              vertices do cubo

      5 ----- 6
     /|      /|
    1 ----- 2 |
    | 4 ....| 7
    |/      |/
    0 ----- 3

*/

GLfloat vertices[8][3] = { {-1.0,-1.0,1.0},{-1.0,1.0,1.0},{1.0,1.0,1.0},{1.0,-1.0,1.0},
{-1.0,-1.0,-1.0},{-1.0,1.0,-1.0},{1.0,1.0,-1.0},{1.0,-1.0,-1.0} };

//cor de cada face do cubo
GLfloat colors[6][3] = { {0.0,0.0,0.0} , {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
{0.0,0.0,1.0} , {1.0,0.0,1.0}};

//posicao do observador (camera)
GLdouble viewer[] = {2.0, 2.0, 3.0};

// desenha uma face do cubo
// a, b, c e d sao indices no vetor de vertices
// ncolor eh um indice no vetor de cores
void quad(int a, int b, int c, int d, int ncolor) {
    glColor3fv(colors[ncolor]);
    glBegin(GL_QUADS);
        glVertex3fv(vertices[a]);
        glVertex3fv(vertices[b]);
        glVertex3fv(vertices[c]);
        glVertex3fv(vertices[d]);
    glEnd();
}

//desenha o cubo (faces no sentido anti-horario - face externa)
void colorcube() {

    // Faces em sentido anti-horario
    quad(0,3,2,1,0);
    quad(2,3,7,6,1);
    quad(0,4,7,3,2);
    quad(1,2,6,5,3);
    quad(4,5,6,7,4);
    quad(0,1,5,4,5);
}

// define o tipo de projecao
// glOrtho(left, right, bottom, top, near, far) -> projecao paralela
// glFrustum(left, right, bottom, top, near, far) -> projecao perspectiva
// gluPerspective(viewAngle, aspectRatio, near, far) -> projecao perspectiva
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // projeção paralela

   // glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // projeção perspectiva

   glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); // projeção perspectiva

    //gluPerspective(70.0,1.0,2.0,20.0); // projeção perspectiva
    glMatrixMode(GL_MODELVIEW);

   //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();
    gluLookAt(viewer[0],viewer[1],viewer[2], // define posicao do observador
    0.0, 0.0, 0.0,                           // ponto de interesse (foco)
    0.0, 1.0, 0.0);                          // vetor de "view up"

    colorcube(); //desenha o cubo

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


int main(int argc, char **argv) {
    glutInit(&argc,argv); //inicializa a glut
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores
    glutInitWindowSize(largurajanela, alturajanela); //dimensões da janela
    glutInitWindowPosition(100, 100); //posicao da janela
    glutCreateWindow("Visualizacao 3D - Movimento de camera "); //cria a janela
    init();
    glutDisplayFunc(display); //determina função callback de desenho
    glutKeyboardFunc(keyboard); //determina função callback de teclado

    glEnable(GL_DEPTH_TEST); //habilita remoção de superfícies ocultas usando Z-Buffer


 //   glEnable (GL_CULL_FACE); //habilita remoção de superfícies ocultas usando Back Facxe Culling
 //   glCullFace(GL_FRONT);    // Remove as faces da frente
 //   glCullFace(GL_BACK);    // Remove as faces de tras

    glutMainLoop();
    return 0;
}

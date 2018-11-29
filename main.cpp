#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "parser.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

using namespace std;

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void desenhaMenu();
void desenhaEixos();

float width = 1000;
float height = 600;

int contObj = 0; //quantidade de objetos importados. MAX = 3;
const int MAXOBJ = 3;
int num_poligonos = 0;

Modelo modelo1, modelo2, modelo3;
bool wireframe = true;

//posicao do observador (camera)
GLdouble viewer[] = {50.0, 50.0, 50.0};

float colors[3][3] = {{0.96, 0.73, 0.26},
                      {0.84, 0.26, 0.96},
                      {0.26, 0.49, 0.96}};

char texto[100];
char objName[MAXOBJ][200];
char rotacaoTexto[MAXOBJ][4][10];
char translacaoTexto[MAXOBJ][3][10];
char escalaTexto[MAXOBJ][3][10];
char triangulos[50];
char ms[50];

/*
    ESTADO CLICK
    -1: nenhum
    0: campo texto abrir arquivo
    1: botao visibilidade (hidden)
    2: translacao 1 (x)
    3: translacao 2 (y)
    4: translacao 3 (z)
    5: rotacao 1 (angulo)
    6: rotacao 2 (x)
    7: rotacao 3 (y)
    8: rotacao 4 (z)
    9: escala 1 (x)
    10: escala 2 (y)
    11: escala 3 (z)
*/
int estadoClick = -1;

/*
    Define o objeto a qual a caixa de texto clicada pertence
    0 ate MAXOBJ
*/
int estadoClickObj = -1;

float rotacao[MAXOBJ][4] = {0.0};
float translacao[MAXOBJ][3] = {0.0};
float escala[MAXOBJ][3] = {0.0};

char transformacoes[10];
int posTrans = 0;

char nomeArquivo[3][200];
int posNomeArquivo = 0;

int hidden[MAXOBJ] = {0};

int qtdFrames = 0;
int tempoInicial, tempoFinal, diferencaTempo;
float framesPorSegundo;

GLuint texture[1];

void desenhaEixos()
{
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1500.0f, 0.0f, 0.0f);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1500.0f, 0.0f);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1500.0f);
    glEnd();
}

void carregarModelos(string nome, Modelo &modelo, int indice)
{
    nome = "Modelos/cube/cube.obj";
    OBJ obj;
    modelo = obj.lerArquivo(nome);

    glColor3f(colors[indice][0], colors[indice][1], colors[indice][2]);

    glPushMatrix();
    glTranslatef(translacao[indice][0], translacao[indice][1], translacao[indice][2]);
    glRotatef(rotacao[indice][0], rotacao[indice][1], rotacao[indice][2], rotacao[indice][3]);
    glScalef(escala[indice][0], escala[indice][1], escala[indice][2]);
    //glPopMatrix();

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int i = 0; i < modelo.faces.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_POLYGON);
        for (int j = 0; j < modelo.faces[i].vertices.size(); j++)
        {
            //cout << modelo.faces[i].vertices[j].normal << " ";
            glNormal3f(modelo.normal[modelo.faces[i].vertices[j].normal - 1].x,
                       modelo.normal[modelo.faces[i].vertices[j].normal - 1].y,
                       modelo.normal[modelo.faces[i].vertices[j].normal - 1].z);

            glTexCoord2f(modelo.textura[modelo.faces[i].vertices[j].textura - 1].u,
                         modelo.textura[modelo.faces[i].vertices[j].textura - 1].v);

            glVertex3f(modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].x,
                       modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].y,
                       modelo.vertices[modelo.faces[i].vertices[j].vertice - 1].z);
        }
        glEnd();
    }
    glPopMatrix();
}

void objTransparente()
{

    float colors[4] = {0.30, 0.5, 0.27, 0.5};

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor4fv(colors);

    /*glMaterialfv(GL_FRONT, GL_AMBIENT, colors);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colors);*/

    glPushMatrix();

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //frente
    glBegin(GL_POLYGON);

    glNormal3f(0, 0, 1);

    glVertex3f(-30, 30, 20);
    glVertex3f(-30, -30, 20);
    glVertex3f(30, -30, 20);
    glVertex3f(30, 30, 20);
    glEnd();

    //lado esquerdo
    glBegin(GL_POLYGON);

    glNormal3f(-1, 0, 0);

    glVertex3f(-30, 30, 20);
    glVertex3f(-30, 30, 19);
    glVertex3f(-30, -30, 19);
    glVertex3f(-30, -30, 20);
    glEnd();

    //atrás
    glBegin(GL_POLYGON);

    glNormal3f(0, 0, -1);

    glVertex3f(-30, 30, 19);
    glVertex3f(30, 30, 19);
    glVertex3f(30, -30, 19);
    glVertex3f(-30, -30, 19);
    glEnd();

    //lado direito
    glBegin(GL_POLYGON);

    glNormal3f(1, 0, 0);

    glVertex3f(30, 30, 20);
    glVertex3f(30, -30, 20);
    glVertex3f(30, -30, 19);
    glVertex3f(30, 30, 19);
    glEnd();

    //tampa
    glBegin(GL_POLYGON);

    glNormal3f(0, 1, 0);

    glVertex3f(-30, 30, 19);
    glVertex3f(-30, 30, 20);
    glVertex3f(30, 30, 20);
    glVertex3f(30, 30, 19);
    glEnd();

    //baixo
    glBegin(GL_POLYGON);

    glNormal3f(0, -1, 0);

    glVertex3f(-30, -30, 20);
    glVertex3f(-30, -30, 19);
    glVertex3f(30, -30, 19);
    glVertex3f(30, -30, 20);
    glEnd();

    glPopMatrix();
}

void init(void)
{

    int x, y, n;
    unsigned char *data = stbi_load("Modelos/cube/default.png", &x, &y, &n, 0);

    glClearColor(0.0, 0.0, 0.0, 0.0);                   // cor para limpeza do buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    /*********************
    TEXTURA
    **********************/
    glShadeModel(GL_SMOOTH); // Enable Smooth Shading
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create Texture
    glGenTextures(1, texture); // define o numero de texturas

    // Primeira textura
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, texture[0], 3, x, y, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    /***********************
     * ILUMINAÇÃO
    ***********************/

    /*
        LUZ no infinito
    */
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat red_light[] = {1.0, 0.0, 0.0, 0.0};

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_shininess);

    /*
    *********
    */

    for (int k = 0; k < MAXOBJ; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            sprintf(rotacaoTexto[k][i], "0.0");
        }
        for (int i = 0; i < 3; i++)
        {
            sprintf(translacaoTexto[k][i], "0.0");
            sprintf(escalaTexto[k][i], "1.0");
            escala[k][i] = 1.0;
        }

        objName[k][0] = 0;
    }

    sprintf(triangulos, "Poligonos: %d", num_poligonos);
    sprintf(ms, "0.0 ms");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    glViewport(0, 0, 2 * (width / 3), height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, 1, 1, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewer[0], viewer[1], viewer[2], // define posicao do observador
              0.0, 0.0, 0.0,                   // ponto de interesse (foco)
              0.0, 1.0, 0.0);

    if (qtdFrames == 0)
    {
        tempoInicial = glutGet(GLUT_ELAPSED_TIME);
    }
    qtdFrames++;
    tempoFinal = glutGet(GLUT_ELAPSED_TIME);

    diferencaTempo = tempoFinal - tempoInicial;

    if (diferencaTempo >= 1000)
    {
        framesPorSegundo = diferencaTempo / qtdFrames;
        qtdFrames = 0;
        sprintf(ms, "%0.3f ms", framesPorSegundo);
    }

    num_poligonos = 0;

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    for (int j = 0; j < contObj; j++)
    {
        if (objName[j][0] != 0 && !hidden[j])
        {
            carregarModelos(string(objName[j]), modelo1, j);
            num_poligonos += modelo1.faces.size();
            sprintf(triangulos, "Poligonos: %d", num_poligonos);
        }
    }

    desenhaEixos();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    objTransparente();
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glViewport(2 * (width / 3), 0, width / 3, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(2 * (width / 3), width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhaMenu();

    glutSwapBuffers();
    glutPostRedisplay();
}

void desenhaMenu()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

    //os campos aparecem na quantidade de objetos importados.
    for (int l = 0; l < contObj; l++)
    {

        //escala
        glColor3f(1.0, 1.0, 1.0);
        sprintf(texto, "Escala:");
        glPushMatrix();
        glRasterPos2f(676, 452 - (l * 130));
        for (char *i = texto; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
        glPopMatrix();

        glColor3f(0.0, 0.0, 0.0);
        for (int j = 0; j < 3; j++)
        {
            glPushMatrix();
            glRasterPos2f(850 + (j * 50) + 2, 452 - (l * 130));
            for (char *i = escalaTexto[l][j]; *i != 0; i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
            glPopMatrix();
        }

        // 3 campos de texto
        glColor3f(1.0, 1.0, 1.0);
        //campo 3 (escalaZ)
        glBegin(GL_POLYGON);
        glVertex2f(950, 450 - (l * 130));
        glVertex2f(990, 450 - (l * 130));
        glVertex2f(990, 470 - (l * 130));
        glVertex2f(950, 470 - (l * 130));
        glEnd();

        //campo 2 (escalaY)
        glBegin(GL_POLYGON);
        glVertex2f(900, 450 - (l * 130));
        glVertex2f(940, 450 - (l * 130));
        glVertex2f(940, 470 - (l * 130));
        glVertex2f(900, 470 - (l * 130));
        glEnd();

        //campo 1 (escalaX)
        glBegin(GL_POLYGON);
        glVertex2f(850, 450 - (l * 130));
        glVertex2f(890, 450 - (l * 130));
        glVertex2f(890, 470 - (l * 130));
        glVertex2f(850, 470 - (l * 130));
        glEnd();

        //rotacao
        glColor3f(1.0, 1.0, 1.0);
        sprintf(texto, "Rotacao:");
        glPushMatrix();
        glRasterPos2f(676, 482 - (l * 130));
        for (char *i = texto; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
        glPopMatrix();

        glColor3f(0.0, 0.0, 0.0);
        for (int j = 0; j < 4; j++)
        {
            glPushMatrix();
            glRasterPos2f(800 + (j * 50) + 2, 482 - (l * 130));
            for (char *i = rotacaoTexto[l][j]; *i != 0; i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
            glPopMatrix();
        }

        //4 campos de texto
        glColor3f(1.0, 1.0, 1.0);
        //campo 4 (rotacaoZ)
        glBegin(GL_POLYGON);
        glVertex2f(950, 480 - (l * 130));
        glVertex2f(990, 480 - (l * 130));
        glVertex2f(990, 500 - (l * 130));
        glVertex2f(950, 500 - (l * 130));
        glEnd();

        //campo 3 (rotacaoY)
        glBegin(GL_POLYGON);
        glVertex2f(900, 480 - (l * 130));
        glVertex2f(940, 480 - (l * 130));
        glVertex2f(940, 500 - (l * 130));
        glVertex2f(900, 500 - (l * 130));
        glEnd();

        //campo 2 (rotacaoX)
        glBegin(GL_POLYGON);
        glVertex2f(850, 480 - (l * 130));
        glVertex2f(890, 480 - (l * 130));
        glVertex2f(890, 500 - (l * 130));
        glVertex2f(850, 500 - (l * 130));
        glEnd();

        //campo 1 (rotacaoAngulo)
        glBegin(GL_POLYGON);
        glVertex2f(800, 480 - (l * 130));
        glVertex2f(840, 480 - (l * 130));
        glVertex2f(840, 500 - (l * 130));
        glVertex2f(800, 500 - (l * 130));
        glEnd();

        //translacao
        glColor3f(1.0, 1.0, 1.0);
        sprintf(texto, "Translacao:");
        glPushMatrix();
        glRasterPos2f(676, 512 - (l * 130));
        for (char *i = texto; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
        glPopMatrix();

        glColor3f(0.0, 0.0, 0.0);
        for (int j = 0; j < 3; j++)
        {
            glPushMatrix();
            glRasterPos2f(850 + (j * 50) + 2, 512 - (l * 130));
            for (char *i = translacaoTexto[l][j]; *i != 0; i++)
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *i);
            glPopMatrix();
        }

        // 3 campos de texto
        glColor3f(1.0, 1.0, 1.0);
        //campo 3 (translacaoZ)
        glBegin(GL_POLYGON);
        glVertex2f(950, 510 - (l * 130));
        glVertex2f(990, 510 - (l * 130));
        glVertex2f(990, 530 - (l * 130));
        glVertex2f(950, 530 - (l * 130));
        glEnd();

        //campo 2 (translacaoY)
        glBegin(GL_POLYGON);
        glVertex2f(900, 510 - (l * 130));
        glVertex2f(940, 510 - (l * 130));
        glVertex2f(940, 530 - (l * 130));
        glVertex2f(900, 530 - (l * 130));
        glEnd();

        //campo 1 (translacaoX)
        glBegin(GL_POLYGON);
        glVertex2f(850, 510 - (l * 130));
        glVertex2f(890, 510 - (l * 130));
        glVertex2f(890, 530 - (l * 130));
        glVertex2f(850, 530 - (l * 130));
        glEnd();

        //nome arquivo
        glColor3f(0.0, 0.0, 0.0);
        glPushMatrix();
        glRasterPos2f(676, 542 - (l * 130));
        for (char *i = objName[l]; *i != 0; i++)
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
        glPopMatrix();

        // desenha se está hidden ou não
        if (hidden[l])
        {
            glColor3f(0.0, 0.0, 0.0);
            glPushMatrix();
            glRasterPos2f(975, 544 - (l * 130));
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 88);
            glPopMatrix();
        }

        //check box hidden
        glColor3f(0.84, 0.8, 0.86);
        glBegin(GL_POLYGON);
        glVertex2f(970, 540 - (l * 130));
        glVertex2f(990, 540 - (l * 130));
        glVertex2f(990, 560 - (l * 130));
        glVertex2f(970, 560 - (l * 130));
        glEnd();
    }

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

    //texto campo texto abrir arquivo
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glRasterPos2f(676, 574);
    for (char *i = nomeArquivo[contObj]; *i != 0; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *i);
    glPopMatrix();

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

void abrirArquivo()
{
    int j = 0;
    for (char *i = nomeArquivo[contObj]; *i != 0; i++)
    {
        objName[contObj][j] = *i;
        j++;
    }
    objName[contObj][j] = 0;
}

void keyboard(unsigned char key, int x, int y)
{

    if (key == 13)
    { //apertou enter: terminou de escrever o valor da transformacao
        if (estadoClick >= 2 && estadoClick <= 4)
        {
            if (transformacoes[0] != 0)
            {                                                                               //se não estiver vazio, copia o valor
                sscanf(transformacoes, "%f", &translacao[estadoClickObj][estadoClick - 2]); //transforma o valor da trasformacao em float e coloca na variavel certa
                for (int i = 0; i < 10; i++)
                { //copia valor da transformacao para o texto
                    translacaoTexto[estadoClickObj][estadoClick - 2][i] = transformacoes[i];
                    transformacoes[i] = 0;
                }
            }
            else
            { //se estiver vazio o vetor de transformacoes digitado pelo usuário, seta o valor para zero
                sprintf(translacaoTexto[estadoClickObj][estadoClick - 2], "0.0");
                translacao[estadoClickObj][estadoClick - 2] = 0.0;
            }
        }
        else if (estadoClick >= 5 && estadoClick <= 8)
        {
            if (transformacoes[0] != 0)
            {                                                                            //se não estiver vazio, copia o valor
                sscanf(transformacoes, "%f", &rotacao[estadoClickObj][estadoClick - 5]); //transforma o valor da trasformacao em float e coloca na variavel certa
                for (int i = 0; i < 10; i++)
                { //copia valor da transformacao para o texto
                    rotacaoTexto[estadoClickObj][estadoClick - 5][i] = transformacoes[i];
                    transformacoes[i] = 0;
                }
            }
            else
            { //se estiver vazio o vetor de transformacoes digitado pelo usuário, seta o valor para zero
                sprintf(rotacaoTexto[estadoClickObj][estadoClick - 5], "0.0");
                rotacao[estadoClickObj][estadoClick - 2] = 0.0;
            }
        }
        else if (estadoClick >= 9 && estadoClick <= 11)
        {
            if (transformacoes[0] != 0)
            {                                                                           //se não estiver vazio, copia o valor
                sscanf(transformacoes, "%f", &escala[estadoClickObj][estadoClick - 9]); //transforma o valor da trasformacao em float e coloca na variavel certa
                for (int i = 0; i < 10; i++)
                { //copia valor da transformacao para o texto
                    escalaTexto[estadoClickObj][estadoClick - 9][i] = transformacoes[i];
                    transformacoes[i] = 0;
                }
            }
            else
            { //se estiver vazio o vetor de transformacoes digitado pelo usuário, seta o valor para 1
                sprintf(escalaTexto[estadoClickObj][estadoClick - 9], "1.0");
                escala[estadoClickObj][estadoClick - 9] = 1.0;
            }
        }
        posTrans = 0;
    }

    else if (estadoClick >= 2 && estadoClick <= 11 && ((key >= 48 && key <= 57) || key == 46 || key == 45))
    { // Caixas de selecao para transformacoes && (0-9 || . || -)
        transformacoes[posTrans] = key;
        posTrans++;
    }

    else if (estadoClick == 0)
    {
        if (key == 8)
        {
            if (posNomeArquivo >= 0)
            {
                posNomeArquivo--;
                nomeArquivo[contObj][posNomeArquivo] = 0;
            }
        }
        else
        {
            if (posNomeArquivo <= 188)
            {
                nomeArquivo[contObj][posNomeArquivo] = key;
                posNomeArquivo++;
                nomeArquivo[contObj][posNomeArquivo] = 0;
            }
        }
    }

    else if (estadoClick == -1)
    {
        if (key == 'F' || key == 'f')
        {
            wireframe = wireframe ? 0 : 1;
        }

        if (key == 'a' || key == 'A')
            viewer[0] -= 1.0;
        if (key == 'd' || key == 'D')
            viewer[0] += 1.0;
        if (key == 's' || key == 'S')
            viewer[1] -= 1.0;
        if (key == 'w' || key == 'W')
            viewer[1] += 1.0;
    }
}

void mouse(int button, int state, int x, int y)
{
    y = height - y;

    //começa como -1 e se não for detectado que o click foi em alguma caixa, no final ele termina como -1 (click fora de caixa)
    estadoClick = -1;
    estadoClickObj = -1;

    if (x >= 676 && x <= 860 && y >= 570 && y <= 590)
    { //nome arquivo
        estadoClick = 0;
    }

    else if (x >= 870 && x <= 990 && y >= 570 && y <= 590)
    { //botao importar
        if (state == GLUT_DOWN)
        {
            if (contObj < MAXOBJ)
            {
                abrirArquivo();
                contObj++;
                posNomeArquivo = 0;
            }
        }
    }

    else
    {
        for (int l = 0; l < contObj; l++)
        {
            if (x >= 970 && x <= 990 && y >= (540 - (l * 130)) && y <= (560 - (l * 130)))
            { //botao hidden
                estadoClick = 1;
                estadoClickObj = -1;
                if (state == GLUT_DOWN)
                {
                    hidden[l] = hidden[l] ? 0 : 1;
                }
            }
            else if (x >= 850 && x <= 890 && y >= (510 - (l * 130)) && y <= (530 - (l * 130)))
            { //translacao x
                estadoClick = 2;
                estadoClickObj = l;
            }
            else if (x >= 900 && x <= 940 && y >= (510 - (l * 130)) && y <= (530 - (l * 130)))
            { //translacao y
                estadoClick = 3;
                estadoClickObj = l;
            }
            else if (x >= 950 && x <= 990 && y >= (510 - (l * 130)) && y <= (530 - (l * 130)))
            { //translacao z
                estadoClick = 4;
                estadoClickObj = l;
            }
            else if (x >= 800 && x <= 840 && y >= (480 - (l * 130)) && y <= (500 - (l * 130)))
            { //rot angulo
                estadoClick = 5;
                estadoClickObj = l;
            }
            else if (x >= 850 && x <= 890 && y >= (480 - (l * 130)) && y <= (500 - (l * 130)))
            { //rot x
                estadoClick = 6;
                estadoClickObj = l;
            }
            else if (x >= 900 && x <= 940 && y >= (480 - (l * 130)) && y <= (500 - (l * 130)))
            { //rot y
                estadoClick = 7;
                estadoClickObj = l;
            }
            else if (x >= 950 && x <= 990 && y >= (480 - (l * 130)) && y <= (500 - (l * 130)))
            { //rot z
                estadoClick = 8;
                estadoClickObj = l;
            }
            else if (x >= 850 && x <= 890 && y >= (450 - (l * 130)) && y <= (470 - (l * 130)))
            { //escala x
                estadoClick = 9;
                estadoClickObj = l;
            }
            else if (x >= 900 && x <= 940 && y >= (450 - (l * 130)) && y <= (470 - (l * 130)))
            { //escala y
                estadoClick = 10;
                estadoClickObj = l;
            }
            else if (x >= 950 && x <= 990 && y >= (450 - (l * 130)) && y <= (470 - (l * 130)))
            { //escala z
                estadoClick = 11;
                estadoClickObj = l;
            }
        }
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);                                    //inicializa a glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores
    glutInitWindowSize(width, height);                        //dimens�es da janela
    glutInitWindowPosition(100, 50);                          //posicao da janela
    glutCreateWindow("Trabalho 3");                           //cria a janela

    init();
    glutDisplayFunc(display);   //determina fun��o callback de desenho
    glutKeyboardFunc(keyboard); //determina fun��o callback de teclado
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST); //habilita remo��o de superf�cies ocultas usando Z-Buffer

    glutMainLoop();
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/gl.h>

using namespace std;

struct Vertice
{
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat z = 0.0;
    GLfloat w = 1.0;
};

struct Normal
{
    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat z = 0.0;
    GLfloat w = 1.0;   
};

struct Textura
{
    GLfloat u = 0.0;
    GLfloat v = 0.0;
};

struct Vertice_Face
{
    int vertice = 0;
    int textura = 0;
    int normal = 0;
};

struct Face
{
    vector<Vertice_Face> vertices;
};

struct Modelo
{
    string mtl;
    vector<Vertice> vertices;
    vector<Textura> textura;
    vector<Normal> normal;
    vector<Face> faces;
};

Modelo modelo; Vertice vertice; Normal normal; Textura textura; Vertice_Face vf; Face face; 

void insertFace(int insercao, int aux)
{
    switch(insercao)
    {
        case 0:
            vf.vertice = aux;
            break;
        case 1: 
            vf.textura = aux;
            break;
        case 2:
            vf.normal = aux;
            break;
    }
}

void lerArquivo(string nome)
{
    ifstream arquivo;
    arquivo.open(nome);
    char item[100], c;

    if (arquivo.is_open()) {
        while (!arquivo.eof()) {
            arquivo >> item;
            if(!strcmp(item,"mtlib"))
            {
                arquivo >> modelo.mtl;
            }
            else if(!strcmp(item,"v"))
            {
                arquivo >> vertice.x;
                arquivo >> vertice.y;
                arquivo >> vertice.z;
                arquivo.get(c);
                if (c != '\n')
                {
                    arquivo >> vertice.w;
                }
                modelo.vertices.push_back(vertice);
            }
            else if(!strcmp(item,"vn"))
            {
                arquivo >> normal.x;
                arquivo >> normal.y;
                arquivo >> normal.z;
                arquivo.get(c);
                if (c != '\n')
                {
                    arquivo >> normal.w;
                }
                modelo.normal.push_back(normal);
            }
            else if(!strcmp(item,"vt"))
            {
                arquivo >> textura.u;
                arquivo >> textura.v;
                modelo.textura.push_back(textura);
            }
            else if(!strcmp(item,"f"))
            {   
                char aux[100]; int i = 0, insercao = 0;
                bool verifica = false;
                arquivo.get(c); //ignorar espaço em branco
                arquivo.get(c);
                while(true)
                {
                    if(isdigit(c))
                    {
                        aux[i++] = c; 
                        verifica = true;
                    }
                    else if(c=='/')
                    {
                        aux[i++] = '\0';
                        arquivo.get(c);
                        if(c=='/')
                        {
                            vf.vertice = atoi(aux);
                            insercao+=2;
                            i = 0;
                        }
                        else
                        {
                            insertFace(insercao, atoi(aux));
                            insercao++;
                            aux[0] = c;
                            i = 1;
                        }
                        verifica = true;
                    }
                    else if (c == ' ')
                    {
                        aux[i++] = '\0';
                        insertFace(insercao, atoi(aux));
                        insercao = 0;
                        i = 0;
                        verifica = true;
                        face.vertices.push_back(vf);
                    }
                    else if (c == '\n')
                    {
                        aux[i++] = '\0';
                        if (verifica)
                        {
                            insertFace(insercao, atoi(aux));
                            face.vertices.push_back(vf);
                            modelo.faces.push_back(face);
                            face.vertices.clear();
                        }
                        break;
                    }
                    arquivo.get(c);
                }
            }
        }
    }
    else cout << "Erro ao abrir o arquivo";
    arquivo.close();
}

int main () {

    lerArquivo("Modelos/teapot.obj");

    return 0;
}
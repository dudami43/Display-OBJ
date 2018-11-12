#ifndef PARSER_H
#define PARSER_H

#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

class OBJ
{
  private:
    void insertFace(int insercao, int aux, Vertice_Face &vf)
    {
        switch (insercao)
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
    void ajusteFaces(Modelo &modelo)
    {
        int min = modelo.faces[0].vertices[0].vertice;
        for (int i = 0; i < modelo.faces.size(); i++)
        {
            for (int j = 0; j < modelo.faces[i].vertices.size(); j++)
            {
                //cout << modelo.faces[i].vertices[j].vertice << endl;
                if (modelo.faces[i].vertices[j].vertice < min)
                    min = modelo.faces[i].vertices[j].vertice;
            }
        }
        if (min < 0)
        {
            for (int i = 0; i < modelo.faces.size(); i++)
            {
                for (int j = 0; j < modelo.faces[i].vertices.size(); j++)
                {
                    //cout << modelo.faces[i].vertices[j].vertice << endl;
                    modelo.faces[i].vertices[j].vertice += 1 - min;
                }
            }
        }
    }

  public:
    Modelo lerArquivo(string nome)
    {
        Modelo modelo;
        Vertice vertice;
        Normal normal;
        Textura textura;
        Vertice_Face vf;
        Face face;
        ifstream arquivo;
        arquivo.open(nome.c_str());
        char item[1000000], c;

        if (arquivo.is_open())
        {
            while (!arquivo.eof())
            {
                arquivo >> item;
                if (!strcmp(item, "mtllib"))
                {
                    arquivo >> modelo.mtl;
                }
                else if (!strcmp(item, "v"))
                {
                    arquivo >> vertice.x;
                    arquivo >> vertice.y;
                    arquivo >> vertice.z;
                    //cout << "v " << vertice.x << " " << vertice.y << " " << vertice.z << endl;
                    arquivo.get(c);
                    if (c != '\n')
                    {
                        arquivo >> vertice.w;
                    }
                    modelo.vertices.push_back(vertice);
                }
                else if (!strcmp(item, "vn"))
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
                else if (!strcmp(item, "vt"))
                {
                    arquivo >> textura.u;
                    arquivo >> textura.v;
                    modelo.textura.push_back(textura);
                }
                else if (!strcmp(item, "f"))
                {
                    char aux[100];
                    int i = 0, insercao = 0;
                    bool verifica = false;
                    arquivo.get(c); // ignorar espaço em branco
                    arquivo.get(c);
                    while (true)
                    {
                        if (isdigit(c) || c == '-')
                        {
                            aux[i++] = c;
                            verifica = true;
                        }
                        else if (c == '/')
                        {
                            aux[i++] = '\0';
                            arquivo.get(c);
                            if (c == '/')
                            {
                                vf.vertice = atoi(aux);
                                insercao += 2;
                                i = 0;
                            }
                            else
                            {
                                insertFace(insercao, atoi(aux), vf);
                                insercao++;
                                aux[0] = c;
                                i = 1;
                            }
                            verifica = true;
                        }
                        else if (c == ' ')
                        {
                            aux[i++] = '\0';
                            insertFace(insercao, atoi(aux), vf);
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
                                insertFace(insercao, atoi(aux), vf);
                                face.vertices.push_back(vf);
                                modelo.faces.push_back(face);
                                face.vertices.clear();
                            }
                            break;
                        }
                        arquivo.get(c);
                    }
                }
                else
                {
                    cout << item << endl;
                }
            }
        }
        else
        {
            cout << "Erro ao abrir o arquivo";
        }
        ajusteFaces(modelo);
        for (int i = 0; i < modelo.faces.size(); i++)
        {
            for (int j = 0; j < modelo.faces[i].vertices.size(); j++)
            {
                cout << modelo.faces[i].vertices[j].vertice << " ";
            }
            cout << endl;
        }
        arquivo.close();
        return modelo;
    }
};

#endif

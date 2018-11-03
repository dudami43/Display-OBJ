#include <iostream>
#include <stdlib.h>
#include "parser.h"

using namespace std;

int main(int argc, char **argv)
{
    OBJ obj;
    Modelo modelo;
    modelo = obj.lerArquivo("Modelos/teapot.obj");
    for (int i = 0; i < modelo.faces.size(); i++)
    {
        cout << modelo.faces[i].vertices[0].vertice << endl;
    }
    return 0;
}

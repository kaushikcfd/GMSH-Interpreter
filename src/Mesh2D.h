//
// Created by kaushikcfd on 13/12/16.
//
#ifndef GMSH_INTERPRETER_MESH2D_H
#define GMSH_INTERPRETER_MESH2D_H
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <string>
#include <cstring>

#include "Node.h"
#include "Element.h"

using namespace std;

class Mesh2D {
private:
    unsigned noNodes, noElements;
    Node **nodes;
    Element *elements;

    void allocateNodes();
    void allocateElements();


public:
    Mesh2D();
    void readFromFile(string filename);
    int getNoOfNodes();
    int getNoOfElements();
    int whichNodeOfElement(unsigned node_index, unsigned element_index);

    Node* getNodes();
    int** getElementNodes();
    ~Mesh2D();
};

#endif //GMSH_INTERPRETER_MESH2D_H

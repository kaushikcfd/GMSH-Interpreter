//
// Created by kaushikcfd on 7/1/17.
//


#ifndef GMSH_INTERPRETER_ELEMENT_H
#define GMSH_INTERPRETER_ELEMENT_H

class Node; // Forward declaration
/**
 * Currently the element is only able to handle triangular elements
 * */

class Element {
private:
    Node** nodes; // Currently, there is only support for triangular elements
    float vertexAngles[3];

    int whichNodeOfElement(const Node &_node);

public:
    Element();
    Node* operator[](int node_index);
    void assigningOtherCoords(const Node &_node, float &x1, float &y1, float &x2, float &y2);
};
#endif //GMSH_INTERPRETER_ELEMENT_H

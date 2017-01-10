//
// Created by kaushikcfd on 13/12/16.
//

#include "Mesh2D.h"

Mesh2D::Mesh2D() {
    noNodes = 0;
    noElements = 0;
}

/**
 * This function is used to allocate memory for the `nodes` array once the number of nodes is known.
 */
void Mesh2D::allocateNodes() {
    nodes = new Node*[noNodes];
    return ;
}

/**
 * This function is used to allocate memort for the `elements` array once the number of elements is known.
 */
void Mesh2D::allocateElements() {
    elements = new Element[noElements];
    return ;
}


void Mesh2D::readFromFile(string filename) {
    FILE *pFile = freopen(filename.c_str(), "r", stdin);
    string s;
    int i, node_no, elm_no, elm_type, tag1, tag2, tag3;
    float node_x, node_y, node_z;// This will always be zeros, as this is the 2D case
    int node_index1, node_index2, node_index3;
    float thetaStart, thetaEnd;

    for(i=0; i<4; i++)// Omitting the top 4 lines, which describe the Mesh Format
        getline(cin, s);

    scanf("%u", &noNodes);// Reading the number of nodes involved in the mesh
    allocateNodes();

    for(i=0; i<noNodes; i++) {
        scanf("%d %f %f %f", &node_no, &node_x, &node_y, &node_z);// Reading the x,y,z of each node.
        nodes[i]->updateCoords(node_x, node_y);
    }

    for(i=0; i<3; i++)// Omitting the 2 lines, which describe the syntax of a .msh file
        getline(cin, s);

    scanf("%u", &noElements);// Reading the number of elements that are to be read.

    allocateElements();
    for(i=0;i<noElements;i++) {
        scanf("%d %d",&elm_no, &elm_type);
        if(elm_type == 2){
            // Reading the three nodes of each elements.
            scanf("%d %d %d %d %d %d", &tag1, &tag2, &tag3, &node_index1, &node_index2, &node_index3);
            memcpy(elements[i][0], nodes[node_index1], sizeof(Node*));
            memcpy(elements[i][1], nodes[node_index2], sizeof(Node*));
            memcpy(elements[i][2], nodes[node_index3], sizeof(Node*));

            for (int j=0; j<3; j++) {
                --elementNodes[i][j];// This is because the numbering of the node should be chosen from 0 based indexing.
                ++n_nodes_nbgElements[elementNodes[i][j]];// Increasing the nbg nodes for each
            }
        }

        else{
            // Omitting the line, as other elements are of no use.
            getline(cin, s);
            --i;
            --n_elements;
        }
    }

    // Allocating memory for the nbg nodes array.
    nodes_nbgElements = new int* [n_nodes];
    nodes_nbgThetaStart = new float* [n_nodes];
    nodes_nbgThetaEnd = new float* [n_nodes];
    int *currentIndex = new int[n_nodes];
    for(i=0; i< n_nodes; i++ ) {
        nodes_nbgElements[i] = new int[n_nodes_nbgElements[i]];
        nodes_nbgThetaStart[i] = new float [n_nodes_nbgElements[i]];
        nodes_nbgThetaEnd[i] = new float [n_nodes_nbgElements[i]];
        currentIndex[i] = 0;
    }

    for(i=0; i< n_elements; i++){
        for(int j=0; j<3; j++) {
            nodes_nbgElements[elementNodes[i][j]][currentIndex[elementNodes[i][j]]] = i;
            ++currentIndex[elementNodes[i][j]];
        }
    }

    // Updating the float **nodes_thetaStart; float ** nodes_thetaEnd;
    for(i=0; i< n_nodes; i++) {
        float x_current = nodes_x[i];
        float y_current = nodes_y[i];
        float x1, y1, x2, y2; // Co-ordinates of the other two noes in the given element.
        for(int j =0; j < n_nodes_nbgElements[i]; j++) {
            int nbgElement_index = nodes_nbgElements[i][j];
            assigningOtherCoords((unsigned)i, (unsigned)nbgElement_index, x1, y1, x2, y2);

            thetaStart = ANGLE(atan2(y1 - y_current, x1- x_current)*180.0/3.1415926);
            thetaEnd = ANGLE(atan2(y2 - y_current, x2- x_current)*180.0/3.1415926);
            if(!((((thetaEnd - thetaStart) > 0) && ((thetaEnd - thetaStart) < 180)) ||(((thetaEnd - thetaStart) > -360) && ((thetaEnd - thetaStart) < -180)) ))
                swap(thetaEnd, thetaStart);

            nodes_nbgThetaStart[i][j] = thetaStart;
            nodes_nbgThetaEnd[i][j] = thetaEnd;
        }
    }


    delete [] currentIndex;
    fclose(pFile);
    return;
}

int Mesh2D::getNoOfNodes(){
    return n_nodes;
}
int Mesh2D::getNoOfElements(){
    return n_elements;
}

float* Mesh2D::getNodeXAsArray(){
    return  nodes_x;
}
float* Mesh2D::getNodeYAsArray(){
    return  nodes_y;
}

int* Mesh2D::getNoOfNbgElements(){
    return  n_nodes_nbgElements;
}

int** Mesh2D::getNbgElements(){
    return  nodes_nbgElements;
}
int** Mesh2D::getElementNodes(){
    return  elementNodes;
}

float** Mesh2D::getNodesNbgThetaStart() {
    return nodes_nbgThetaStart;
}

float** Mesh2D::getNodesNbgThetaEnd() {
    return nodes_nbgThetaEnd;
}

Mesh2D::~Mesh2D() {
    delete [] nodes_x;
    delete [] nodes_y;
    for(int i=0; i< n_nodes; i++) {
        delete [] nodes_nbgElements[i];
        delete [] nodes_nbgThetaStart[i];
        delete [] nodes_nbgThetaEnd[i];
        delete [] elementNodes[i];
    }
}

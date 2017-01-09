//
// Created by kaushikcfd on 13/12/16.
//

#include "Mesh2D.h"

Mesh2D::Mesh2D() {
    noNodes = 0;
    noElements = 0;
}

void Mesh2D::readFromFile(string filename) {
    FILE *pFile = freopen(filename.c_str(), "r", stdin);
    string s;
    int i, node_no, elm_no, elm_type, tag1, tag2, tag3;
    float z;// This will always be zeros, as this is the 2D case
    float thetaStart, thetaEnd;

    for(i=0; i<4; i++)// Omitting the top 4 lines, which describe the Mesh Format
        getline(cin, s);

    scanf("%u", &n_nodes);// Reading the number of nodes involved in the mesh
    nodes_x = allocateFloatArray(n_nodes);// Allocating the memory space for x-cordinates of the nodes.
    nodes_y = allocateFloatArray(n_nodes);// Allocating the memory space for y-cordinates of the nodes.
    n_nodes_nbgElements = allocateIntArray(n_nodes);// Allocating the memory space for the nbg. elements of the nodes.

    for(i=0; i<n_nodes; i++) {
        scanf("%d %f %f %f", &node_no, &nodes_x[i], &nodes_y[i], &z);// Reading the x,y,z of each node.
        n_nodes_nbgElements[i] = 0;
    }

    for(i=0; i<3; i++)// Omitting the 2 lines, which describe the syntax of a .msh file
        getline(cin, s);

    scanf("%u", &n_elements);// Reading the number of elements that are to be read.

    allocateElementArray();
    for(i=0;i<n_elements;i++) {
        scanf("%d %d",&elm_no, &elm_type);
        if(elm_type == 2){
            // Reading the three nodes of each elements.
            scanf("%d %d %d %d %d %d", &tag1, &tag2, &tag3, &elementNodes[i][0], &elementNodes[i][1], &elementNodes[i][2]);
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

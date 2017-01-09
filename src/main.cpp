#include <iostream>
#include "Mesh2D.h"

#define FILE_PATH "/home/kaushikcfd/MyStuff/Dropbox/Infi-EntertainmentStuff/IITB-AcadStuff/Baskar_Project/gmsh_interpreter/square-structured.msh"

void displayArray(float *x, unsigned n)
{
    unsigned i;
    for(i=0; i<n; i++)
        printf("%d\t%6.2f\n",i+1, x[i]);
}

void displayMatrix(int **A, unsigned m, unsigned n)
{
    unsigned i,j;
    for(i=0; i<m; i++)
    {
        for(j=0;j<n;j++)
            printf("%d\t",A[i][j]);
        printf("\n");
    }
}

int main() {
    Mesh2D mesh;
    mesh.readFromFile(FILE_PATH);
    float **A = mesh.getNodesNbgThetaStart();
    float **B = mesh.getNodesNbgThetaEnd();
    int **C = mesh.getNbgElements();
    int *number = mesh.getNoOfNbgElements();

    unsigned i,j;
    for(i=0; i<mesh.getNoOfNodes(); i++)
    {
        printf("Node: %d:\n", i+1);
        for(j=0;j<number[i];j++)
            printf("%6.2f\t",A[i][j]);
        printf("\n");
        for(j=0;j<number[i];j++)
            printf("%6.2f\t",B[i][j]);
        printf("\n------------------------------------\n");
    }

    return 0;
}
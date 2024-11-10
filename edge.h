#ifndef EDGE_H
#define EDGE_H

#include "vertice.h"

class Face;

class Edge{
private:
    Vertice *_vOrigin = nullptr, *_vDestination = nullptr;
    Face *_fLeft = nullptr, *_fRight = nullptr;
    Edge *_eLeft_In = nullptr, *_eLeft_Out = nullptr, *_eRight_In = nullptr, *_eRight_Out = nullptr;
public:
    Edge(Vertice *vertice_Origin = nullptr, Vertice *vertice_Destination = nullptr,
         Face *face_left = nullptr, Face *face_right = nullptr,
         Edge *left_in = nullptr, Edge *left_out = nullptr,
         Edge *right_in = nullptr, Edge *right_out = nullptr);

    void SetVerticeOrigin(Vertice *v);
    void SetVerticeDestination(Vertice *v);
    void SetFaceLeft(Face *f);
    void SetFaceRight(Face *f);
    void SetEdgeLeftIn(Edge *e);
    void SetEdgeLeftOut(Edge *e);
    void SetEdgeRightIn(Edge *e);
    void SetEdgeRightOut(Edge *e);
    Vertice &GetVerticeOrigin();
    Vertice &GetVerticeDestination();
    Face &GetFaceLeft();
    Face &GetFaceRight();
    Edge &GetEdgeLeftIn();
    Edge &GetEdgeLeftOut();
    Edge &GetEdgeRightIn();
    Edge &GetEdgeRightOut();
};

#endif // EDGE_H

#ifndef EDGE_H
#define EDGE_H

#include "vertice.h"
#include "face.h"

class Edge{
private:
    Vertice *_vUp, *_vDown;
    Face *_fLeft, *_fRight;
    Edge *_eLeft_Up, *_eLeft_Down, *_eRight_Up, *_eRight_Down;
public:
    Edge(Vertice *vertice_Up = nullptr,
         Vertice *vertice_Down = nullptr,
         Face *face_left = nullptr,
         Face *face_right = nullptr,
         Edge *left_up = nullptr,
         Edge *left_down = nullptr,
         Edge *right_up = nullptr,
         Edge *right_down = nullptr);

    bool operator==(Edge &other);

    bool HasNullFace();
    bool IsEquivalent(Vertice *v1, Vertice *v2);
    bool SetNullSide(Face *f, Edge *e1, Edge *e2);
    bool SetEdge(Edge *e, Face *f);
    bool HasVertice(Vertice *v);

    void SetVerticeUp(Vertice *v);
    void SetVerticeDown(Vertice *v);
    void SetFaceLeft(Face *f);
    void SetFaceRight(Face *f);
    void SetEdgeLeftUp(Edge *e);
    void SetEdgeLeftDown(Edge *e);
    void SetEdgeRightUp(Edge *e);
    void SetEdgeRightDown(Edge *e);
    Vertice *GetSharedVertice(Edge *other);
    Vertice *GetVerticeUp();
    Vertice *GetVerticeDown();
    Face *GetFaceLeft();
    Face *GetFaceRight();
    Face *GetValidFace();
    Edge *GetSomeNextEdge(Face *f);
    Edge *GetNextEdge(Edge *before, Face *f);
    Edge *GetNextEdge(Edge *before, Vertice *v);
    Edge *GetEdgeLeftUp();
    Edge *GetEdgeLeftDown();
    Edge *GetEdgeRightUp();
    Edge *GetEdgeRightDown();
    Edge *GetRightEdge(Vertice *v);
    Edge *GetLeftEdge(Vertice *v);
};

#endif // EDGE_H

#ifndef EDGE_H
#define EDGE_H

#include <QString>

#include "vertice.h"
#include "face.h"

class Edge{
private:
    QString _id;
    Vertice *_vUp, *_vDown;
    Face *_fLeft, *_fRight;
    Edge *_eLeft_Up, *_eLeft_Down, *_eRight_Up, *_eRight_Down;
public:
    Edge(QString id = QString(),
         Vertice *vertice_Up = nullptr,
         Vertice *vertice_Down = nullptr,
         Face *face_left = nullptr,
         Face *face_right = nullptr,
         Edge *left_up = nullptr,
         Edge *left_down = nullptr,
         Edge *right_up = nullptr,
         Edge *right_down = nullptr);

    bool operator==(Edge &other);

    bool HasNullFace();
    bool HasFace(Face *f);
    bool IsFaceRight(Face *f);
    bool IsFaceLeft(Face *f);
    bool IsEquivalent(Vertice *v1, Vertice *v2);
    bool SetNullSide(Face *f, Edge *e1, Edge *e2);
    bool AddEdge(Edge *e, Face *f);
    bool ReplaceEdgeNotFacing(Edge *from, Edge *to, Face *f);
    Edge *SetEdge(Edge *e, Face *f);
    Edge *SetEdge(Edge *e);
    bool SetEdgeIfNull(Edge *e, Face *f);
    bool SetOpositeEdge(Edge *e, Face *f);
    bool HasVertice(Vertice *v);
    bool HasEdgePlaced(Edge *e, Face *f);

    void SetId(QString id);
    void SetVerticeUp(Vertice *v);
    void SetVerticeDown(Vertice *v);
    void SetFaceLeft(Face *f);
    void SetFaceRight(Face *f);
    void SetEdgeLeftUp(Edge *e);
    void SetEdgeLeftDown(Edge *e);
    void SetEdgeRightUp(Edge *e);
    void SetEdgeRightDown(Edge *e);
    QString GetId() const;
    Vertice *GetSharedVertice(Edge *other);
    Vertice *GetVerticeUp() const;
    Vertice *GetVerticeDown() const;
    Face *GetFaceLeft() const;
    Face *GetFaceRight() const;
    Edge *GetEdge(Vertice *v, Face *f);
    Edge *GetOpositeEdge(Edge* e);
    Edge *GetSomeNextEdge(const Face *f);
    Edge *GetNextEdge(Edge *before, const Face *f);
    Edge *GetNextEdge(Edge *before, const Vertice *v);
    Edge *GetEdgeLeftUp() const;
    Edge *GetEdgeLeftDown() const;
    Edge *GetEdgeRightUp() const;
    Edge *GetEdgeRightDown() const;
    Edge *GetRightEdge(const Vertice *v);
    Edge *GetLeftEdge(const Vertice *v);
};

#endif // EDGE_H

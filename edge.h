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
    Vertice &GetVerticeOrigin() const;
    Vertice &GetVerticeDestination() const;
    Face &GetFaceLeft() const;
    Face &GetFaceRight() const;
    Edge &GetEdgeLeftIn() const;
    Edge &GetEdgeLeftOut() const;
    Edge &GetEdgeRightIn() const;
    Edge &GetEdgeRightOut() const;
};

inline bool operator==(const Edge &v1, const Edge &v2){
    return ( (v1.GetVerticeOrigin() == v2.GetVerticeOrigin()) &&
             (v1.GetVerticeDestination() == v2.GetVerticeDestination()) ) ||
           ( (v1.GetVerticeOrigin() == v2.GetVerticeDestination())&&
             (v1.GetVerticeDestination() == v2.GetVerticeOrigin()) );
}

inline size_t qHash(const Edge &key, size_t seed){
    uint32_t v1, v2;

    v1 = qHash(key.GetVerticeOrigin(),seed);
    v2 = qHash(key.GetVerticeDestination(),seed);

    return qHash((v1^v2), seed);
}

#endif // EDGE_H

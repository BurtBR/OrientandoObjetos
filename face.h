#ifndef FACE_H
#define FACE_H

#include "edge.h"

class Face{
private:
    Edge *_edge = nullptr;
public:
    Face(Edge *edge = nullptr);

    void SetEdge(Edge *e);
    Edge &GetEdge();
};

#endif // FACE_H

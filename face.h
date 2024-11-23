#ifndef FACE_H
#define FACE_H

class Edge;

class Face{
private:
    Edge *_edge;
public:
    Face(Edge *edge = nullptr);

    void SetEdge(Edge *e);
    Edge *GetEdge();
};

#endif // FACE_H

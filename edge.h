#ifndef EDGE_H
#define EDGE_H

#include <cstddef>

// TEMP
#include <QDebug>

class Edge{
private:
    size_t _vOrigin, _vDestination, _fLeft, _fRight, _eLeft_In, _eLeft_Out, _eRight_In, _eRight_Out;
public:
    Edge(size_t vertice_Origin = -1, size_t vertice_Destination = -1,
         size_t face_left = -1, size_t face_right = -1,
         size_t left_in = -1, size_t left_out = -1,
         size_t right_in = -1, size_t right_out = -1);

    bool operator==(const Edge &other) const;

    void SetVerticeOrigin(size_t v);
    void SetVerticeDestination(size_t v);
    void SetFaceLeft(size_t f);
    void SetFaceRight(size_t f);
    void SetEdgeLeftIn(size_t e);
    void SetEdgeLeftOut(size_t e);
    void SetEdgeRightIn(size_t e);
    void SetEdgeRightOut(size_t e);
    size_t GetVerticeOrigin() const;
    size_t GetVerticeDestination() const;
    size_t GetFaceLeft() const;
    size_t GetFaceRight() const;
    size_t GetEdgeLeftIn() const;
    size_t GetEdgeLeftOut() const;
    size_t GetEdgeRightIn() const;
    size_t GetEdgeRightOut() const;
};

#endif // EDGE_H

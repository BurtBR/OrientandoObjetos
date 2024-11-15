#ifndef VERTICE_H
#define VERTICE_H

#include <cstddef>

class Edge;

class Vertice{
private:
    float _x=0, _y=0, _z=1, _w = 1.0;
    size_t _incident;
public:
    Vertice(float x=0, float y=0, float z=0, float w = 1.0, size_t incident = -1);
    Vertice(const Vertice &other);

    bool operator==(const Vertice &other) const;

    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);
    void SetXYZW(float x, float y, float z, float w);
    void SetIncidentEdge(size_t e);
    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetW() const;
    size_t GetIncidentEdge();
};

#endif // VERTICE_H

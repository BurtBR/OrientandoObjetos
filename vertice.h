#ifndef VERTICE_H
#define VERTICE_H

// TEMP
#include <QDebug>

#include <cstddef>
#include <cstdint>
#include <cstring>

class Edge;

class Vertice{
private:
    float _x=0, _y=0, _z=1, _w = 1.0;
    size_t _incident;
public:
    Vertice(float x=0, float y=0, float z=0, float w = 1.0, size_t incident = -1);
    Vertice(const Vertice &other);

    bool operator==(const Vertice &other);

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

inline bool operator==(const Vertice &v1, const Vertice &v2){
    bool result = ( (v1.GetX() == v2.GetX()) &&
                   (v1.GetY() == v2.GetY()) &&
                   (v1.GetZ() == v2.GetZ()) &&
                   (v1.GetW() == v2.GetW()) );

    return result;
}

#endif // VERTICE_H

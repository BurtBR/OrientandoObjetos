#ifndef VERTICE_H
#define VERTICE_H

#include <cstddef>
#include <QGenericMatrix>

class Edge;

class Vertice{
private:
    QGenericMatrix<1, 4, float> _position;
    size_t _incident;
public:
    Vertice(float x=0, float y=0, float z=0, float w = 1.0, size_t incident = -1);
    Vertice(const Vertice &other);

    bool operator==(const Vertice &other) const;

    void SetPosition(QGenericMatrix<1, 4, float> pos);
    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);
    void SetXYZW(float x, float y, float z, float w);
    void SetIncidentEdge(size_t e);
    QGenericMatrix<1, 4, float> GetPosition() const;
    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetW() const;
    size_t GetIncidentEdge() const;
};

#endif // VERTICE_H

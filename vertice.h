#ifndef VERTICE_H
#define VERTICE_H

#include <QGenericMatrix>

class Edge;

class Vertice{
private:
    QGenericMatrix<1, 4, float> _position;
    Edge *_incident = nullptr;
public:
    Vertice(float x=0, float y=0, float z=0, float w = 1.0, Edge *incident = nullptr);
    Vertice(const Vertice &other);

    bool operator==(const Vertice &other) const;

    bool HasIncidentEdge();
    void SetPosition(QGenericMatrix<1, 4, float> pos);
    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);
    void SetXYZW(float x, float y, float z, float w);
    void SetIncidentEdge(Edge *e);
    QGenericMatrix<1, 4, float> GetPosition();
    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetW() const;
    Edge *GetIncidentEdge() const;
};

#endif // VERTICE_H

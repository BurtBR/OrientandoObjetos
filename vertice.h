#ifndef VERTICE_H
#define VERTICE_H

class Edge;

class Vertice{
private:
    float _x=0, _y=0, _z=1, _w = 1.0;
    Edge *_incident = nullptr;
public:
    Vertice(float x=0, float y=0, float z=0, float w = 1.0, Edge *incidentedge = nullptr);

    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);
    void SetXYZ(float x, float y, float z);
    void SetIncidentEdge(Edge *e);
    float GetX();
    float GetY();
    float GetZ();
    float GetW();
    Edge &GetIncidentEdge();
};

#endif // VERTICE_H

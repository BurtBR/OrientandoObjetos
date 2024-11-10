#include "vertice.h"


Vertice::Vertice(float x, float y, float z, float w, Edge *incidentedge){
    SetX(x);
    SetY(y);
    SetZ(z);
    SetW(w);
    SetIncidentEdge(incidentedge);
}

Vertice::Vertice(const Vertice &other){
    SetX(other._x);
    SetY(other._y);
    SetZ(other._z);
    SetW(other._w);
    SetIncidentEdge(other._incident);
}

void Vertice::SetX(float x){
    _x = x;
}

void Vertice::SetY(float y){
    _y = y;
}

void Vertice::SetZ(float z){
    _z = z;
}

void Vertice::SetW(float w){
    _w = w;
}

void Vertice::SetXYZW(float x, float y, float z, float w){
    SetX(x);
    SetY(y);
    SetZ(z);
    SetW(w);
}

void Vertice::SetIncidentEdge(Edge *e){
    _incident = e;
}

float Vertice::GetX() const{
    return _x;
}

float Vertice::GetY() const{
    return _y;
}

float Vertice::GetZ() const{
    return _z;
}

float Vertice::GetW() const{
    return _w;
}

Edge &Vertice::GetIncidentEdge(){
    return *_incident;
}

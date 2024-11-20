#include "vertice.h"


Vertice::Vertice(float x, float y, float z, float w, size_t incident){
    SetX(x);
    SetY(y);
    SetZ(z);
    SetW(w);
    SetIncidentEdge(incident);
}

Vertice::Vertice(const Vertice &other){
    SetX(other.GetX());
    SetY(other.GetY());
    SetZ(other.GetZ());
    SetW(other.GetW());
    SetIncidentEdge(other.GetIncidentEdge());
}

bool Vertice::operator==(const Vertice &other) const{
    return(GetX() == other.GetX() &&
           GetY() == other.GetY() &&
           GetZ() == other.GetZ() &&
           GetW() == other.GetW());
}

void Vertice::SetPosition(QGenericMatrix<4, 1, float> pos){
    _position = pos;
}

void Vertice::SetX(float x){
    _position.data()[0] = x;
}

void Vertice::SetY(float y){
    _position.data()[1] = y;
}

void Vertice::SetZ(float z){
    _position.data()[2] = z;
}

void Vertice::SetW(float w){
    _position.data()[3] = w;
}

void Vertice::SetXYZW(float x, float y, float z, float w){
    SetX(x);
    SetY(y);
    SetZ(z);
    SetW(w);
}

void Vertice::SetIncidentEdge(size_t e){
    _incident = e;
}

QGenericMatrix<4, 1, float> Vertice::GetPosition() const{
    return _position;
}

float Vertice::GetX() const{
    return _position.constData()[0];
}

float Vertice::GetY() const{
    return _position.constData()[1];
}

float Vertice::GetZ() const{
    return _position.constData()[2];
}

float Vertice::GetW() const{
    return _position.constData()[3];
}

size_t Vertice::GetIncidentEdge() const{
    return _incident;
}

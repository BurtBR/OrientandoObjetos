#include "vertice.h"

Vertice::Vertice(float x, float y, float z, float w, Edge *incidentedge){
    _x = x;
    _y = y;
    _z = z;
    _w = w;
    _incident = incidentedge;
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

void Vertice::SetXYZ(float x, float y, float z){
    _x = x;
    _y = y;
    _z = z;
}

void Vertice::SetIncidentEdge(Edge *e){
    _incident = e;
}

float Vertice::GetX(){
    return _x;
}

float Vertice::GetY(){
    return _y;
}

float Vertice::GetZ(){
    return _z;
}

float Vertice::GetW(){
    return _w;
}

Edge &Vertice::GetIncidentEdge(){
    return *_incident;
}

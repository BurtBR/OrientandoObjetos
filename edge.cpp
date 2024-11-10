#include "edge.h"

Edge::Edge(Vertice *vertice_Origin, Vertice *vertice_Destination,
           Face *face_left, Face *face_right,
           Edge *left_in, Edge *left_out,
           Edge *right_in, Edge *right_out){
    _vOrigin = vertice_Origin;
    _vDestination = vertice_Destination;
    _fLeft = face_left;
    _fRight = face_right;
    _eLeft_In = left_in;
    _eLeft_Out = left_out;
    _eRight_In = right_in;
    _eRight_Out = right_out;
}

void Edge::SetVerticeOrigin(Vertice *v){
    _vOrigin = v;
}

void Edge::SetVerticeDestination(Vertice *v){
    _vDestination = v;
}

void Edge::SetFaceLeft(Face *f){
    _fLeft = f;
}

void Edge::SetFaceRight(Face *f){
    _fRight = f;
}

void Edge::SetEdgeLeftIn(Edge *e){
    _eLeft_In = e;
}

void Edge::SetEdgeLeftOut(Edge *e){
    _eLeft_Out = e;
}

void Edge::SetEdgeRightIn(Edge *e){
    _eRight_In = e;
}

void Edge::SetEdgeRightOut(Edge *e){
    _eRight_Out = e;
}

Vertice &Edge::GetVerticeOrigin(){
    return *_vOrigin;
}

Vertice &Edge::GetVerticeDestination(){
    return *_vDestination;
}

Face &Edge::GetFaceLeft(){
    return *_fLeft;
}

Face &Edge::GetFaceRight(){
    return *_fRight;
}

Edge &Edge::GetEdgeLeftIn(){
    return *_eLeft_In;
}

Edge &Edge::GetEdgeLeftOut(){
    return *_eLeft_Out;
}

Edge &Edge::GetEdgeRightIn(){
    return *_eRight_In;
}

Edge &Edge::GetEdgeRightOut(){
    return *_eRight_Out;
}

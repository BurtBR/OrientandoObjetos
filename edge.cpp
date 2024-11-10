#include "edge.h"

Edge::Edge(Vertice *vertice_Origin, Vertice *vertice_Destination,
           Face *face_left, Face *face_right,
           Edge *left_in, Edge *left_out,
           Edge *right_in, Edge *right_out){

    SetVerticeOrigin(vertice_Origin);
    SetVerticeDestination(vertice_Destination);
    SetFaceLeft(face_left);
    SetFaceRight(face_right);
    SetEdgeLeftIn(left_in);
    SetEdgeLeftOut(left_out);
    SetEdgeRightIn(right_in);
    SetEdgeRightOut(right_out);
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

Vertice &Edge::GetVerticeOrigin() const{
    return *_vOrigin;
}

Vertice &Edge::GetVerticeDestination() const{
    return *_vDestination;
}

Face &Edge::GetFaceLeft() const{
    return *_fLeft;
}

Face &Edge::GetFaceRight() const{
    return *_fRight;
}

Edge &Edge::GetEdgeLeftIn() const{
    return *_eLeft_In;
}

Edge &Edge::GetEdgeLeftOut() const{
    return *_eLeft_Out;
}

Edge &Edge::GetEdgeRightIn() const{
    return *_eRight_In;
}

Edge &Edge::GetEdgeRightOut() const{
    return *_eRight_Out;
}

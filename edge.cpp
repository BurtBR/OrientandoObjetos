#include "edge.h"

Edge::Edge(size_t vertice_Origin, size_t vertice_Destination,
           size_t face_left, size_t face_right,
           size_t left_in, size_t left_out,
           size_t right_in, size_t right_out){

    SetVerticeOrigin(vertice_Origin);
    SetVerticeDestination(vertice_Destination);
    SetFaceLeft(face_left);
    SetFaceRight(face_right);
    SetEdgeLeftIn(left_in);
    SetEdgeLeftOut(left_out);
    SetEdgeRightIn(right_in);
    SetEdgeRightOut(right_out);
}

void Edge::SetVerticeOrigin(size_t v){
    _vOrigin = v;
}

void Edge::SetVerticeDestination(size_t v){
    _vDestination = v;
}

void Edge::SetFaceLeft(size_t f){
    _fLeft = f;
}

void Edge::SetFaceRight(size_t f){
    _fRight = f;
}

void Edge::SetEdgeLeftIn(size_t e){
    _eLeft_In = e;
}

void Edge::SetEdgeLeftOut(size_t e){
    _eLeft_Out = e;
}

void Edge::SetEdgeRightIn(size_t e){
    _eRight_In = e;
}

void Edge::SetEdgeRightOut(size_t e){
    _eRight_Out = e;
}

size_t Edge::GetVerticeOrigin() const{
    return _vOrigin;
}

size_t Edge::GetVerticeDestination() const{
    return _vDestination;
}

size_t Edge::GetFaceLeft() const{
    return _fLeft;
}

size_t Edge::GetFaceRight() const{
    return _fRight;
}

size_t Edge::GetEdgeLeftIn() const{
    return _eLeft_In;
}

size_t Edge::GetEdgeLeftOut() const{
    return _eLeft_Out;
}

size_t Edge::GetEdgeRightIn() const{
    return _eRight_In;
}

size_t Edge::GetEdgeRightOut() const{
    return _eRight_Out;
}

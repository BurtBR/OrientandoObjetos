#include "face.h"

Face::Face(Edge *edge){
    SetEdge(edge);
}

void Face::SetEdge(Edge *e){
    _edge = e;
}

Edge &Face::GetEdge() const{
    return *_edge;
}

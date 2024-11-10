#include "face.h"

Face::Face(Edge *edge){
    _edge = edge;
}

void Face::SetEdge(Edge *e){
    _edge = e;
}

Edge &Face::GetEdge(){
    return *_edge;
}

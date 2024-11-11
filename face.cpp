#include "face.h"

Face::Face(size_t edge){
    SetEdge(edge);
}

void Face::SetEdge(size_t e){
    _edge = e;
}

size_t Face::GetEdge() const{
    return _edge;
}

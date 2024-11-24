#include "face.h"

Face::Face(QString id, Edge *edge){
    SetId(id);
    SetEdge(edge);
}

void Face::SetId(QString id){
    _id = id;
}

void Face::SetEdge(Edge *e){
    _edge = e;
}

QString Face::GetId() const{
    return _id;
}

Edge *Face::GetEdge() const{
    return _edge;
}

#include "edge.h"

Edge::    Edge(Vertice *vertice_Up, Vertice *vertice_Down,
           Face *face_left, Face *face_right,
           Edge *left_up, Edge *left_down, Edge *right_up, Edge *right_down){
    SetVerticeUp(vertice_Up);
    SetVerticeDown(vertice_Down);
    SetFaceLeft(face_left);
    SetFaceRight(face_right);
    SetEdgeLeftUp(left_up);
    SetEdgeLeftDown(left_down);
    SetEdgeRightUp(right_up);
    SetEdgeRightDown(right_down);
}

bool Edge::operator==(Edge &other){
    return IsEquivalent(other.GetVerticeUp(), other.GetVerticeDown());
}

bool Edge::HasNullFace(){
    if(GetFaceLeft() == nullptr)
        return true;
    else if(GetFaceRight() == nullptr)
        return true;

    return false;
}

bool Edge::IsEquivalent(Vertice *v1, Vertice *v2){
    if(GetVerticeUp() == v1 && GetVerticeDown() == v2)
        return true;
    else if(GetVerticeUp() == v2 && GetVerticeDown() == v1)
        return true;

    return false;
}

bool Edge::SetNullSide(Face *f, Edge *e1, Edge *e2){
    if(GetFaceRight() == nullptr){
        SetFaceRight(f);
        if(e1 != nullptr){
            if(e1->HasVertice(_vUp))
                SetEdgeRightUp(e1);
            else if(e1->HasVertice(_vDown))
                SetEdgeRightDown(e1);
        }

        if(e2 != nullptr){
            if(e2->HasVertice(_vUp))
                SetEdgeRightUp(e2);
            else if(e2->HasVertice(_vDown))
                SetEdgeRightDown(e2);
        }
        return true;
    }else if(GetFaceLeft() == nullptr){
        SetFaceLeft(f);
        if(e1 != nullptr){
            if(e1->HasVertice(_vUp))
                SetEdgeLeftUp(e1);
            else if(e1->HasVertice(_vDown))
                SetEdgeLeftDown(e1);
        }

        if(e2 != nullptr){
            if(e2->HasVertice(_vUp))
                SetEdgeLeftUp(e2);
            else if(e2->HasVertice(_vDown))
                SetEdgeLeftDown(e2);
        }
        return true;
    }

    return false;
}

bool Edge::SetEdge(Edge *e, Face *f){
    if(e == nullptr || f == nullptr)
        return false;

    if(e->HasVertice(GetVerticeUp())){
        if(f == GetFaceLeft()){
            SetEdgeLeftUp(e);
            return true;
        }else if(f == GetFaceRight()){
            SetEdgeRightUp(e);
            return true;
        }
    }else if(e->HasVertice(GetVerticeDown())){
        if(f == GetFaceLeft()){
            SetEdgeLeftDown(e);
            return true;
        }else if(f == GetFaceRight()){
            SetEdgeRightDown(e);
            return true;
        }
    }

    return false;
}

bool Edge::HasVertice(Vertice *v){
    if(v == _vUp)
        return true;
    else if(v == _vDown)
        return true;
    return false;
}

void Edge::SetVerticeUp(Vertice *v){
    _vUp = v;
}

void Edge::SetVerticeDown(Vertice *v){
    _vDown = v;
}

void Edge::SetFaceLeft(Face *f){
    _fLeft = f;
}

void Edge::SetFaceRight(Face *f){
    _fRight = f;
}

void Edge::SetEdgeLeftUp(Edge *e){
    _eLeft_Up = e;
}

void Edge::SetEdgeLeftDown(Edge *e){
    _eLeft_Down = e;
}

void Edge::SetEdgeRightUp(Edge *e){
    _eRight_Up = e;
}

void Edge::SetEdgeRightDown(Edge *e){
    _eRight_Down = e;
}

Vertice *Edge::GetSharedVertice(Edge *other){
    if(other == nullptr)
        return nullptr;

    if(other->GetVerticeUp() == GetVerticeUp())
        return GetVerticeUp();
    else if(other->GetVerticeUp() == GetVerticeDown())
        return GetVerticeDown();
    else if(other->GetVerticeDown() == GetVerticeUp())
        return GetVerticeUp();
    else if(other->GetVerticeDown() == GetVerticeDown())
        return GetVerticeDown();

    return nullptr;
}

Vertice *Edge::GetVerticeUp(){
    return _vUp;
}

Vertice *Edge::GetVerticeDown(){
    return _vDown;
}

Face *Edge::GetFaceLeft(){
    return _fLeft;
}

Face *Edge::GetFaceRight(){
    return _fRight;
}

Face *Edge::GetValidFace(){
    return (GetFaceRight()==nullptr ? GetFaceLeft() : GetFaceRight());
}

Edge *Edge::GetSomeNextEdge(Face *f){

    if(f == GetFaceLeft())
        return GetEdgeLeftUp();
    else if(f == GetFaceRight())
        return GetEdgeRightUp();

    return nullptr;
}

Edge *Edge::GetNextEdge(Edge *before, Face *f){

    if(f == GetFaceRight()){
        if(GetEdgeRightUp() == before)
            return GetEdgeRightDown();
        else if(GetEdgeRightDown() == before)
            return GetEdgeRightUp();
    }else if(f == GetFaceLeft()){
        if(before == GetEdgeLeftUp())
            return GetEdgeLeftDown();
        else if(before == GetEdgeLeftDown())
            return GetEdgeLeftUp();
    }

    return nullptr;
}

Edge *Edge::GetNextEdge(Edge *before, Vertice *v){
    if(v == GetVerticeUp()){
        if(before == GetEdgeRightUp())
            return GetEdgeLeftUp();
        else if(before == GetEdgeLeftUp())
            return GetEdgeRightUp();
    }else if(v == GetVerticeDown()){
        if(before == GetEdgeRightDown())
            return GetEdgeLeftDown();
        else if(before == GetEdgeLeftDown())
            return GetEdgeRightDown();
    }

    return nullptr;
}

Edge *Edge::GetEdgeLeftUp(){
    return _eLeft_Up;
}

Edge *Edge::GetEdgeLeftDown(){
    return _eLeft_Down;
}

Edge *Edge::GetEdgeRightUp(){
    return _eRight_Up;
}

Edge *Edge::GetEdgeRightDown(){
    return _eRight_Down;
}

Edge *Edge::GetRightEdge(Vertice *v){
    if(v == GetVerticeUp())
        return GetEdgeRightUp();
    else if(v == GetVerticeDown())
        return GetEdgeRightDown();

    return nullptr;
}

Edge *Edge::GetLeftEdge(Vertice *v){
    if(v == GetVerticeUp())
        return GetEdgeLeftUp();
    else if(v == GetVerticeDown())
        return GetEdgeLeftDown();

    return nullptr;
}

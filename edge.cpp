#include "edge.h"

Edge::    Edge(QString id, Vertice *vertice_Up, Vertice *vertice_Down,
           Face *face_left, Face *face_right,
           Edge *left_up, Edge *left_down, Edge *right_up, Edge *right_down){
    SetId(id);
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

bool Edge::HasFace(Face *f){
    if(GetFaceRight() == f)
        return true;
    if(GetFaceLeft() == f)
        return true;

    return false;
}

bool Edge::IsFaceRight(Face *f){
    return (f == GetFaceRight() ? true : false);
}

bool Edge::IsFaceLeft(Face *f){
    return (f == GetFaceLeft() ? true : false);
}

bool Edge::IsEquivalent(Vertice *v1, Vertice *v2){
    if(GetVerticeUp() == v1 && GetVerticeDown() == v2)
        return true;
    else if(GetVerticeUp() == v2 && GetVerticeDown() == v1)
        return true;

    return false;
}

bool Edge::SetNullSide(Face *f, Edge *e1, Edge *e2){
    if(f == nullptr)
        return false;

    if(GetFaceRight() == nullptr){
        SetFaceRight(f);
    }else if(GetFaceLeft() == nullptr){
        SetFaceLeft(f);
    }else
        return false;

    AddEdge(e1, f);
    AddEdge(e2, f);
    return true;
}

bool Edge::AddEdge(Edge *e, Face *f){
    if(e == nullptr || f == nullptr)
        return false;

    Edge *otherthis, *othere;

    if(HasEdgePlaced(e, f))
        return false;

    otherthis = SetEdge(e);
    othere = e->SetEdge(this);

    if(!otherthis && !othere)
        return true;

    if(othere && otherthis){
        othere->SetEdge(otherthis);
        otherthis->SetEdge(othere);
    }else if(otherthis){
        e->SetEdge(otherthis);
        otherthis->SetEdge(e);
    }else if(othere){
        SetEdge(othere);
        othere->SetEdge(this);
    }

    return true;
}

bool Edge::ReplaceEdgeNotFacing(Edge *from, Edge *to, Face *f){
    if(from == nullptr || to == nullptr)
        return false;

    if(f == GetFaceRight()){
        if(GetEdgeLeftUp() == from)
            SetEdgeLeftUp(to);
        else if(GetEdgeLeftDown() == from)
            SetEdgeLeftDown(to);
        else
            return false;
    }else if(f == GetFaceLeft()){
        if(GetEdgeRightUp() == from)
            SetEdgeRightUp(to);
        else if(GetEdgeRightDown() == from)
            SetEdgeRightDown(to);
        else
            return false;
    }else
        return false;

    return true;
}

Edge *Edge::SetEdge(Edge *e, Face *f){
    if(e == nullptr)
        return nullptr;

    Edge *aux;

    if(e->HasVertice(GetVerticeUp())){

        if(this->IsFaceRight(f)){
            aux = GetEdgeRightUp();
            SetEdgeRightUp(e);
            if(GetEdgeLeftUp() == nullptr)
                SetEdgeLeftUp(e);
        }else if(this->IsFaceLeft(f)){
            aux = GetEdgeLeftUp();
            SetEdgeLeftUp(e);
            if(GetEdgeRightUp() == nullptr)
                SetEdgeRightUp(e);
        }else
            return nullptr;

        return aux;

    }else if(e->HasVertice(GetVerticeDown())){

        if(this->IsFaceRight(f)){
            aux = GetEdgeRightDown();
            SetEdgeRightDown(e);
            if(GetEdgeLeftDown() == nullptr)
                SetEdgeLeftDown(e);
        }else if(this->IsFaceLeft(f)){
            aux = GetEdgeLeftDown();
            SetEdgeLeftDown(e);
            if(GetEdgeRightDown() == nullptr)
                SetEdgeRightDown(e);
        }else
            return nullptr;

        return aux;

    }

    return nullptr;
}

Edge *Edge::SetEdge(Edge *e){
    if(e == nullptr)
        return nullptr;

    // If both pointers are changed, they were null and one edge points to the other "covalent bond".

    Edge *aux = nullptr;

    if(e->HasFace(GetFaceRight())){
        if(e->HasVertice(GetVerticeUp())){
            aux = GetEdgeRightUp();
            SetEdgeRightUp(e);
            if(GetEdgeLeftUp() == nullptr)
                SetEdgeLeftUp(e);
        }else if(e->HasVertice(GetVerticeDown())){
            aux = GetEdgeRightDown();
            SetEdgeRightDown(e);
            if(GetEdgeLeftDown() == nullptr)
                SetEdgeLeftDown(e);
        }
    }

    if(e->HasFace(GetFaceLeft())){
        if(e->HasVertice(GetVerticeUp())){
            aux = GetEdgeLeftUp();
            SetEdgeLeftUp(e);
            if(GetEdgeRightUp() == nullptr)
                SetEdgeRightUp(e);
        }else if(e->HasVertice(GetVerticeDown())){
            aux = GetEdgeLeftDown();
            SetEdgeLeftDown(e);
            if(GetEdgeRightDown() == nullptr)
                SetEdgeRightDown(e);
        }
    }

    return aux;
}

bool Edge::SetEdgeIfNull(Edge *e, Face *f){
    if(e == nullptr)
        return false;

    if(e->HasVertice(GetVerticeUp())){

        if(GetEdgeRightUp() != nullptr)
            return false;

        if(this->IsFaceRight(f)){
            SetEdgeRightUp(e);
            SetEdgeLeftUp(e);
        }else if(this->IsFaceLeft(f)){
            SetEdgeLeftUp(e);
            SetEdgeRightUp(e);
        }else
            return false;

        return true;

    }else if(e->HasVertice(GetVerticeDown())){

        if(GetEdgeRightDown() != nullptr)
            return false;

        if(this->IsFaceRight(f)){
            SetEdgeRightDown(e);
            SetEdgeLeftDown(e);
        }else if(this->IsFaceLeft(f)){
            SetEdgeLeftDown(e);
            SetEdgeRightDown(e);
        }else
            return false;

        return true;

    }

    return false;
}

bool Edge::SetOpositeEdge(Edge *e, Face *f){
    if(e == nullptr)
        return false;

    if(e->HasVertice(GetVerticeUp())){

        if(this->IsFaceRight(f)){
            SetEdgeLeftUp(e);
            if(GetEdgeRightUp() == nullptr)
                SetEdgeRightUp(e);
        }else if(this->IsFaceLeft(f)){
            SetEdgeRightUp(e);
            if(GetEdgeLeftUp() == nullptr)
                SetEdgeLeftUp(e);
        }else
            return false;

        return true;

    }else if(e->HasVertice(GetVerticeDown())){

        if(this->IsFaceRight(f)){
            SetEdgeLeftDown(e);
            if(GetEdgeRightDown() == nullptr)
                SetEdgeRightDown(e);
        }else if(this->IsFaceLeft(f)){
            SetEdgeRightDown(e);
            if(GetEdgeLeftDown() == nullptr)
                SetEdgeLeftDown(e);
        }else
            return false;

        return true;

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

bool Edge::HasEdgePlaced(Edge *e, Face *f){
    if(e == nullptr || f == nullptr)
        return false;

    if(e->HasVertice(GetVerticeUp())){
        if(f == GetFaceRight()){
            return (e == GetEdgeRightUp());
        }else if(f == GetFaceLeft()){
            return (e == GetEdgeLeftUp());
        }else
            return false;
    }else if(e->HasVertice(GetVerticeDown())){
        if(f == GetFaceRight()){
            return (e == GetEdgeRightDown());
        }else if(f == GetFaceLeft()){
            return (e == GetEdgeLeftDown());
        }else
            return false;
    }

    return false;
}

void Edge::SetId(QString id){
    _id = id;
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

QString Edge::GetId() const{
    return _id;
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

Vertice *Edge::GetVerticeUp() const{
    return _vUp;
}

Vertice *Edge::GetVerticeDown() const{
    return _vDown;
}

Face *Edge::GetFaceLeft() const{
    return _fLeft;
}

Face *Edge::GetFaceRight() const{
    return _fRight;
}

Edge *Edge::GetEdge(Vertice *v, Face *f){

    // Don't need to check if null

    if(v == GetVerticeUp()){
        if(f == GetFaceRight())
            return GetEdgeRightUp();
        else if(f == GetFaceLeft())
            return GetEdgeLeftUp();
    }else if(v == GetVerticeDown()){
        if(f == GetFaceRight())
            return GetEdgeRightDown();
        else if(f == GetFaceLeft())
            return GetEdgeLeftDown();
    }

    return nullptr;
}

Edge *Edge::GetOpositeEdge(Edge* e){

    if(e == GetEdgeRightUp())
        return GetEdgeLeftUp();
    else if(e == GetEdgeRightDown())
        return GetEdgeLeftDown();
    else if(e == GetEdgeLeftUp())
        return GetEdgeRightUp();
    else if(e == GetEdgeLeftDown())
        return GetEdgeRightDown();

    return nullptr;
}

Edge *Edge::GetSomeNextEdge(const Face *f){

    if(f == GetFaceLeft())
        return GetEdgeLeftUp();
    else if(f == GetFaceRight())
        return GetEdgeRightUp();

    return nullptr;
}

Edge *Edge::GetNextEdge(Edge *before, const Face *f){

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

Edge *Edge::GetNextEdge(Edge *before, const Vertice *v){
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

Edge *Edge::GetEdgeLeftUp() const{
    return _eLeft_Up;
}

Edge *Edge::GetEdgeLeftDown() const{
    return _eLeft_Down;
}

Edge *Edge::GetEdgeRightUp() const{
    return _eRight_Up;
}

Edge *Edge::GetEdgeRightDown() const{
    return _eRight_Down;
}

Edge *Edge::GetRightEdge(const Vertice *v){
    if(v == GetVerticeUp())
        return GetEdgeRightUp();
    else if(v == GetVerticeDown())
        return GetEdgeRightDown();

    return nullptr;
}

Edge *Edge::GetLeftEdge(const Vertice *v){
    if(v == GetVerticeUp())
        return GetEdgeLeftUp();
    else if(v == GetVerticeDown())
        return GetEdgeLeftDown();

    return nullptr;
}

#include "workergeometry.h"

#include <QFile>
#include <QFileInfo>

WorkerGeometry::WorkerGeometry(QObject *parent) : QObject{parent}{

}

WorkerGeometry::~WorkerGeometry(){
    _opMatrix.setToIdentity();
}

WorkerGeometry::VerticeData WorkerGeometry::ParseVertice(const Vertice *v){

    WorkerGeometry::VerticeData data;

    if(v == nullptr)
        return data;

    data = {
        ._addr = v,
        ._id = v->GetId(),
        ._x = QString::number(v->GetX(), 'f', 2),
        ._y = QString::number(v->GetY(), 'f', 2),
        ._z = QString::number(v->GetZ(), 'f', 2),
        ._incident = (v->GetIncidentEdge() == nullptr ? "-" : v->GetIncidentEdge()->GetId())
    };

    return data;
}

WorkerGeometry::EdgeData WorkerGeometry::ParseEdge(const Edge *e){

    WorkerGeometry::EdgeData data;

    if(e == nullptr)
        return data;

    data = {
        ._addr = e,
        ._id = e->GetId(),
        ._verticeUp = (e->GetVerticeUp() == nullptr ? "-" : e->GetVerticeUp()->GetId()),
        ._verticeDown = (e->GetVerticeDown() == nullptr ? "-" : e->GetVerticeDown()->GetId()),
        ._faceLeft = (e->GetFaceLeft() == nullptr ? "-" : e->GetFaceLeft()->GetId()),
        ._faceRight = (e->GetFaceRight() == nullptr ? "-" : e->GetFaceRight()->GetId()),
        ._edgeLeftUp = (e->GetEdgeLeftUp() == nullptr ? "-" : e->GetEdgeLeftUp()->GetId()),
        ._edgeLeftDown = (e->GetEdgeLeftDown() == nullptr ? "-" : e->GetEdgeLeftDown()->GetId()),
        ._edgeRightUp = (e->GetEdgeRightUp() == nullptr ? "-" : e->GetEdgeRightUp()->GetId()),
        ._edgeRightDown = (e->GetEdgeRightDown() == nullptr ? "-" : e->GetEdgeRightDown()->GetId())
    };

    return data;
}

WorkerGeometry::FaceData WorkerGeometry::ParseFace(const Face *f){

    WorkerGeometry::FaceData data;

    if(f == nullptr)
        return data;

    data = {
        ._addr = f,
        ._id = f->GetId(),
        ._edge = (f->GetEdge() == nullptr ? "-" : f->GetEdge()->GetId())
    };

    return data;
}

bool WorkerGeometry::InsertVertice(QStringList list, QVector<Vertice*> &vectorvertices){
    float x, y, z;

    if(list.size() != 4){
        emit Message("Valores incompatíveis de " + list[0], ErrorMessage::ErrorCode::CorruptedFile);
        return false;
    }

    if(list[0] != "v")
        return false;

    if(!StrToFloat(list[1], x) || !StrToFloat(list[2], y) || !StrToFloat(list[3], z)){
        emit Message("Formatação incompatível de ponto flutuante: " +
                     list.join(' '),
                     ErrorMessage::ErrorCode::CorruptedFile);
        return false;
    }else{
        vectorvertices.append(&(*_vertices.insert(_vertices.end(), Vertice(QString::number(_vertices.size()+1), x,y,z))));
    }

    return true;
}

bool WorkerGeometry::InsertFace(QStringList list, QVector<Vertice *> &vectorvertices){
    Face *currface;
    Edge *curredge, *firstedge, *otheredge;
    size_t idx, origin, destination, firstvertice;

    if(list.size() < 4){
        emit Message("São necessários ao menos 3 pontos em uma face.", ErrorMessage::ErrorCode::CorruptedFile);
        return false;
    }

    if(list[0] != "f")
        return false;

    currface = &(*_faces.insert(_faces.end(), Face(QString::number(_faces.size()+1))));

    idx = list[1].split('/')[0].toLongLong()-1;
    if(idx >= vectorvertices.size()){
        emit Message("Índices de vértices inválidos: " + list.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
        return false;
    }

    firstvertice = idx;
    origin = firstvertice;

    idx = list[2].split('/')[0].toLongLong()-1;
    if(idx >= vectorvertices.size()){
        emit Message("Índices de vértices inválidos: " + list.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
        return false;
    }

    destination = idx;

    curredge = FindEdge(vectorvertices[origin], vectorvertices[destination]);
    if(curredge == nullptr){
        curredge = &(*_edges.insert(_edges.end(),Edge(QString::number(_edges.size()+1),
                                                       vectorvertices[origin],
                                                       vectorvertices[destination],
                                                       nullptr,
                                                       currface)));

        if(vectorvertices[destination]->GetIncidentEdge() == nullptr)
            vectorvertices[destination]->SetIncidentEdge(otheredge);
        if(vectorvertices[origin]->GetIncidentEdge() == nullptr)
            vectorvertices[origin]->SetIncidentEdge(curredge);
    }else if(!curredge->SetNullSide(currface, nullptr, nullptr)){
        emit Message("Vértice compartilha mais de 2 faces: " + list.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
        return false;
    }

    currface->SetEdge(curredge);
    firstedge = curredge;

    for(int i=3; i<list.size() ;i++){
        origin = destination;
        idx = list[i].split('/')[0].toLongLong()-1;
        if(idx >= vectorvertices.size()){
            emit Message("Índices de vértices inválidos: " + list.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
            return false;
        }

        destination = idx;

        otheredge = FindEdge(vectorvertices[origin], vectorvertices[destination]);
        if(otheredge == nullptr){
            otheredge = &(*_edges.insert(_edges.end(),
                                         Edge(QString::number(_edges.size()+1),
                                              vectorvertices[origin],
                                              vectorvertices[destination],
                                              nullptr,
                                              currface)));

            if(vectorvertices[destination]->GetIncidentEdge() == nullptr)
                vectorvertices[destination]->SetIncidentEdge(otheredge);
            if(vectorvertices[origin]->GetIncidentEdge() == nullptr)
                vectorvertices[origin]->SetIncidentEdge(otheredge);
        }else if(!otheredge->SetNullSide(currface, nullptr, nullptr)){
            emit Message("Vértice compartilha mais de 2 faces: " + list.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
            return false;
        }
        curredge->AddEdge(otheredge, currface);
        curredge = otheredge;
    }

    if(destination != firstvertice){
        origin = destination;
        destination = firstvertice;

        otheredge = FindEdge(vectorvertices[origin], vectorvertices[destination]);

        if(otheredge == nullptr){
            otheredge = &(*_edges.insert(_edges.end(),
                                         Edge(QString::number(_edges.size()+1),
                                              vectorvertices[origin],
                                              vectorvertices[destination],
                                              nullptr,
                                              currface)));

            if(vectorvertices[destination]->GetIncidentEdge() == nullptr)
                vectorvertices[destination]->SetIncidentEdge(otheredge);
            if(vectorvertices[origin]->GetIncidentEdge() == nullptr)
                vectorvertices[origin]->SetIncidentEdge(otheredge);
        }else if(!otheredge->SetNullSide(currface, curredge, nullptr)){
                emit Message("Vértice compartilha mais de 2 faces: " + list.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                return false;
        }
        curredge->AddEdge(otheredge, currface);
        curredge = otheredge;
        firstedge->AddEdge(curredge, currface);
    }

    return true;
}

void WorkerGeometry::ClearGeometry(){
    _edges.clear();
    _vertices.clear();
    _faces.clear();
}

void WorkerGeometry::FailedToOpenObj(){
    ClearGeometry();
    emit FileHandlingFinished();
}

bool WorkerGeometry::StrToFloat(const QString &str, float &number){
    bool ok;
    number = str.toFloat(&ok);
    return ok;
}

Edge *WorkerGeometry::FindEdge(Vertice *v1, Vertice *v2){

    if(v1 == nullptr || v2 == nullptr)
        return nullptr;

    Edge *ebegin, *prevedge, *curredge, *nextedge;
    Vertice *center;

    if(v1->GetIncidentEdge() != nullptr){
        center = v1;
    }else if(v2->GetIncidentEdge() != nullptr){
        center = v2;
    }else{
        return nullptr;
    }

    ebegin = center->GetIncidentEdge();

    if(ebegin->IsEquivalent(v1, v2))
        return ebegin;

    curredge = ebegin;
    nextedge = curredge->GetNextEdge(curredge->GetRightEdge(center), center);

    while(nextedge != nullptr && nextedge != ebegin){
        if(nextedge->IsEquivalent(v1, v2)){
            return nextedge;
        }
        prevedge = curredge;
        curredge = nextedge;
        nextedge = nextedge->GetNextEdge(prevedge, center);
    }

    return nullptr;
}

void WorkerGeometry::SendVerticeList(){
    std::list<Vertice>::iterator itr = _vertices.begin();
    QStringList strlist;
    QVector<const Vertice*> vertices;

    while(itr != _vertices.end()){
        strlist.append("Vértice " + itr->GetId());
        vertices.append(&(*itr));
        itr++;
    }

    emit SetVerticeList(vertices, strlist);
}

void WorkerGeometry::SendEdgeList(){
    std::list<Edge>::iterator itr = _edges.begin();
    QStringList strlist;
    QVector<const Edge*> edges;

    while(itr != _edges.end()){
        strlist.append("Aresta " + itr->GetId());
        edges.append(&(*itr));
        itr++;
    }

    emit SetEdgeList(edges, strlist);
}

void WorkerGeometry::SendFaceList(){
    std::list<Face>::iterator itr = _faces.begin();
    QStringList strlist;
    QVector<const Face*> faces;

    while(itr != _faces.end()){
        strlist.append("Face " + itr->GetId());
        faces.append(&(*itr));
        itr++;
    }

    emit SetFaceList(faces, strlist);
}

void WorkerGeometry::SendOperations(){
    if(!_ops.size()){
        _opMatrix.setToIdentity();
        emit SetOperationMatrix(_opMatrix);
        emit SetOperationList(QStringList());
        return;
    }

    emit SetOperationMatrix(_opMatrix);

    QStringList list;

    for(int i=0; i<_ops.size() ;i++){
        list.append(_ops[i].GetOperationStr());
    }

    emit SetOperationList(list);
}

void WorkerGeometry::GetSelectedVertice(const Vertice *v){

    if(v == nullptr)
        return;

    emit SetSelectedVerticeData(ParseVertice(v));
}

void WorkerGeometry::GetSelectedEdge(const Edge *e){

    if(e == nullptr)
        return;

    emit SetSelectedEdgeData(ParseEdge(e));
}

void WorkerGeometry::GetSelectedFace(const Face *f){

    if(f == nullptr)
        return;

    emit SetSelectedFaceData(ParseFace(f));
}

void WorkerGeometry::OpenObj(QString filename){

    QFile fp(filename);
    QFileInfo fileinfo(fp);
    QStringList strlist, unknownparameters;
    QVector<Vertice *> vectorvertices;

    if(!fileinfo.exists()){
        emit Message("O arquivo não existe.", ErrorMessage::ErrorCode::FailedToOpenFile);
        return FailedToOpenObj();
    }

    if(!fp.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit Message("Acesso negado.", ErrorMessage::ErrorCode::FailedToOpenFile);
        return FailedToOpenObj();
    }

    emit Message("Carregando arquivo: " + fileinfo.fileName());
    ClearGeometry();

    while(!fp.atEnd()){
        strlist.clear();
        strlist.append(fp.readLine().trimmed());
        if(strlist[0].size()){
            strlist = strlist[0].split(' ', Qt::SkipEmptyParts);

            switch(strlist[0][0].toLatin1()){

            case 'v':
                if(strlist[0].size() == 1){ // Vertice
                    if(!InsertVertice(strlist, vectorvertices)){
                        fp.close();
                        return FailedToOpenObj();
                    }
                }else{
                    switch(strlist[0][1].toLatin1()){
                    //case 'n':
                    //case 't':
                    default:
                        if(!unknownparameters.contains(strlist[0]))
                            unknownparameters.append(strlist[0]);
                        break;
                    }
                }
                break;

            case 'f': // Face
                if(!InsertFace(strlist, vectorvertices)){
                    fp.close();
                    return FailedToOpenObj();
                }
                break;

            case '#':
                break;

            default:
                if(!unknownparameters.contains(strlist[0]))
                    unknownparameters.append(strlist[0]);
                break;
            }
        }
    }

    fp.close();

    if(unknownparameters.size())
        emit Message("Parâmetros \"" + unknownparameters.join("\" \"") + "\" não implementados.", ErrorMessage::ErrorCode::CorruptedFile);

    unknownparameters.clear();

    _ops.clear();
    _opMatrix.setToIdentity();

    SendVerticeList();
    SendEdgeList();
    SendFaceList();
    SendOperations();

    emit FileHandlingFinished();
}

void WorkerGeometry::PrintAllData(){

    if(!_vertices.size()){
        emit Message("Não há estruturas para imprimir", ErrorMessage::ErrorCode::Misc);
        return;
    }

    std::list<Vertice>::iterator v = _vertices.begin();
    std::list<Edge>::iterator e = _edges.begin();
    std::list<Face>::iterator f = _faces.begin();

    while(v != _vertices.end()){
        emit Message("Vértice " + QString::number((uint64_t)&(*v), 16) + " | (  X;  Y;  Z)=(" +
                     QString::number(v->GetX()).rightJustified(3,' ') + ";" +
                     QString::number(v->GetY()).rightJustified(3,' ') + ";" +
                     QString::number(v->GetZ()).rightJustified(3,' ') + ") |" +
                     " Aresta " + v->GetIncidentEdge()->GetId() + " |", ErrorMessage::ErrorCode::Misc);
        v++;
    }

    while(e != _edges.end()){
        emit Message("Aresta " + e->GetId() + "\n| "+
                     "V. Up " + e->GetVerticeUp()->GetId() + " | "+
                     "V. Down " + e->GetVerticeDown()->GetId() + " |\n| "+
                     "Face Dir. " + e->GetFaceRight()->GetId() + " | "+
                     "Dir. Up " + e->GetEdgeRightUp()->GetId() + " | "+
                     "Dir. Down " + e->GetEdgeRightDown()->GetId() + " |\n| "+
                     "Face Esq. " + e->GetFaceLeft()->GetId() + " | "+
                     "Esq. Up " + e->GetEdgeLeftUp()->GetId() + " | "+
                     "Esq. Down " + e->GetEdgeLeftDown()->GetId() + " | ", ErrorMessage::ErrorCode::Misc);
        e++;
    }

    while(f != _faces.end()){
        emit Message("Face " + QString::number((uint64_t)&(*f), 16) + " | " +
                         "Aresta " + f->GetEdge()->GetId() + " |", ErrorMessage::ErrorCode::Misc);
        f++;
    }
}

void WorkerGeometry::PrintVerticesFromFace(const Face *f){
    QString text;

    if(f == nullptr){
        emit Message("Face não encontrada.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    Edge *ebegin = f->GetEdge(), *itr, *before, *next;

    if(ebegin == nullptr){
        emit Message("Dados inconsistentes, a Face " + f->GetId() + " possui aresta inválida.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    itr = ebegin;
    next = itr->GetSomeNextEdge(f);

    if(next == nullptr){
        emit Message("Dados inconsistentes, a Face " + f->GetId() + " possui aresta inválida.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    text = "A Face " + f->GetId() + " possui os vértices: " + itr->GetSharedVertice(next)->GetId();

    do{
        before = itr;
        itr = next;

        next = itr->GetNextEdge(before, f);
        if(next == nullptr){
            emit Message("Dados inconsistentes, a " + f->GetId() + " possui aresta inválida.", ErrorMessage::ErrorCode::Misc);
            return;
        }

        text.append(", " + itr->GetSharedVertice(next)->GetId() );
    }while(next != ebegin);

    emit Message(text, ErrorMessage::ErrorCode::Misc);
}

void WorkerGeometry::PrintFacesFromEdge(const Edge *e){

    if(e == nullptr){
        emit Message("Aresta não encontrada.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    QString text;

    if((e->GetFaceLeft() == nullptr) && (e->GetFaceRight() == nullptr)){
        emit Message("A Aresta" + e->GetId() + " não possui faces adjacentes.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    text = "A Aresta " + e->GetId() + " possui a Face ";

    if(e->GetFaceRight() == nullptr)
        text.append(e->GetFaceLeft()->GetId() + " ao seu lado esquerdo.");
    else if(e->GetFaceLeft() == nullptr)
        text.append(e->GetFaceRight()->GetId() + " ao seu lado direito.");
    else
        text.append(e->GetFaceLeft()->GetId() + " ao seu lado esquerdo e a Face " +
                    e->GetFaceRight()->GetId() + " ao seu lado direito.");

    emit Message(text, ErrorMessage::ErrorCode::Misc);

}

void WorkerGeometry::PrintEdgesFromVertice(const Vertice *v){

    Edge *ebegin, *prevedge, *curredge, *nextedge;
    QString text;

    if(v == nullptr){
        emit Message("Vertice não encontrado.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    ebegin = v->GetIncidentEdge();
    if(ebegin == nullptr){
        emit Message("Referência inválida para aresta no Vértice " + v->GetId() + ".", ErrorMessage::ErrorCode::Misc);
        return;
    }

    text = "O Vértice " + v->GetId() + " compartilha as arestas " + ebegin->GetId();

    curredge = ebegin;
    nextedge = curredge->GetNextEdge(curredge->GetRightEdge(v), v);

    while(nextedge != nullptr && nextedge != ebegin){
        text.append(", " + nextedge->GetId());
        prevedge = curredge;
        curredge = nextedge;
        nextedge = nextedge->GetNextEdge(prevedge, v);
    }

    if(nextedge == nullptr){
        emit Message("O mapeamento de Arestas parece estar incorreto.", ErrorMessage::ErrorCode::Misc);
    }

    emit Message(text, ErrorMessage::ErrorCode::Misc);
}

void WorkerGeometry::AddOperation(float x, float y, float z, Operation::OpType op){

    Operation newop(x, y, z, op);

    _ops.append(newop);
    _opMatrix = (newop.GetMatrix() * _opMatrix);
    SendOperations();
}

void WorkerGeometry::CalculateOpMatrix(){
    if(!_ops.size()){
        _opMatrix.setToIdentity();
        return;
    }

    _opMatrix = _ops[0].GetMatrix();

    for(int i=1; i<_ops.size() ;i++){
        _opMatrix = (_ops[i].GetMatrix() * _opMatrix);
    }
}

void WorkerGeometry::RemoveOperation(size_t idx){
    if(idx >= _ops.size())
        return;

    _ops.remove(idx);
    CalculateOpMatrix();
    SendOperations();
}

void WorkerGeometry::MoveOperationUp(size_t idx){
    if(idx <= 0 || idx >= _ops.size())
        return;

    Operation aux = _ops[idx];

    _ops[idx] = _ops[idx-1];
    _ops[idx-1] = aux;

    CalculateOpMatrix();
    SendOperations();
}

void WorkerGeometry::MoveOperationDown(size_t idx){
    if(idx >= (_ops.size()-1))
        return;

    Operation aux = _ops[idx];

    _ops[idx] = _ops[idx+1];
    _ops[idx+1] = aux;

    CalculateOpMatrix();
    SendOperations();
}

void WorkerGeometry::SetOperationXYZ(size_t idx, float x, float y, float z){
    if(idx >= _ops.size())
        return;

    _ops[idx].SetOperation(x, y, z, _ops[idx].GetOpType());

    CalculateOpMatrix();
    SendOperations();
}

void WorkerGeometry::GetSelectedOperation(size_t idx){
    if(idx >= _ops.size())
        return;

    emit SetSelectedOperation(_ops[idx].GetX(), _ops[idx].GetY(), _ops[idx].GetZ());
}

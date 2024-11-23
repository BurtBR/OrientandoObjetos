#include "workergeometry.h"

#include <QFile>
#include <QFileInfo>

WorkerGeometry::WorkerGeometry(QObject *parent) : QObject{parent}{

}

WorkerGeometry::~WorkerGeometry(){
    _opMatrix.setToIdentity();
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

uint64_t WorkerGeometry::StrToAddr(QString str){
    if(!str.size())
        return 0;

    QStringList list = str.split(' ');

    if(list.size() != 2)
        return 0;

    return list[1].toULongLong(nullptr,16);
}

Edge *WorkerGeometry::FindEquivalentEdge(Edge *e){

    std::list<Edge>::iterator itr = _edges.begin();

    while(itr != _edges.end()){
        if(*itr == *e && &(*itr) != e)
            return &(*itr);
        itr++;
    }
    return nullptr;
}

Edge *WorkerGeometry::FindEdge(Vertice *v1, Vertice *v2){

    std::list<Edge>::iterator itr = _edges.begin();

    while(itr != _edges.end()){
        if(itr->IsEquivalent(v1, v2))
            return &(*itr);
        itr++;
    }
    return nullptr;
}

void WorkerGeometry::CheckVerticesIncidentEdges(){
/*
    QHash<size_t, Vertice>::Iterator v = _vertices.begin();
    QHash<size_t, Edge>::Iterator e;
    QVector<size_t> wrongvertices;

    while(v != _vertices.end()){
        if(v->GetIncidentEdge() == -1)
            wrongvertices.append(v.key());
        v++;
    }

    if(!wrongvertices.size())
        return;

    e = _edges.begin();

    while(wrongvertices.size()){
        if(wrongvertices.contains(e->GetVerticeDestination())){
            _vertices.find(e->GetVerticeDestination())->SetIncidentEdge(e.key());
            wrongvertices.removeOne(e->GetVerticeDestination());
        }
        e++;
    }
*/
}

void WorkerGeometry::ReplaceVerticeReference(size_t from, size_t to){
/*
    QHash<size_t, Edge>::Iterator itr = _edges.begin();

    while(itr != _edges.end()){

        if(itr->GetVerticeOrigin() == from){
            itr->SetVerticeOrigin(to);
        }else if(itr->GetVerticeDestination() == from){
            itr->SetVerticeDestination(to);
        }

        itr++;
    }
*/
}

void WorkerGeometry::ReplaceEdgeReferences(size_t from, size_t to){
/*
    QHash<size_t, Vertice>::Iterator v = _vertices.begin();
    QHash<size_t, Edge>::Iterator e = _edges.begin();
    QHash<size_t, Face>::Iterator f = _faces.begin();

    while(v != _vertices.end()){
        if(v->GetIncidentEdge() == from){
            v->SetIncidentEdge(to);
        }
        v++;
    }

    while(e != _edges.end()){
        if(e->GetEdgeLeftIn() == from)
            e->SetEdgeLeftIn(to);
        if(e->GetEdgeLeftOut() == from)
            e->SetEdgeLeftOut(to);
        if(e->GetEdgeRightIn() == from)
            e->SetEdgeRightIn(to);
        if(e->GetEdgeRightOut() == from)
            e->SetEdgeRightOut(to);
        e++;
    }

    while(f != _faces.end()){
        if(f->GetEdge() == from)
            f->SetEdge(to);
        f++;
    }
*/
}

void WorkerGeometry::SendVerticeList(){
    std::list<Vertice>::iterator itr = _vertices.begin();
    QStringList strlist;

    while(itr != _vertices.end()){
        strlist.append("Vértice " + QString::number((uint64_t)&(*itr), 16).toUpper());
        itr++;
    }

    emit SetVerticeList(strlist);
}

void WorkerGeometry::SendEdgeList(){
    std::list<Edge>::iterator itr = _edges.begin();
    QStringList strlist;

    while(itr != _edges.end()){
        strlist.append("Aresta " + QString::number((uint64_t)&(*itr), 16).toUpper());
        itr++;
    }

    emit SetEdgeList(strlist);
}

void WorkerGeometry::SendFaceList(){
    std::list<Face>::iterator itr = _faces.begin();
    QStringList strlist;

    while(itr != _faces.end()){
        strlist.append("Face " + QString::number((uint64_t)&(*itr), 16).toUpper());
        itr++;
    }

    emit SetFaceList(strlist);
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

void WorkerGeometry::GetSelectedVertice(QString str){
    Vertice *v = (Vertice*)StrToAddr(str);
    if(v != nullptr)
        emit SetSelectedVerticeData(*v);
}

void WorkerGeometry::GetSelectedEdge(QString str){
    Edge *e = (Edge*)StrToAddr(str);
    if(e != nullptr)
        emit SetSelectedEdgeData(*e);
}

void WorkerGeometry::GetSelectedFace(QString str){
    Face *f = (Face*)StrToAddr(str);
    if(f != nullptr)
        emit SetSelectedFaceData(*f);
}

void WorkerGeometry::OpenObj(QString filename){

    QFile fp(filename);
    QFileInfo fileinfo(fp);
    QStringList strlist, unknownparameters;
    float x, y, z;
    QVector<Vertice *> vectorvertices;
    Face *currface = nullptr;
    Edge *curredge = nullptr, *firstedge = nullptr, *otheredge;
    size_t idx, origin, destination, firstvertice;

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
                if(strlist.size() != 4){
                    emit Message("Valores incompatíveis de " + strlist[0], ErrorMessage::ErrorCode::CorruptedFile);
                    return FailedToOpenObj();
                }else if(strlist[0].size() == 1){ // Vertice

                    if(!StrToFloat(strlist[1], x) || !StrToFloat(strlist[2], y) || !StrToFloat(strlist[3], z)){
                        emit Message("Formatação incompatível de ponto flutuante: " +
                                     strlist[1] + " " + strlist[2] + " " + strlist[3],
                                     ErrorMessage::ErrorCode::CorruptedFile);
                        return FailedToOpenObj();
                    }else{
                        vectorvertices.append(&(*_vertices.insert(_vertices.end(), Vertice(x,y,z))));
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
                if(strlist.size() < 4){
                    emit Message("São necessários ao menos 3 pontos em uma face.", ErrorMessage::ErrorCode::CorruptedFile);
                    return FailedToOpenObj();
                }

                currface = &(*_faces.insert(_faces.end(), Face()));

                idx = strlist[1].split('/')[0].toLongLong()-1;
                if(idx >= _vertices.size()){
                    emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    return FailedToOpenObj();
                }

                firstvertice = idx;
                origin = firstvertice;

                idx = strlist[2].split('/')[0].toLongLong()-1;

                if(idx >= vectorvertices.size()){
                    emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    return FailedToOpenObj();
                }

                destination = idx;

                otheredge = FindEdge(vectorvertices[origin], vectorvertices[destination]);
                if(otheredge == nullptr){
                    curredge = &(*_edges.insert(_edges.end(),Edge(vectorvertices[origin], vectorvertices[destination], nullptr, currface)));
                    if(vectorvertices[destination]->GetIncidentEdge() == nullptr)
                        vectorvertices[destination]->SetIncidentEdge(curredge);
                    else if(vectorvertices[origin]->GetIncidentEdge() == nullptr)
                        vectorvertices[origin]->SetIncidentEdge(curredge);
                }else{
                    curredge = otheredge;
                    if(!curredge->SetNullSide(currface, nullptr, nullptr)){
                        emit Message("Índices de vértice compartilha mais de 2 faces: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                        return FailedToOpenObj();
                    }
                }

                currface->SetEdge(curredge);

                firstedge = curredge;

                for(int i=3; i<strlist.size() ;i++){
                    origin = destination;
                    idx = strlist[i].split('/')[0].toLongLong()-1;
                    if(idx >= vectorvertices.size()){
                        emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                        return FailedToOpenObj();
                    }else{
                        destination = idx;
                        otheredge = FindEdge(vectorvertices[origin], vectorvertices[destination]);
                        if(otheredge == nullptr){
                            otheredge = &(*_edges.insert(_edges.end(),
                                                         Edge(vectorvertices[origin],
                                                              vectorvertices[destination],
                                                              nullptr,
                                                              currface)));
                            otheredge->SetEdge(curredge, currface);

                            if(vectorvertices[destination]->GetIncidentEdge() == nullptr)
                                vectorvertices[destination]->SetIncidentEdge(curredge);
                            else if(vectorvertices[origin]->GetIncidentEdge() == nullptr)
                                vectorvertices[origin]->SetIncidentEdge(curredge);
                        }else{
                            if(!otheredge->SetNullSide(currface, curredge, nullptr)){
                                emit Message("Vértice compartilha mais de 2 faces: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                                return FailedToOpenObj();
                            }
                        }
                        curredge->SetEdge(otheredge, currface);
                        curredge = otheredge;
                    }
                }

                if(destination != firstvertice){
                    origin = destination;
                    destination = firstvertice;

                    otheredge = &(*_edges.insert(_edges.end(),Edge(vectorvertices[origin],
                                                                   vectorvertices[destination],
                                                                   nullptr,
                                                                   currface)));
                    otheredge->SetEdge(curredge, currface);
                    otheredge->SetEdge(firstedge, currface);
                    curredge->SetEdge(otheredge, currface);
                    curredge = otheredge;
                }

                firstedge->SetEdge(curredge, currface);

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

    PrintAllData();

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
                     " Aresta " + QString::number((uint64_t)v->GetIncidentEdge(), 16) + " |", ErrorMessage::ErrorCode::Misc);
        v++;
    }

    while(e != _edges.end()){
        emit Message("Aresta " + QString::number((uint64_t)&(*e), 16) + "\n| "+
                     "V. Up " + QString::number((uint64_t)e->GetVerticeUp()) + " | "+
                     "V. Down " + QString::number((uint64_t)e->GetVerticeDown(), 16) + " |\n| "+
                     "Face Dir. " + QString::number((uint64_t)e->GetFaceRight(), 16) + " | "+
                     "Dir. Up " + QString::number((uint64_t)e->GetEdgeRightUp(), 16) + " | "+
                     "Dir. Down " + QString::number((uint64_t)e->GetEdgeRightDown(), 16) + " |\n| "+
                     "Face Esq. " + QString::number((uint64_t)e->GetFaceLeft(), 16) + " | "+
                     "Esq. Up " + QString::number((uint64_t)e->GetEdgeLeftUp(), 16)+ " | "+
                     "Esq. Down " + QString::number((uint64_t)e->GetEdgeLeftDown(), 16) + " | ", ErrorMessage::ErrorCode::Misc);
        e++;
    }

    while(f != _faces.end()){
        emit Message("Face " + QString::number((uint64_t)&(*f), 16) + " | " +
                     "Aresta " + QString::number((uint64_t)f->GetEdge(), 16) + " |", ErrorMessage::ErrorCode::Misc);
        f++;
    }
}

void WorkerGeometry::PrintVerticesFromFace(QString str){
    Face *face = (Face*)StrToAddr(str);
    QString text;

    if(face == nullptr){
        emit Message(str + " não encontrada.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    Edge *ebegin = face->GetEdge(), *itr, *before, *next;

    if(ebegin == nullptr){
        emit Message("Dados inconsistentes, a " + str + " possui aresta inválida.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    itr = ebegin;
    next = itr->GetSomeNextEdge(face);

    if(next == nullptr){
        emit Message("Dados inconsistentes, a " + str + " possui aresta inválida.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    text = "A " + str + " possui os vértices: " + QString::number((uint64_t)itr->GetSharedVertice(next), 16);

    do{
        before = itr;
        itr = next;

        next = itr->GetNextEdge(before);
        if(next == nullptr){
            emit Message("Dados inconsistentes, a " + str + " possui aresta inválida.", ErrorMessage::ErrorCode::Misc);
            return;
        }

        text.append(", " + QString::number((uint64_t)itr->GetSharedVertice(next), 16) );
    }while(next != ebegin);

    emit Message(text, ErrorMessage::ErrorCode::Misc);
}

void WorkerGeometry::PrintFacesFromEdge(QString str){

    Edge *ptr = (Edge*)StrToAddr(str);

    if(ptr == nullptr){
        emit Message(str + " não encontrada.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    QString text;

    if((ptr->GetFaceLeft() == nullptr) && (ptr->GetFaceRight() == nullptr)){
        emit Message("A " + str + " não possui faces adjacentes.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    text = "A " + str + " possui a Face ";

    if(ptr->GetFaceRight() == nullptr)
        text.append(QString::number((uint64_t)ptr->GetFaceLeft(), 16) + " ao seu lado esquerdo.");
    else if(ptr->GetFaceLeft() == nullptr)
        text.append(QString::number((uint64_t)ptr->GetFaceRight(), 16) + " ao seu lado direito.");
    else
        text.append(QString::number((uint64_t)ptr->GetFaceLeft(), 16) + " ao seu lado esquerdo e a Face " +
                    QString::number((uint64_t)ptr->GetFaceRight(), 16) + " ao seu lado direito.");

    emit Message(text, ErrorMessage::ErrorCode::Misc);

}

void WorkerGeometry::PrintEdgesFromVertice(QString str){

    //TEMP
    emit Message("Não Implementado.", ErrorMessage::ErrorCode::Misc);
    return;

    /*

    QHash<size_t, Edge>::Iterator curredge, eBegin;
    QHash<size_t, Vertice>::Iterator vertice = _vertices.find(v);
    QString text;
    size_t next;

    if(vertice == _vertices.end()){
        emit Message("Vértice " + QString::number((int)v) + " não encontrado.", ErrorMessage::ErrorCode::Misc);
        return;
    }

    eBegin = _edges.find(vertice->GetIncidentEdge());

    if(eBegin == _edges.end()){
        emit Message("Referência inválida para aresta no vértice " + QString::number((int)v) + ".", ErrorMessage::ErrorCode::Misc);
        return;
    }

    text = "O vértice " + QString::number((int)v) + " compartilha as arestas " + QString::number((int)eBegin.key());

    curredge = eBegin;
    if(curredge->GetFaceLeft() == -1){
        next = curredge->GetEdgeRightOut();
        while(next != -1){
            text.append(", " + QString::number((int)next));
            curredge = _edges.find(next);
            if(curredge == _edges.end()){
                emit Message("Referência inválida para vértice.", ErrorMessage::ErrorCode::Misc);
                return;
            }
        }
    }else if(curredge->GetFaceRight() == -1){

    }else{

    }
    */
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

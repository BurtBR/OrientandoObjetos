#include "workergeometry.h"

#include <QFile>
#include <QFileInfo>

WorkerGeometry::WorkerGeometry(QObject *parent) : QObject{parent}{

}

WorkerGeometry::~WorkerGeometry(){

}

bool WorkerGeometry::StrToFloat(const QString &str, float &number){
    bool ok;
    number = str.toFloat(&ok);
    return ok;
}

void WorkerGeometry::ParseFileData(){

    QHash<size_t, Face>::Iterator faceitr = _faces.begin();
    QHash<size_t, Edge>::Iterator initialedge, curredge, otheredge;
    QVector<size_t> visitedfaces;
    bool rightface;

    // WALK ADJACENT FACES USING QLIST WITH VISITED FACES

    emit Message("Processando dados do arquivo.", ErrorMessage::ErrorCode::Misc);

    CheckDuplicateVertices();

    if(!_faces.size()){
        _edges.clear();
        _vertices.clear();
        _faces.clear();
        emit Message("Objetos necessitam ao menos uma face.", ErrorMessage::ErrorCode::CorruptedFile);
        return;
    }

    visitedfaces.append(faceitr.key());

    for(int i=0; i<visitedfaces.size() ;i++){

        initialedge = _edges.find(_faces.find(visitedfaces[i])->GetEdge());
        curredge = initialedge;

        if(initialedge->GetFaceRight() == visitedfaces[i])
            rightface = true;
        else
            rightface = false;

        do{

            if(rightface){
                if(curredge->GetFaceLeft() == -1){
                    otheredge = FindEquivalentEdge(curredge.key());
                    if(otheredge != _edges.end()){

                        if(otheredge->GetFaceLeft() == -1){
                            if(curredge->GetVerticeOrigin() != otheredge->GetVerticeOrigin())
                                FlipFaceEdges(otheredge->GetFaceRight());
                            else
                                FlipFaceEdgesVertical(otheredge->GetFaceRight());
                        }else if(curredge->GetVerticeOrigin() != otheredge->GetVerticeOrigin()){
                            FlipFaceEdgesHorizontal(otheredge->GetFaceLeft());
                        }

                        curredge->SetEdgeLeftIn(otheredge->GetEdgeLeftIn());
                        curredge->SetEdgeLeftOut(otheredge->GetEdgeLeftOut());
                        curredge->SetFaceLeft(otheredge->GetFaceLeft());

                        if(!visitedfaces.contains(curredge->GetFaceLeft()))
                            visitedfaces.append(curredge->GetFaceLeft());

                        ReplaceEdgeReferences(otheredge.key(), curredge.key());
                        _edges.erase((QHash<size_t, Edge>::ConstIterator) otheredge);
                    }
                }
                curredge = _edges.find(curredge->GetEdgeRightOut());
            }else{
                if(curredge->GetFaceRight() == -1){
                    otheredge = FindEquivalentEdge(curredge.key());
                    if(otheredge != _edges.end()){

                        if(otheredge->GetFaceRight() == -1){
                            if(curredge->GetVerticeOrigin() != otheredge->GetVerticeOrigin())
                                FlipFaceEdges(otheredge->GetFaceLeft());
                            else
                                FlipFaceEdgesVertical(otheredge->GetFaceLeft());
                        }else if(curredge->GetVerticeOrigin() != otheredge->GetVerticeOrigin()){
                            FlipFaceEdgesHorizontal(otheredge->GetFaceRight());
                        }

                        curredge->SetEdgeRightIn(otheredge->GetEdgeRightIn());
                        curredge->SetEdgeRightOut(otheredge->GetEdgeRightOut());
                        curredge->SetFaceRight(otheredge->GetFaceRight());

                        if(!visitedfaces.contains(curredge->GetFaceRight()))
                            visitedfaces.append(curredge->GetFaceRight());

                        ReplaceEdgeReferences(otheredge.key(), curredge.key());
                        _edges.erase((QHash<size_t, Edge>::ConstIterator) otheredge);
                    }
                }
                curredge = _edges.find(curredge->GetEdgeLeftOut());
            }

        }while(initialedge != curredge);

        // If some isolated face exists
        if( (i == (visitedfaces.size()-1)) && (visitedfaces.size() != _faces.size()) ){
            faceitr++;
            while(visitedfaces.contains(faceitr.key()))
                faceitr++;
            visitedfaces.append(faceitr.key());
        }
    }

    emit Message("O arquivo contém " +
                 QString::number(_vertices.size()) + " vértices, " +
                 QString::number(_edges.size()) + " arestas e " +
                 QString::number(_faces.size()) + " faces.");

}

QHash<size_t, Edge>::Iterator WorkerGeometry::FindEquivalentEdge(size_t edge){
    QHash<size_t, Edge>::Iterator itr = _edges.begin(), equivalent = _edges.find(edge);

    while(itr != _edges.end()){
        // Different ID but same edge
        if((itr.key() != equivalent.key()) && (itr.value() == equivalent.value())){
            return itr;
        }
        itr++;
    }

    return _edges.end();
}

void WorkerGeometry::CheckDuplicateVertices(){

    QHash<size_t, Vertice>::Iterator itr = _vertices.begin(), itr2;

    while(itr != _vertices.end()){

        itr2 = itr;
        itr2++;

        while(itr2 != _vertices.end()){

            if(itr.value() == itr2.value()){
                emit Message("Vértice duplicado no arquivo: " +
                                 QString::number(itr->GetX()) + " " +
                                 QString::number(itr->GetY()) + " " +
                                 QString::number(itr->GetZ()), ErrorMessage::ErrorCode::CorruptedFile);
                ReplaceVerticeReference(itr2.key(), itr.key());
                itr2 = _vertices.erase((QHash<size_t, Vertice>::ConstIterator)itr2);
            }else{
                itr2++;
            }
        }

        itr++;
    }
}

void WorkerGeometry::ReplaceVerticeReference(size_t from, size_t to){
    QHash<size_t, Edge>::Iterator itr = _edges.begin();

    while(itr != _edges.end()){

        if(itr->GetVerticeOrigin() == from)
            itr->SetVerticeOrigin(to);
        else if(itr->GetVerticeDestination() == from)
            itr->SetVerticeDestination(to);

        itr++;
    }
}

void WorkerGeometry::ReplaceEdgeReferences(size_t from, size_t to){
    QHash<size_t, Vertice>::Iterator v = _vertices.begin();
    QHash<size_t, Edge>::Iterator e = _edges.begin();
    QHash<size_t, Face>::Iterator f = _faces.begin();

    while(v != _vertices.end()){
        if(v->GetIncidentEdge() == from)
            v->SetIncidentEdge(to);
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
}

void WorkerGeometry::FlipFaceEdges(size_t face){
    QHash<size_t, Edge>::Iterator curredge, startedge = _edges.find(_faces.find(face)->GetEdge());
    bool right;

    if(startedge == _edges.end())
        return;

    // As this method flip vertices and faces, this condition is inverted
    if(startedge->GetFaceRight() == face)
        right = false;
    else
        right = true;

    curredge = startedge;

    do{
        FlipEdge(curredge);
        if(right)
            curredge = _edges.find(curredge->GetEdgeRightOut());
        else
            curredge = _edges.find(curredge->GetEdgeLeftOut());
    }while(curredge != startedge);
}

void WorkerGeometry::FlipFaceEdgesHorizontal(size_t face){
    QHash<size_t, Edge>::Iterator curredge, startedge = _edges.find(_faces.find(face)->GetEdge());
    bool right;

    if(startedge == _edges.end())
        return;

    if(startedge->GetFaceRight() == face)
        right = true;
    else
        right = false;

    curredge = startedge;

    do{
        FlipEdgeHorizontalAxis(curredge);
        if(right)
            curredge = _edges.find(curredge->GetEdgeRightOut());
        else
            curredge = _edges.find(curredge->GetEdgeLeftOut());
    }while(curredge != startedge);
}

void WorkerGeometry::FlipFaceEdgesVertical(size_t face){
    QHash<size_t, Edge>::Iterator curredge, startedge = _edges.find(_faces.find(face)->GetEdge());
    bool right;

    if(startedge == _edges.end())
        return;

    // As this method flip faces, this condition is inverted
    if(startedge->GetFaceRight() == face)
        right = false;
    else
        right = true;

    curredge = startedge;

    do{
        FlipEdgeVerticalAxis(curredge);
        if(right)
            curredge = _edges.find(curredge->GetEdgeRightOut());
        else
            curredge = _edges.find(curredge->GetEdgeLeftOut());
    }while(curredge != startedge);
}

void WorkerGeometry::FlipEdge(QHash<size_t, Edge>::Iterator &e){
    QHash<size_t, Vertice>::Iterator v;
    size_t aux;

    v = _vertices.find(e->GetVerticeDestination());
    if(v->GetIncidentEdge() == e.key()){
        v->SetIncidentEdge(e->GetEdgeLeftIn());
    }

    v = _vertices.find(e->GetVerticeOrigin());
    if(v->GetIncidentEdge() == -1){
        v->SetIncidentEdge(e.key());
    }

    aux = e->GetVerticeOrigin();
    e->SetVerticeOrigin(e->GetVerticeDestination());
    e->SetVerticeDestination(aux);

    aux = e->GetEdgeLeftIn();
    e->SetEdgeLeftIn(e->GetEdgeRightIn());
    e->SetEdgeRightIn(aux);

    aux = e->GetEdgeLeftOut();
    e->SetEdgeLeftOut(e->GetEdgeRightOut());
    e->SetEdgeRightOut(aux);

    aux = e->GetFaceLeft();
    e->SetFaceLeft(e->GetFaceRight());
    e->SetFaceRight(aux);
}

void WorkerGeometry::FlipEdgeVerticalAxis(QHash<size_t, Edge>::Iterator &e){
    QHash<size_t, Vertice>::Iterator v;
    size_t aux;

    v = _vertices.find(e->GetVerticeDestination());
    if(v->GetIncidentEdge() == e->GetEdgeLeftIn()){
        v->SetIncidentEdge(e.key());
    }

    v = _vertices.find(e->GetVerticeOrigin());
    if(v->GetIncidentEdge() == e->GetEdgeRightIn()){
        v->SetIncidentEdge(e->GetEdgeLeftOut());
    }

    aux = e->GetEdgeLeftIn();
    e->SetEdgeLeftIn(e->GetEdgeRightOut());
    e->SetEdgeRightOut(aux);

    aux = e->GetEdgeLeftOut();
    e->SetEdgeLeftOut(e->GetEdgeRightIn());
    e->SetEdgeRightIn(aux);

    aux = e->GetFaceLeft();
    e->SetFaceLeft(e->GetFaceRight());
    e->SetFaceRight(aux);
}

void WorkerGeometry::FlipEdgeHorizontalAxis(QHash<size_t, Edge>::Iterator &e){
    QHash<size_t, Vertice>::Iterator v;
    size_t aux;

    v = _vertices.find(e->GetVerticeDestination());
    if(v->GetIncidentEdge() == e.key()){
        v->SetIncidentEdge(e->GetEdgeRightOut());
    }

    v = _vertices.find(e->GetVerticeOrigin());
    if(v->GetIncidentEdge() == e->GetEdgeRightIn()){
        v->SetIncidentEdge(e.key());
    }

    aux = e->GetVerticeOrigin();
    e->SetVerticeOrigin(e->GetVerticeDestination());
    e->SetVerticeDestination(aux);

    aux = e->GetEdgeLeftIn();
    e->SetEdgeLeftIn(e->GetEdgeLeftOut());
    e->SetEdgeLeftOut(aux);

    aux = e->GetEdgeRightIn();
    e->SetEdgeRightIn(e->GetEdgeRightOut());
    e->SetEdgeRightOut(aux);
}

void WorkerGeometry::OpenObj(QString filename){

    QFile fp(filename);
    QFileInfo fileinfo(fp);
    QStringList strlist, unknownparameters;
    float x, y, z;
    size_t idx, origin, destination, firstvertice, firstedge;

    if(!fileinfo.exists()){
        emit Message("O arquivo não existe.", ErrorMessage::ErrorCode::FailedToOpenFile);
        emit FileHandlingFinished();
        return;
    }

    if(!fp.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit Message("Acesso negado.", ErrorMessage::ErrorCode::FailedToOpenFile);
        emit FileHandlingFinished();
        return;
    }

    emit Message("Carregando arquivo: " + fileinfo.fileName());
    _edges.clear();
    _vertices.clear();
    _faces.clear();

    while(!fp.atEnd()){
        strlist.clear();
        strlist.append(fp.readLine().trimmed());
        if(strlist[0].size()){
            strlist = strlist[0].split(' ', Qt::SkipEmptyParts);

            switch(strlist[0][0].toLatin1()){

            case 'v':
                if(strlist.size() != 4){
                    emit Message("Valores incompatíveis de " + strlist[0], ErrorMessage::ErrorCode::CorruptedFile);
                }else if(strlist[0].size() == 1){ // Vertice

                    if(!StrToFloat(strlist[1], x) || !StrToFloat(strlist[2], y) || !StrToFloat(strlist[3], z))
                        emit Message("Formatação incompatível de ponto flutuante: " +
                                     strlist[1] + " " + strlist[2] + " " + strlist[3],
                                     ErrorMessage::ErrorCode::CorruptedFile);
                    else{
                        _vertices.insert(_vertices.size(), Vertice(x,y,z));
                        //_vertices.append(Vertice(x,y,z));
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
                    break;
                }
                idx = strlist[1].split('/')[0].toLongLong()-1;
                if(idx >= _vertices.size()){
                    emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    break;
                }

                firstvertice = idx;
                origin = firstvertice;

                idx = strlist[2].split('/')[0].toLongLong()-1;

                if(!_vertices.contains(idx)){
                    emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    break;
                }

                destination = idx;

                _edges.insert(_edges.size(), Edge(origin, destination, -1, _faces.size()));
                _faces.insert(_faces.size(), Face(_edges.size()-1));

                _vertices.find(destination)->SetIncidentEdge(_edges.size()-1);

                firstedge = (_edges.size()-1);

                for(int i=3; i<strlist.size() ;i++){
                    origin = destination;
                    idx = strlist[i].split('/')[0].toLongLong()-1;
                    if(!_vertices.contains(idx)){
                        emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    }else{
                        destination = idx;
                        _edges[_edges.size()-1].SetEdgeRightOut(_edges.size());
                        _edges.insert(_edges.size(), Edge(origin, destination, -1, _faces.size()-1, -1, -1, _edges.size()-1));
                        _vertices.find(destination)->SetIncidentEdge(_edges.size()-1);
                    }
                }

                if(destination != firstvertice){
                    origin = destination;
                    destination = firstvertice;
                    _edges[_edges.size()-1].SetEdgeRightOut(_edges.size());
                    _edges.insert(_edges.size(), Edge(origin, destination, -1, _faces.size()-1, -1, -1, _edges.size()-1, firstedge));
                    _vertices.find(destination)->SetIncidentEdge(_edges.size()-1);
                }else{
                    _edges[_edges.size()-1].SetEdgeRightOut(firstedge);
                }

                _edges[firstedge].SetEdgeRightIn(_edges.size()-1);

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

    ParseFileData();

    emit FileHandlingFinished();
}

void WorkerGeometry::PrintAllData(){

    if(!_vertices.size()){
        emit Message("Não há estruturas para imprimir", ErrorMessage::ErrorCode::Misc);
        return;
    }

    QHash<size_t, Vertice>::Iterator v = _vertices.begin();
    QHash<size_t, Edge>::Iterator e = _edges.begin();
    QHash<size_t, Face>::Iterator f = _faces.begin();

    while(v != _vertices.end()){
        emit Message("Vértice " + QString::number(v.key()).rightJustified(3,' ') + " | (  X;  Y;  Z)=(" +
                     QString::number(v->GetX()).rightJustified(3,' ') + ";" +
                     QString::number(v->GetY()).rightJustified(3,' ') + ";" +
                     QString::number(v->GetZ()).rightJustified(3,' ') + ") |" +
                     " Aresta " + QString::number((int)v->GetIncidentEdge()).rightJustified(3,' ') + " |", ErrorMessage::ErrorCode::Misc);
        v++;
    }

    while(e != _edges.end()){
        emit Message("Aresta " + QString::number(e.key()).rightJustified(3,' ') + "\n| "+
                     "Orig. " + QString::number(e->GetVerticeOrigin()).rightJustified(3,' ') + " | "+
                     "Dest. " + QString::number(e->GetVerticeDestination()).rightJustified(3,' ') + " |\n| "+
                     "Face Dir. " + QString::number((int)e->GetFaceRight()).rightJustified(3,' ') + " | "+
                     "Dir. In " + QString::number((int)e->GetEdgeRightIn()).rightJustified(3,' ') + " | "+
                     "Dir. Out " + QString::number((int)e->GetEdgeRightOut()).rightJustified(3,' ') + " |\n| "+
                     "Face Esq. " + QString::number((int)e->GetFaceLeft()).rightJustified(3,' ') + " | "+
                     "Esq. In " + QString::number((int)e->GetEdgeLeftIn()).rightJustified(3,' ') + " | "+
                     "Esq. Out " + QString::number((int)e->GetEdgeLeftOut()).rightJustified(3,' ') + " | ", ErrorMessage::ErrorCode::Misc);
        e++;
    }

    while(f != _faces.end()){
        emit Message("Face " + QString::number(f.key()).rightJustified(3,' ') + " | " +
                     "Aresta " + QString::number(f->GetEdge()).rightJustified(3,' ') + " |", ErrorMessage::ErrorCode::Misc);
        f++;
    }
}

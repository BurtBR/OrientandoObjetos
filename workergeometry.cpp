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
    for(int i=0; i<_vertices.size() ;i++){
        emit Message("Vértice " + QString::number(i) + ": " +
                     QString::number(_vertices[i].GetX()) + " " +
                     QString::number(_vertices[i].GetY()) + " " +
                     QString::number(_vertices[i].GetZ()) );
    }

    for(int i=0; i<_edges.size() ;i++){
        emit Message("Aresta " + QString::number(i) + ": " +
                     "Origem "+ QString::number(_edges[i].GetVerticeOrigin()) +
                     ", Destino " + QString::number(_edges[i].GetVerticeDestination()) +
                     ", DirIn " + QString::number(_edges[i].GetEdgeRightIn()) +
                     ", DirOut " + QString::number(_edges[i].GetEdgeRightOut()) +
                     ", Face " + QString::number(_edges[i].GetFaceRight()));
    }

    for(int i=0; i<_faces.size() ;i++){
        emit Message("Face " + QString::number(i) +
                     ", Aresta " + QString::number(_faces[i].GetEdge()));
    }

    QVector<Vertice> verticeaux;
    QVector<Edge> edgesaux;

    for(int i=0; i<_vertices.size() ;i++){
        if(!verticeaux.contains(_vertices[i]))
            verticeaux.append(_vertices[i]);
        else
            emit Message("Vértice duplicado no arquivo: " +
                         QString::number(_vertices[i].GetX()) + " " +
                         QString::number(_vertices[i].GetY()) + " " +
                         QString::number(_vertices[i].GetZ()), ErrorMessage::ErrorCode::CorruptedFile);
    }

    if(!_faces.size()){
        _edges.clear();
        _vertices.clear();
        _faces.clear();
        emit Message("Objetos necessitam ao menos uma face.", ErrorMessage::ErrorCode::CorruptedFile);
        return;
    }



    emit Message("O arquivo contém " +
                 QString::number(_vertices.size()) + " vértices, " +
                 QString::number(_edges.size()) + " arestas e " +
                 QString::number(_faces.size()) + " faces.");

}

void WorkerGeometry::OpenObj(QString filename){

    QFile fp(filename);
    QFileInfo fileinfo(fp);
    QStringList strlist;
    float x, y, z;
    size_t idx, origin, destination, firstvertice, firstedge;

    if(!fileinfo.exists()){
        emit Message("O arquivo não existe", ErrorMessage::ErrorCode::FailedToOpenFile);
        emit FileHandlingFinished();
        return;
    }

    if(!fp.open(QIODevice::ReadOnly | QIODevice::Text)){
        emit Message("Acesso negado", ErrorMessage::ErrorCode::FailedToOpenFile);
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

            case 'p': // Point
                emit Message("\"" + strlist[0] + "\" não implementado.", ErrorMessage::ErrorCode::CorruptedFile);
                break;

            case 'l': // Line
                emit Message("\"" + strlist[0] + "\" não implementado.", ErrorMessage::ErrorCode::CorruptedFile);
                break;

            case 'v':
                if(strlist.size() != 4){
                    emit Message("Valores incompatíveis de " + strlist[0], ErrorMessage::ErrorCode::CorruptedFile);
                }else if(strlist[0].size() == 1){ // Vertice

                    if(!StrToFloat(strlist[1], x) || !StrToFloat(strlist[2], y) || !StrToFloat(strlist[3], z))
                        emit Message("Formatação incompatível de ponto flutuante: " +
                                     strlist[1] + " " + strlist[2] + " " + strlist[3],
                                     ErrorMessage::ErrorCode::CorruptedFile);
                    else{
                        _vertices.append(Vertice(x,y,z));
                    }

                }else{
                    switch(strlist[0][1].toLatin1()){
                    //case 'n':
                    //case 't':
                    default:
                        emit Message("\"" + strlist[0] + "\" não implementado.", ErrorMessage::ErrorCode::CorruptedFile);
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
                if(idx >= _vertices.size()){
                    emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    break;
                }

                destination = idx;

                _edges.append(Edge(origin, destination, -1, _faces.size()));
                _faces.append(Face(_edges.size()-1));

                firstedge = (_edges.size()-1);

                for(int i=3; i<strlist.size() ;i++){
                    origin = destination;
                    idx = strlist[i].split('/')[0].toLongLong()-1;
                    if(idx >= _vertices.size()){
                        emit Message("Índices de vértices inválidos: " + strlist.join(' '), ErrorMessage::ErrorCode::CorruptedFile);
                    }else{
                        destination = idx;
                        _edges.last().SetEdgeRightOut(_edges.size());
                        _edges.append(Edge(origin, destination, -1, _faces.size()-1, -1, -1, _edges.size()-1));
                    }
                }

                if(destination != firstvertice){
                    origin = destination;
                    destination = firstvertice;
                    _edges.last().SetEdgeRightOut(_edges.size());
                    _edges.append(Edge(origin, destination, -1, _faces.size()-1, -1, -1, _edges.size()-1, firstedge));
                }else{
                    _edges.last().SetEdgeRightOut(firstedge);
                }

                _edges[firstedge].SetEdgeRightIn(_edges.size()-1);

                break;

            case '#':
                break;

            default:
                emit Message("\"" + strlist[0] + "\" não implementado.", ErrorMessage::ErrorCode::CorruptedFile);
                break;
            }
        }
    }

    fp.close();

    ParseFileData();

    emit FileHandlingFinished();
}

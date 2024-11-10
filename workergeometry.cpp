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

void WorkerGeometry::OpenObj(QString filename){

    QFile fp(filename);
    QFileInfo fileinfo(fp);
    QStringList strlist;
    QVector<QSet<Vertice>::Iterator> vertice_arr;
    float faux, x, y, z;

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

            //qDebug() << strlist;

            switch(strlist[0][0].toLatin1()){

            case 'p': // Point

                break;

            case 'l': // Line

                break;

            case 'v':
                if(strlist.size() != 4){
                    emit Message("Valores incompatíveis de " + strlist[0], ErrorMessage::ErrorCode::CorruptedFile);
                }else if(strlist[0].size() == 1){ // Vertice

                    if(!StrToFloat(strlist[1], x) || !StrToFloat(strlist[2], y) || !StrToFloat(strlist[3], z))
                        emit Message("Formatação incompatível de ponto flutuante: " +
                                     strlist[1] + " " + strlist[2] + " " + strlist[3],
                                     ErrorMessage::ErrorCode::CorruptedFile);
                    else if(!_vertices.contains(Vertice(x,y,z))){
                        vertice_arr.append(_vertices.insert(Vertice(x,y,z)));
                    }else{
                        emit Message("Vértices duplicados no arquivo: " +
                                     strlist[1] + " " + strlist[2] + " " + strlist[3],
                                     ErrorMessage::ErrorCode::CorruptedFile);
                        vertice_arr.append(vertice_arr.last());
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
                for(int i=1; i<strlist.size() ;i++){

                }
                break;

            case '#':
                break;

            default:
                emit Message("\"" + strlist[0] + "\" não implementado.", ErrorMessage::ErrorCode::CorruptedFile);
                break;
            }
        }
    }

    //TEMP
    for(int i=0; i<vertice_arr.size() ;i++){
        qDebug() << vertice_arr[i]->GetX() << vertice_arr[i]->GetY() << vertice_arr[i]->GetZ();
    }

    fp.close();
    emit FileHandlingFinished();
}

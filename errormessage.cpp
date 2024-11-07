#include "errormessage.h"

ErrorMessage::ErrorMessage(){

}

QString ErrorMessage::GetErrorStr(ErrorMessage::ErrorCode e){
    switch(e){
    case ErrorCode::FailedToWrite:
        return "Falha ao escrever em arquivo.";
        break;

    case ErrorCode::FailedToOpenFile:
        return "Falha ao abrir o arquivo.";
        break;

    case ErrorCode::FailedToAllocate:
        return "Falha de alocação de recursos.";
        break;

    default:
        return QString();
    }
}

#include "operation.h"

Operation::Operation(){

}

Operation::Operation(float x, float y, float z, OpType op){
    SetOperation(x, y, z, op);
}

Operation::OpType Operation::GetOpType(){
    return _op;
}

QMatrix4x4 Operation::GetMatrix(){
    return _m;
}

void Operation::SetTranslation(float x, float y, float z){
    _m.setToIdentity();
    _op = OpType::Translation;
    _paramx = x;
    _paramy = y;
    _paramz = z;

    _m.setRow(3, QVector4D(x, y, z, 1));
}

void Operation::SetRotationX(float angle){
    SetRotation(angle, 0, 0);
}

void Operation::SetRotationY(float angle){
    SetRotation(0, angle, 0);
}

void Operation::SetRotationZ(float angle){
    SetRotation(0, 0, angle);
}

void Operation::SetRotation(float anglex, float angley, float anglez){
    _op = OpType::Rotation;
    _paramx = anglex;
    _paramy = angley;
    _paramz = anglez;

    anglex = (anglex*M_PI/180);
    angley = (angley*M_PI/180);
    anglez = (anglez*M_PI/180);

    _m.setRow(0, QVector4D(
              RoundFloat(qCos(angley)*qCos(anglez)),
              RoundFloat(qCos(angley)*qSin(anglez)),
              RoundFloat(-qSin(angley)),
              0));

    _m.setRow(1, QVector4D(
              RoundFloat(qCos(anglez)*qSin(anglex)*qSin(angley) - qCos(anglex)*qSin(anglez)),
              RoundFloat(qCos(anglex)*qCos(anglez) + qSin(anglex)*qSin(angley)*qSin(anglez)),
              RoundFloat(qCos(angley)*qSin(anglex)),
              0));

    _m.setRow(2, QVector4D(
              RoundFloat(qSin(anglex)*qSin(anglez) + qCos(anglex)*qCos(anglez)*qSin(angley)),
              RoundFloat(qCos(anglex)*qSin(angley)*qSin(anglez) - qCos(anglez)*qSin(anglex)),
              RoundFloat(qCos(anglex)*qCos(angley)),
              0));

    _m.setRow(3, QVector4D(
              0,
              0,
              0,
              1));
}

void Operation::SetScale(float x, float y, float z){
    _m.setToIdentity();
    _op = OpType::Scale;
    _paramx = x;
    _paramy = y;
    _paramz = z;

    _m.data()[0] = x;
    _m.data()[5] = y;
    _m.data()[10] = z;
}

void Operation::SetShearX(float y, float z){
    _m.setToIdentity();
    _op = OpType::ShearX;
    _paramx = 0;
    _paramy = y;
    _paramz = z;

    _m.setRow(0, QVector4D(1, y, z, 0));
}

void Operation::SetShearY(float x, float z){
    _m.setToIdentity();
    _op = OpType::ShearY;
    _paramx = x;
    _paramy = 0;
    _paramz = z;
    _m.setRow(1, QVector4D(x, 1, z, 0));
}

void Operation::SetShearZ(float x, float y){
    _m.setToIdentity();
    _op = OpType::ShearZ;
    _paramx = x;
    _paramy = y;
    _paramz = 0;

    _m.setRow(2, QVector4D(x, y, 1, 0));
}

float Operation::RoundFloat(float number){

    if(abs(number) < 1E-7)
        number = 0;

    return number;
}

QString Operation::GetOperationStr(const OpType op){
    switch(op){
    case OpType::Translation:
        return "Translação";
        break;

    case OpType::Rotation:
        return "Rotação";
        break;

    case OpType::Scale:
        return "Escala";
        break;

    case OpType::ShearX:
        return "Cisalhamento X";
        break;

    case OpType::ShearY:
        return "Cisalhamento Y";
        break;

    case OpType::ShearZ:
        return "Cisalhamento Z";
        break;

    default:
        break;
    }

    return QString();
}

void Operation::SetOperation(float x, float y, float z, OpType op){
    switch(op){
    case OpType::Translation:
        SetTranslation(x, y, z);
        break;

    case OpType::Rotation:
        SetRotation(x, y, z);
        break;

    case OpType::Scale:
        SetScale(x, y, z);
        break;

    case OpType::ShearX:
        SetShearX(y, z);
        break;

    case OpType::ShearY:
        SetShearY(x, z);
        break;

    case OpType::ShearZ:
        SetShearZ(x, y);
        break;

    default:
        break;
    }
}

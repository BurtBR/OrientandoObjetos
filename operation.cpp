#include "operation.h"

Operation::Operation(){

}

Operation::OpType Operation::GetOpType(){
    return _op;
}

QMatrix4x4 Operation::GetMatrix(){
    return _m;
}

void Operation::SetTranslation(float x, float y, float z){
    _op = OpType::Translation;
    _paramx = x;
    _paramy = y;
    _paramz = z;
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
}

void Operation::SetScale(float x, float y, float z){
    _op = OpType::Scale;
    _paramx = x;
    _paramy = y;
    _paramz = z;
}

void Operation::SetShearX(float y, float z){
    _op = OpType::ShearX;
    _paramx = 0;
    _paramy = y;
    _paramz = z;
}

void Operation::SetShearY(float x, float z){
    _op = OpType::ShearY;
    _paramx = x;
    _paramy = 0;
    _paramz = z;
}

void Operation::SetShearZ(float x, float y){
    _op = OpType::ShearZ;
    _paramx = x;
    _paramy = y;
    _paramz = 0;
}

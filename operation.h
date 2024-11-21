#ifndef OPERATION_H
#define OPERATION_H

#include <QtMath>
#include <QMatrix4x4>

class Operation{

public:
    enum class OpType{
        Translation,
        Rotation,
        Scale,
        ShearX,
        ShearY,
        ShearZ
    };

private:
    OpType _op = OpType::Translation;
    QMatrix4x4 _m = QMatrix4x4(1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, 0, 1);
    float _paramx = 0, _paramy = 0, _paramz = 0;

public:
    Operation();
    Operation(float x, float y, float z, OpType op);
    OpType GetOpType();
    QMatrix4x4 GetMatrix();
    void SetTranslation(float x, float y = 0, float z = 0);
    void SetRotationX(float angle);
    void SetRotationY(float angle);
    void SetRotationZ(float angle);
    void SetRotation(float anglex, float angley, float anglez = 0);
    void SetScale(float x, float y, float z = 0);
    void SetShearX(float y, float z);
    void SetShearY(float x, float z);
    void SetShearZ(float x, float y);
    void SetOperation(float x, float y, float z, OpType op);
    QString GetOperationStr();

    static QString GetOperationStr(const OpType op);
    static float RoundFloat(float number);
};

#endif // OPERATION_H

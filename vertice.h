#ifndef VERTICE_H
#define VERTICE_H

#include <QGenericMatrix>
#include <QMatrix4x4>
#include <QString>

class Edge;

class Vertice{
private:
    QString _id;
    QGenericMatrix<1, 4, float> _position;
    Edge *_incident = nullptr;
public:
    Vertice(QString id = QString(), float x=0, float y=0, float z=0, float w = 1.0, Edge *incident = nullptr);
    Vertice(const Vertice &other);

    bool operator==(const Vertice &other) const;

    bool HasIncidentEdge();
    void SetId(QString id);
    void SetPosition(QGenericMatrix<1, 4, float> pos);
    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    void SetW(float w);
    void SetXYZW(float x, float y, float z, float w);
    void SetIncidentEdge(Edge *e);
    QString GetId() const;
    QGenericMatrix<1, 4, float> GetPosition() const;
    float GetX() const;
    float GetY() const;
    float GetZ() const;
    float GetW() const;
    Edge *GetIncidentEdge() const;
    QGenericMatrix<4, 1, float> Operate(QGenericMatrix<4, 4, float> operation);
};

#endif // VERTICE_H

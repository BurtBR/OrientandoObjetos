#ifndef FACE_H
#define FACE_H

#include <QString>

class Edge;

class Face{
private:
    QString _id;
    Edge *_edge;
public:
    Face(QString id = QString(), Edge *edge = nullptr);

    void SetId(QString id);
    void SetEdge(Edge *e);
    QString GetId() const;
    Edge *GetEdge() const;
};

#endif // FACE_H

#ifndef FACE_H
#define FACE_H

#include "edge.h"

class Face{
private:
    Edge *_edge = nullptr;
public:
    Face(Edge *edge = nullptr);

    void SetEdge(Edge *e);
    Edge &GetEdge() const;
};

inline bool operator==(const Face &v1, const Face &v2){
    return ( v1 == v2 );
}

inline size_t qHash(const Face &key, size_t seed){
    return qHash(key.GetEdge(), seed);
}

#endif // FACE_H

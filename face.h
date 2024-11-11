#ifndef FACE_H
#define FACE_H

#include <cstddef>

class Face{
private:
    size_t _edge = -1;
public:
    Face(size_t edge = -1);

    void SetEdge(size_t e);
    size_t GetEdge() const;
};

#endif // FACE_H

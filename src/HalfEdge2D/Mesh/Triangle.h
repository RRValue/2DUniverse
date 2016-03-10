#ifndef _MESH_TRIANGLE_H_
#define _MESH_TRIANGLE_H_

#include <array>

class Triangle : public std::array<size_t, 3>
{
public:
    Triangle();
    ~Triangle();
};

#endif //_MESH_TRIANGLE_H_

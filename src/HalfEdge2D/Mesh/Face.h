#ifndef _MESH_FACE_H_
#define _MESH_FACE_H_

#include "HalfEdge2D/Base/Vector.h"

#include <vector>

class Face
{
public:
    Face();
    Face(const Face& other);
    virtual ~Face();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // getter
    size_t getNumVert() const;
    const std::vector<size_t>& getVertIds() const;
    const size_t& getVertIdx(const size_t& idx) const;
    const Vec4f& getColour() const;

    // setter
    void addVertIdx(const size_t& idxVert);
    void addVertIdx(const std::vector<size_t>& idxs);
    void addVertIdx(const std::initializer_list<size_t>& idxs);
    void setVertIdx(const size_t& idx, const size_t& idxVert);
    void setColour(const Vec4f& color);

private:
    Vec4f m_Colour;
    std::vector<size_t> m_idx;
};

#endif //_MESH_FACE_H_

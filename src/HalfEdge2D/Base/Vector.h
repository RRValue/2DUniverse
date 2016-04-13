#ifndef _BASE_VECTOR_H_
#define _BASE_VECTOR_H_

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

#include <Eigen\Dense>

typedef Eigen::Matrix<float, 2, 1>  Vec2f;
typedef Eigen::Matrix<int, 2, 1>  Vec2i;
typedef Eigen::Matrix<double, 2, 1>  Vec2d;
typedef Eigen::Matrix<float, 3, 1>  Vec3f;
typedef Eigen::Matrix<float, 4, 1>  Vec4f;
typedef Eigen::Matrix<float, 2, 2>  Mat2f;
typedef Eigen::Matrix<float, 3, 3>  Mat3f;

typedef std::vector<Vec2f, Eigen::aligned_allocator<Vec2f>> Vec2fVec;

#endif //_BASE_VECTOR_H_

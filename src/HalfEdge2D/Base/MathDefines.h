#ifndef _BASE_MATHDEFINES_H_
#define _BASE_MATHDEFINES_H_

#define _USE_MATH_DEFINES
#include <cmath>

static float PI_F = (float)M_PI;
static double PI_D = (double)M_PI;

static float PI2_F = PI_F * 2.0f;
static double PI2_D = PI_D * 2.0;

static float factorDegToRadF = PI_F / 180.0f;
static double factorDegToRadD = PI_D / 180.0;

static float factorRadToDegF = 1.0f / factorDegToRadF;
static double factorRadToDegD = 1.0 / factorDegToRadD;

#define degToRadF(v) (factorDegToRadF * v)
#define degToRadD(v) (factorDegToRadD * v)

#define radToDegF(v) (factorRadToDegF * v)
#define radToDegD(v) (factorRadToDegD * v)

#endif //_BASE_MATHDEFINES_H_

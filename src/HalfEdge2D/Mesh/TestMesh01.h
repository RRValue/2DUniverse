#ifndef _MESH_TESTMESH_01_H_
#define _MESH_TESTMESH_01_H_

#include <vector>

std::vector<float> testVertices01 = {0.309016994375f, 0.951056516295f,
-0.809016994375f, 0.587785252292f,
-0.809016994375f, -0.587785252292f,
0.309016994375f, -0.951056516295f,
1.0f, -2.44929359829e-16f,
-0.249999999999f, 0.769420884294f,
-0.809016994375f, 1.62658775338e-12f,
-0.250000000001f, -0.769420884293f,
0.654508497187f, -0.475528258149f,
0.654508497188f, 0.475528258146f,
1.01574304523e-13f, 1.16129328376e-14f,
-0.404508497187f, 0.293892626147f,
-0.404508497188f, -0.293892626145f,
0.5f, -9.69359134884e-13f,
0.154508497187f, -0.475528258148f,
0.154508497189f, 0.475528258147f
};

std::vector<int> testTriangles01 = {1, 11, 5,
2, 12, 6,
3, 14, 7,
4, 13, 8,
0, 15, 9,
10, 15, 11,
10, 12, 14,
7, 14, 12,
5, 11, 15,
6, 12, 11,
9, 15, 13,
8, 13, 14,
10, 14, 13,
10, 13, 15,
10, 11, 12,
1, 6, 11,
2, 7, 12,
4, 9, 13,
3, 8, 14,
0, 5, 15
};

#endif //_MESH_TESTMESH_01_H_

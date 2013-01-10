#pragma once

#include "particle.h"
#include "verletConstraint.h"

class Cloth {
public:
	Cloth(GsArray<particle*> * particleArr, GsArray<verletConstraint*> * vconstr, SnGroup* grp, GsVec origin, int u, int v, bool horizontal);
	particle*** particles;
	int dimx, dimy;
};
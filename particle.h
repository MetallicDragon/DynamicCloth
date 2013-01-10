#pragma once

#include <math.h>
#include <gsim/gs_vec.h>
#include <gsim/sn_group.h>
#include <gsim/sn_sphere.h>
#include <gsim/sn_cylinder.h>
#include <gsim/sn_transform.h>

class particle
{
public:
	particle(void);
	~particle(void);
	void update(float dt);
	void updateVerlet();
	void applyForce(GsVec f);
	GsVec* position;
	GsVec* prevPos;
	GsVec* velocity;
	GsVec* gravity;
	GsVec* extforce;
	SnGroup* sng;
	SnSphere* sphere;
	SnTransform* transform;
	float restorationFactor;
	bool fixed;
};


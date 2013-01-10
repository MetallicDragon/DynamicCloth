#pragma once

#include "particle.h"


class forceObject {
public:
	forceObject(void);
	forceObject(particle* p) {this->p = p;}
	~forceObject(void);
	virtual void applyForce();
	particle* p;
	GsVec force;

};

class forceSphereCollision: public forceObject {
public:
	particle* p2;
	float ks, restlen;
	forceSphereCollision(particle* first, particle* second) {
		p = first;
		p2 = second;
		ks = 10.0f;
		restlen = p->sphere->radius() + p2->sphere->radius();
	}
	void applyForce() {
		GsVec radiusVec = *(p->position) - *(p2->position);
		float radius = abs(radiusVec.len());
		
		if (true/*radius < restlen*/) {
			//if the particles are intersecting, then apply collision force
			radiusVec.normalize();
			force = 1 * (ks * (abs(restlen) - radius)) * radiusVec;
			p->applyForce(force);
			p2->applyForce(-1 * force);
		}
	}

};

class forceDampedSpring: public forceObject {
public:
	particle* p2;
	float ks, kd, restlen;
	forceDampedSpring(particle* first, particle* second) {
		p = first;
		p2 = second;
		ks = 25.0f;
		kd = 1.2f;
		restlen = p->sphere->radius() + p2->sphere->radius();
	}
	void applyForce() {
		GsVec radiusVec = *(p->position) - *(p2->position);
		float radius = abs(radiusVec.len());


		radiusVec;
		GsVec deltaRadius = *(p->velocity) - *(p2->velocity);
		force = -1 * (ks*(radius-restlen) + kd*dot(deltaRadius, radiusVec)/radius) * radiusVec/radius;
	
		p->applyForce(force);
		p2->applyForce(-1 * force);
	}

};


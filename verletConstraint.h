#pragma once

#include "particle.h"
#include <math.h>

class verletConstraint {
public:
	verletConstraint(void);
	verletConstraint(particle* p) {this->p = p;}
	~verletConstraint(void);
	particle* p;
	GsVec force;
	virtual void applyConstraint();

};

class verletConstraintFixedDistance: public verletConstraint {
public:

	particle* p2;
	float constr_dist, cd2;
	float str;
	verletConstraintFixedDistance(particle* p, particle* p2, float d) {
		this->p = p;
		this->p2 = p2;
		constr_dist = d;
		str = 1.0f;
	}
	verletConstraintFixedDistance(particle* p, particle* p2, float d, float s) {
		this->p = p;
		this->p2 = p2;
		constr_dist = d;
		cd2 = d*d;
		str = s;
	}
	void applyConstraint() {
		GsVec delta = *(p2->position) - *(p->position);
		float deltalength = delta.len();
		float diff = (deltalength - constr_dist)/deltalength;
		//delta *= (cd2)/((delta.x*delta.x +delta.y*delta.y + delta.z*delta.z) + cd2) - 0.5;
		if (p->fixed && !p2->fixed) {
			//case that p is fixed, so p2 must be moved more than normal 
			*(p2->position) -= delta * str * diff;
			//*(p2->position) -= delta * str;
		} else if (!p->fixed && p2->fixed) {
			//case that p2 is fixed
			*(p->position) += delta * str * diff;
			//*(p->position) += delta * str;
		} else if (!p->fixed && !p2->fixed) {
			//neither is fixed, proceed as normal
			*(p->position) += delta * 0.5 * str * diff;
			*(p2->position) -= delta * 0.5 * str * diff;
			//*(p->position) += delta;// * 0.5 * str;
			//*(p2->position) -= delta;// * 0.5 * str;
		}
		//if both are fixed, nothing happens
	}


};

class verletConstraintMinDistance: public verletConstraint {
public:

	particle* p2;
	float constr_dist, cd2;
	verletConstraintMinDistance(particle* p, particle* p2, float d) {
		this->p = p;
		this->p2 = p2;
		constr_dist = d;
		cd2 = d*d;
	}
	void applyConstraint() {
		GsVec delta = *(p2->position) - *(p->position);
		float d2 = delta.x*delta.x + delta.y*delta.y + delta.z*delta.z;
		if (d2 < cd2) {
			float deltalength = delta.len();
			float diff = (deltalength - constr_dist)/deltalength;
			if (p->fixed && !p2->fixed) {
				//case that p is fixed, so p2 must be moved more than normal 
				*(p2->position) -= delta * 1.0 * diff;
			} else if (!p->fixed && p2->fixed) {
				//case that p2 is fixed
				*(p->position) += delta * 1.0 * diff;
			} else if (!p->fixed && !p2->fixed) {
				//neither is fixed, proceed as normal
				*(p->position) += delta * 0.5 * diff;
				*(p2->position) -= delta * 0.5 * diff;
			}
			//if both are fixed, nothing happens
		}
	}


};
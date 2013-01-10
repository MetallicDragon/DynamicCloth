#include "cloth.h"

Cloth::Cloth(GsArray<particle*> * particleArr, GsArray<verletConstraint*> * vconstr, SnGroup* grp, GsVec origin, int u, int v, bool horizontal) {
	dimx = u;
	dimy = v;
	particles = new particle**[dimx];
	float str = 1.0f;
	for (int i = 0; i < dimx; i++) {
		particles[i] = new particle*[dimy];
	}
	for (int i = 0; i < dimx; i++) {
		for (int j = 0; j < dimy; j++) {
			particle* p = new particle();
			grp->add(p->sng);
			particles[i][j] = p;//add to array of particles which are part of this cloth
			particleArr->push(p);//add to array of all particles in scene
		}
	}

	for (int i = 0; i < dimx; i++) {
		for (int j = 0; j < dimy; j++) {
			particle* p = particles[i][j];
			particle* p2;
			float radius = p->sphere->radius();
			GsVec pos(origin);
			if (horizontal) {
				pos.x += 2.0f * radius * j;
				pos.y += 0;
				pos.z += 2.0f * radius * i;
			} else {
				pos.x += 2.0f * radius * j ;
				pos.y += 1 - (2.0f * radius)*i;
				//pos.z += 0 - radius*i;
			}
			p->position->set(pos.x,pos.y,pos.z); //set initial position
			p->prevPos->set(GsVec(*(p->position)));//set prevpos, for verlet purposes
			
			// STRETCH SPRINGS, vertical + horizontal by 1
			if (j<(dimy-1)) { //i.e. not on rightmost edge
				//connect particles left-right
				p2 = particles[i][j+1];
				radius = p->sphere->radius() + p2->sphere->radius();
				verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
				vconstr->push(c);
			}
			if (i < dimx-1) {
				//connect particles up-down
				p2 = particles[i+1][j];
				radius = p->sphere->radius() + p2->sphere->radius();
				verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
				vconstr->push(c);
			}

			//SHEAR SPRINGS, diagonal by 1
			if (j<(dimy-1)) {//i.e. not rightmost edge
				if (i > 0) {//i.e. not topmost
					p2 = particles[i-1][j+1];
					radius = p->sphere->radius() + p2->sphere->radius();
					radius *= sqrt(2.0f); //since they are diagonal
					verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
					vconstr->push(c);
				}
				if (i<(dimx-1)) {//i.e. not bottom-most
					p2 = particles[i+1][j+1];
					radius = p->sphere->radius() + p2->sphere->radius();
					radius *= sqrt(2.0f); //since they are diagonal
					verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
					vconstr->push(c);
				}
			}

			//BEND SPRINGS, diagonal/horizontal/vertical by 2
			//horizontal + vertical
			if (j<(dimy-2)) { //i.e. not on rightmost edge
				//connect particles left-right
				p2 = particles[i][j+2];
				radius = p->sphere->radius() + p2->sphere->radius();
				radius *= 2.0f;
				verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
				vconstr->push(c);
			}
			if (i < dimx-2) {
				//connect particles up-down
				p2 = particles[i+2][j];
				radius = p->sphere->radius() + p2->sphere->radius();
				radius *= 2.0f;
				verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
				vconstr->push(c);
			}
			//diagonal
			if (j<(dimy-2)) {//i.e. not rightmost edge
				if (i > 1) {//i.e. not topmost
					p2 = particles[i-2][j+2];
					radius = p->sphere->radius() + p2->sphere->radius();
					radius *= sqrt(2.0f); //since they are diagonal
					radius *= 2.0f;
					verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
					vconstr->push(c);
				}
				if (i<(dimx-2)) {//i.e. not bottom-most
					p2 = particles[i+2][j+2];
					radius = p->sphere->radius() + p2->sphere->radius();
					radius *= sqrt(2.0f); //since they are diagonal
					radius *= 2.0f;
					verletConstraintFixedDistance *c = new verletConstraintFixedDistance(p, p2, radius, str);
					vconstr->push(c);
				}
			}
		}
	}

}
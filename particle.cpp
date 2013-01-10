#include "particle.h"
#include <gsim/sn_group.h>
#include <gsim/sn_sphere.h>
#include <gsim/sn_cylinder.h>
#include <gsim/sn_transform.h>
#include <gsim/gs_color.h>


particle::particle(void)
{
	velocity = new GsVec(0.0,0.0,0.0);
	position = new GsVec(0.0,0.0,0.0);
	prevPos = new GsVec(0.0,0.0,0.0);
	gravity = new GsVec(0.0, -9.8,0.0);
	extforce = new GsVec(0.0,0.0,0.0);
	restorationFactor = 0.8;

	sng = new SnGroup;
	transform = new SnTransform;
	sphere = new SnSphere;
	sng->add(transform);
	sng->add(sphere);
	sng->separator(true);
	fixed = false;

	sphere->color(GsColor::random(5));
	sphere->radius(0.03f);
	sphere->center(GsPnt(0,0,0));
	transform->get().translation(0.0f,0.0f,0.0f);
}


particle::~particle(void)
{
	delete velocity;
	delete position;
	delete prevPos;
	delete gravity;
	delete extforce;
	sng->unref();
}

void particle::updateVerlet() {
	GsVec forceTotal(0.0, 0.0, 0.0);
	float dt = 1.0/60.0;

	//compute forces
	forceTotal += (*gravity);
	forceTotal += (*extforce);

	if (!fixed) {
		GsVec temp = GsVec(*position);
		(*position) += ((*position) - (*prevPos)) * .99 + forceTotal * (dt * dt);
		(*prevPos) = temp;
	} else {
		//
	}

	transform->get().translation(*position);

	//check for collision with walls
	float box = 2.0f;
	if (position->y < -box) {
		position->y = -box;
	}
	
	if (position->y > box) {
		position->y = box;
	}

	if (position->x < -box) {
		position->x = -box;
	}

	if (position->x > box) {
		position->x = box;
	}

	if (position->z < -box) {
		position->z = -box;
	}

	if (position->z > box) {
		position->z = box;
	}

	(*extforce) *= 0.0;
}

void particle::update(float dt) {
	GsVec forceTotal(0.0, 0.0, 0.0);

	//compute forces
	forceTotal += (*gravity);
	forceTotal += (*extforce);

	/*if (verlet) {
		//verlet step
		if (!fixed) {
			GsVec temp = GsVec(*position);
			(*position) = 1.5 * (*position) - (*prevPos) + forceTotal * (dt * dt);
			(*prevPos) = temp;
		}
	} else {*/
		//euler step
	if (!fixed) {
		(*position) += dt * (*velocity) + (0.5 * forceTotal * dt * dt); //i.e. p = vt + 0.5at^2, two terms of tailor expansion
	
		//update velocity
		(*velocity) += dt * forceTotal;
		(*velocity) -= (*velocity) *  (0.8 * dt);
	} else {
		(*velocity) *= 0;
	}


	transform->get().translation(*position);
	


	

	
	//check for collision with walls
	if (position->y < -2 && velocity->y < 0) {
		velocity->y *= -restorationFactor;
		velocity->y += -2 - position->y;
	}
	
	if (position->y > 2 && velocity->y > 0) {
		velocity->y *= -restorationFactor;
	}

	if (position->x < -2 && velocity->x < 0) {
		velocity->x *= -restorationFactor;
	}

	if (position->x > 2 && velocity->x > 0) {
		velocity->x *= -restorationFactor;
	}

	if (position->z < -2 && velocity->z < 0) {
		velocity->z *= -restorationFactor;
	}

	if (position->z > 2 && velocity->z > 0) {
		velocity->z *= -restorationFactor;
	}

	(*extforce) *= 0.0;

}

void particle::applyForce(GsVec f) {
	(*extforce) += f;
}
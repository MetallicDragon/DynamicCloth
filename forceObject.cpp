#include "forceObject.h"


forceObject::forceObject(void)
{
	force.set(0.0f,0.0f,0.0f);
}


forceObject::~forceObject(void)
{
}

void forceObject::applyForce() {
	p->applyForce(force);
}

# pragma once

#include "app_viewer.h"
#include "particle.h"
#include "forceObject.h"
#include "verletConstraint.h"
#include "cloth.h"
#include "partmesh.h"
#include <gsim/sn_manipulator.h>

// In this simple example, put all application data
// to be shared by different components here:
class AppData
 { public :
    fltk::Window* window;
    AppViewer* viewer;
    double timestart;
	GsArrayPt<particle*> * particles;
	GsArrayPt<forceObject*> * forces;
	GsArrayPt<verletConstraint*> * vconstr;
	double lastFrameTime;
	bool wind, verlet, partVis;
	Cloth* c1;
	Partmesh* c1mesh;
	SnGroup* g;
	particle* bigP;
	SnManipulator* manip;

 };

// There will be only a single instance of AppData:
extern AppData* App;


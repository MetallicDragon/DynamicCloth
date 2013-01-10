# include <stdlib.h>

# include <gsim/fl.h>
# include <gsim/sn_group.h>
# include <gsim/sn_sphere.h>
# include <gsim/sn_cylinder.h>
# include <gsim/sn_transform.h>
#include <gsim/sn_manipulator.h>

# include <fltk/Item.h>
# include <fltk/Button.h>

# include <fltk/Window.h>
# include <fltk/run.h>

#include "particle.h"
#include "cloth.h"
#include "app_main.h"
#include "partmesh.h"
#include <math.h>

// The single instance of AppData is declared here:
AppData* App;

// Add events generated by the user interface here:
enum AppEvent { evQuit, evTime, evEtc, evWind, evPartVis, evDemo1, evDemo2, evDemo3};

void free_app_stuff() {
	
	if (App->c1mesh != NULL ) App->c1mesh->unref();
	App->c1mesh = NULL;
	if (App->c1 != NULL) delete App->c1;
	App->c1 = NULL;


}


// Process events here:
void process_event ( AppEvent e )
{
	switch ( e )
	{ case evQuit: exit(0); break;

	case evTime:
		{
			GsString s;
			s << "Time: " << (gs_time() - App->timestart);
			App->viewer->message(s);
		} break;
	case evWind:
		{
			GsString s;
			s << "Wind toggled ";
			if (App->wind)
				s << "off.";
			else s << "on.";
			App->viewer->message(s);
			App->wind = !App->wind;
		} break;
	case evPartVis:
		{
			App->partVis = !App->partVis;
			for (int i = 0; i < App->c1->dimx; i++) {
				for (int j = 0; j < App->c1->dimy; j++) {
					App->c1->particles[i][j]->sphere->visible(App->partVis);
				}
			}
		} break;

	case evDemo1:

		//free_app_stuff();
		{
			for (int i = 0; i < App->particles->size(); i++) {
				for (int j = i + 1; j < App->particles->size(); j++) {
					verletConstraintMinDistance *c = new verletConstraintMinDistance(App->particles->get(i), App->particles->get(j), App->particles->get(i)->sphere->radius() + App->particles->get(j)->sphere->radius());
					App->vconstr->push(c);
				}
			}


			App->c1 = new Cloth(App->particles, App->vconstr, App->g, GsVec(-1.0f,0.0,0.0),25,25, true);
			for (int i = 0; i < App->c1->dimx; i++) {
				//App->c1->particles[0][i]->fixed = true;
			}
			App->c1mesh = new Partmesh(App->c1);
			App->g->add(App->c1mesh);

			App->bigP = new particle();
			App->bigP->sphere->radius(0.25f);
			App->bigP->fixed = true;
			App->bigP->position->set(0.0f,0.0f,0.0f);
			App->manip = new SnManipulator();
			App->manip->child(App->bigP->sphere);
			App->g->add(App->manip);
			App->particles->push(App->bigP);
			for (int i = 0; i < App->c1->dimx; i++) {
				for (int j = 0; j < App->c1->dimy; j++) {
					verletConstraintMinDistance *c = new verletConstraintMinDistance(App->bigP, App->c1->particles[i][j], App->bigP->sphere->radius() + App->c1->particles[i][j]->sphere->radius());
					App->vconstr->push(c);
				}
			}

		} break;

	case evDemo2:
		{

		} break;

	case evDemo3:
		{

		} break;
	}
}

// here we convert fltk callback system to our event function
static void event_cb ( fltk::Item* o, void* udata )
{
	process_event ( (AppEvent)(int)udata );
}


void init_app ()
{
	// Create the single AppData instance:
	App = new AppData;


	// Save current time:
	App->timestart = gs_time(); // current time in seconds
	App->lastFrameTime = gs_time();

	// Set Misc App variables
	App->wind = false;
	App->verlet = true;
	App->partVis = false;
	App->vconstr = new GsArrayPt<verletConstraint*>;
	App->forces = new GsArrayPt<forceObject*>;
	App->particles = new GsArrayPt<particle*>;
	App->c1mesh = NULL;
	App->c1 = NULL;

	// Create main window:
	App->window = new fltk::Window ( 800, 600, "My First GSim Application");
	App->window->resizable(App->window);

	// Add widgets to the window:
	App->window->begin();

	// Create the gsim opengl viewer inside the window:
	App->viewer = new AppViewer ( 90, 0, 750, 600 );

	// Add some buttons, for example:
	fltk::Button* quitbut = new fltk::Button(5,50,40,20,"Quit");
	quitbut->callback((fltk::Callback*)event_cb,(void*)evQuit);

	fltk::Button* printbut = new fltk::Button(5,80,40,20,"Time");
	printbut->callback((fltk::Callback*)event_cb,(void*)evTime);

	fltk::Button* windbut = new fltk::Button(5,110,80,20,"Wind On/Off");
	windbut->callback((fltk::Callback*)event_cb,(void*)evWind);

	fltk::Button* partvisbut = new fltk::Button(5,140,80,60,"Toggle Cloth\nParticles");
	partvisbut->callback((fltk::Callback*)event_cb,(void*)evPartVis);

	/*fltk::Button* demo1but = new fltk::Button(5,210,60,20,"Demo 1");
	demo1but->callback((fltk::Callback*)event_cb,(void*)evDemo1);

	fltk::Button* demo2but = new fltk::Button(5,240,60,20,"Demo 2");
	demo2but->callback((fltk::Callback*)event_cb,(void*)evDemo2);

	fltk::Button* demo3but = new fltk::Button(5,270,60,20,"Demo 3");
	demo3but->callback((fltk::Callback*)event_cb,(void*)evDemo3);*/

	// Done building our window:
	App->window->end();
}

int main ( int argc, char** argv )
{
	// Init application:
	init_app();

	// If needed, use gsout to send messages to the console:
	// gsout<<"Showing window...\n";

	// As example, add two objects to the scenegraph:
	SnGroup* g = new SnGroup;  // a group node can contain many nodes
	App->g = g;

	for (int i = 0; i < 10; i++) {

	}

	/*SnCylinder* c = new SnCylinder; // a cylinder object
	c->color ( GsColor::red );
	g->add ( c );
	*/

	SnSphere* s = new SnSphere; // a sphere object
	s->radius ( 0.0f );
	s->center ( GsPnt(1,0,0) );
	s->color(GsColor::blue);
	g->add ( s );


	/*for (int i = 0; i < 200; i++) {
	particle* p = new particle();
	g->add(p->sng);
	p->position->set(rand()/(float)RAND_MAX * 3 - 1.5, rand()/(float)RAND_MAX * 3 - 1.5, rand()/(float)RAND_MAX * 3 - 1.5);
	//p.position->set(0.0f, rand()/(float)RAND_MAX * 4 - 2, 0.0f);
	//p.position->set(0.05f * i, 1.0f + 0.001 * i, 0.0f);
	p->prevPos->set(GsVec(*(p->position)));
	App->particles->push(p);
	}*/
		int numparts = 25;
		for (int i = 0; i < numparts; i++) {
			particle* p = new particle();
			g->add(p->sng);
			p->position->set(rand()/(float)RAND_MAX * 1.0f - 0.5f, -0.32f, rand()/(float)RAND_MAX * 1.0f - 0.5f);
			p->sphere->radius(0.06f);
			//p.position->set(0.0f, rand()/(float)RAND_MAX * 4 - 2, 0.0f);
			//p.position->set(0.05f * i, 1.0f + 0.001 * i, 0.0f);
			p->prevPos->set(GsVec(*(p->position)));
			App->particles->push(p);
		}

		for (int i = 0; i < App->particles->size(); i++) {
			for (int j = i + 1; j < App->particles->size(); j++) {
				verletConstraintMinDistance *c = new verletConstraintMinDistance(App->particles->get(i), App->particles->get(j), App->particles->get(i)->sphere->radius() + App->particles->get(j)->sphere->radius());
				App->vconstr->push(c);
			}
		}


		App->c1 = new Cloth(App->particles, App->vconstr, App->g, GsVec(-1.2f,-0.5f,-1.2f),40,40, true);
		for (int i = 0; i < App->c1->dimx; i++) {
			for (int j = 0; j < App->c1->dimy; j++) {
				App->c1->particles[i][j]->sphere->visible(App->partVis);
				for (int k = 0; k < numparts; k++) {
					verletConstraintMinDistance *c = new verletConstraintMinDistance(App->particles->get(k), App->c1->particles[i][j], App->particles->get(k)->sphere->radius() + App->c1->particles[i][j]->sphere->radius());
					App->vconstr->push(c);
				}
			}
		}
		for (int i = 0; i < App->c1->dimx; i++) {
			if (i%39 == 0) {	
				App->c1->particles[App->c1->dimx-1][i]->fixed = true;
				App->c1->particles[i][App->c1->dimy-1]->fixed = true;
				App->c1->particles[0][i]->fixed = true;
				App->c1->particles[i][0]->fixed = true;
			}
		}
		App->c1mesh = new Partmesh(App->c1);
		App->g->add(App->c1mesh);

		App->bigP = new particle();
		App->bigP->sphere->radius(0.25f);
		App->bigP->fixed = true;
		App->bigP->position->set(0.0f,0.0f,0.0f);
		App->manip = new SnManipulator();
		App->manip->child(App->bigP->sphere);
		App->g->add(App->manip);
		App->particles->push(App->bigP);
		for (int i = 0; i < App->c1->dimx; i++) {
			for (int j = 0; j < App->c1->dimy; j++) {
				verletConstraintMinDistance *c = new verletConstraintMinDistance(App->bigP, App->c1->particles[i][j], App->bigP->sphere->radius() + App->c1->particles[i][j]->sphere->radius());
				App->vconstr->push(c);
			}
		}









	App->viewer->root ( g ); // set the group node to be the root of the scene

	// A few viewer configurations:
	App->viewer->cmd ( FlViewer::CmdAxis );
	App->viewer->view_all();


	// Show the window:
	App->window->show();

	// There are two possible ways to control the main loop:
	// (1) Let fltk handle the loop:
	//fltk::run();

	// (2) control yourself the loop:
	App->viewer->view_all();
	while ( true )
	{ // process user interface events
		fltk::check();

		// compute time step since last frame
		double dt = gs_time() - App->lastFrameTime; 
		if (dt > (1.0/60)) {
			// update scripts, AI, physics, etc

			// update positions, network, etc


			if (true /*!App->verlet*/) {
				//apply euler forces
				int s =  App->forces->size();
				for (int i = 0; i < App->forces->size(); i++) {
					App->forces->get(i)->applyForce();
				}
			}
			for (int i = 0; i < App->particles->size(); i++) {
				if (App->wind) {
					App->particles->get(i)->applyForce(GsVec(5.0, 0.0, -2.5));
				}
				if (App->verlet) {
					if (App->bigP != NULL) App->bigP->position = new GsVec(App->manip->translation());
					App->particles->get(i)->updateVerlet();
				} else {
					App->particles->get(i)->update(dt);
				}
			}

			if (App->verlet) {
				//apply verlet constraints
				for (int j = 0; j < 3; j++) {
					int s =  App->vconstr->size();
					for (int i = 0; i < s; i++) {
						App->vconstr->get(i)->applyConstraint();
					}
				}
			}
			// draw current frame
			if (App-> c1mesh != NULL) App->c1mesh->changed(true);
			App->viewer->render();
			App->lastFrameTime = gs_time();
		}
	}

	return 0;
}

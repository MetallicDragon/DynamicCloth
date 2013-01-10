/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include "partmesh.h"
# include <gsim/gs_array.h>
# include <gsim/gs_ogl.h>
# include <gsim/gs_box.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_quat.h>

//# define GS_USE_TRACE1 // Render
# include <gsim/gs_trace.h>

//================================== Partmesh ====================================

const char* Partmesh::class_name = "Partmesh";

Partmesh::Partmesh () : SnShape(class_name), a(GsVec::null), b(GsVec::i)
 {
   radiusa = radiusb = 0.1f;
 }

Partmesh::Partmesh ( const Partmesh& c ) : SnShape(class_name), a(c.a), b(c.b)
 {
   radiusa = c.radiusa;
   radiusb = c.radiusb;
 }

Partmesh::Partmesh (Cloth* cl) : SnShape(class_name) {
	cloth = cl;
}

void Partmesh::get_bounding_box ( GsBox& box ) const
 { 
   GsVec va = b-a; 
   va.normalize();
   GsVec vr1;
   if ( angle(GsVec::i,va)<0.1f )
     vr1 = cross ( GsVec::j, va );
   else
     vr1 = cross ( GsVec::i, va );
   
   GsVec vr2 = cross ( vr1, va );

   float radius = radiusa > radiusb ? radiusa : radiusb;

   vr1.len ( radius );
   vr2.len ( radius );

   box.set_empty();
   box.extend ( a+vr1 );
   box.extend ( a-vr1 );
   box.extend ( a+vr2 );
   box.extend ( a-vr2 );
   box.extend ( b+vr1 );
   box.extend ( b-vr1 );
   box.extend ( b+vr2 );
   box.extend ( b-vr2 );
 }

void Partmesh::gl_render_node () const
 {
   # ifdef GS_OPENGL
   GS_TRACE1 ( "Render particle mesh" );

   gsRenderMode rm = render_mode();
   glEnable ( GL_CULL_FACE );
   glEnable ( GL_LIGHTING );

   glMaterial ( material() );

   switch ( rm )
    { case gsRenderModeFlat :
           glShadeModel ( GL_FLAT );
           glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
           break;
      case gsRenderModeLines :
           glShadeModel ( GL_SMOOTH );
           glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
           break;
      case gsRenderModePoints :
           glShadeModel ( GL_SMOOTH );
           break;
      default:
           glShadeModel ( GL_SMOOTH );
           glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
           break;
    }

   	int x,y;
	x = cloth->dimx;
	y = cloth->dimy;
	/*
	like so:
	  _________________
	v1|  /|v3/|v5/|v7
	  | / | / | / |
	v2|/  |v4 |v6 |v8  etc
	*/
	//first, generate normals
	GsVec** normals = new GsVec*[x];
	for (int i = 0; i < x; i++) {
		normals[i] = new GsVec[y];
	}
	for (int i = 0; i < x; i++) {
		GsVec p1,p2,p3,p4,q1,q2,q3;
		for (int j = 0; j < y; j++) {
			int u,v;
			if (i > 0) {
				p1 = *(cloth->particles[i-1][j]->position);
			} else {
				p1 = *(cloth->particles[i][j]->position);
			}

			if (i < x-1) {
				p2 = *(cloth->particles[i+1][j]->position);
			} else {
				p2 = *(cloth->particles[i][j]->position);
			}
			
			if (j > 0) {
				p3 = *(cloth->particles[i][j-1]->position);
			} else {
				p3 = *(cloth->particles[i][j]->position);
			}
			if (j < y-1) {
				p4 = *(cloth->particles[i][j+1]->position);
			} else {
				p4 = *(cloth->particles[i][j]->position);
			}
			
			if (i < x-1) {
				u = 0;
			} else {
				u = -1;
			}
			if (j < y-1) {
				v = 0;
			} else {
				v = -1;
			}
			
			q1 = *(cloth->particles[i+u][j+v]->position);
			q2 = *(cloth->particles[i+u+1][j+v]->position);
			q3 = *(cloth->particles[i+u][j+v+1]->position);
			
			GsVec n,n2;
			n.cross(p2-p1,p4-p3);
			n2.cross(q2-q1,q3-q1);
			n = (n+n2)/(n.len() + n2.len());
			n.normalize();
			normals[i][j] = n;
		}
	}

	float r = 0.001f;//cloth->particles[0][0]->sphere->radius();
	int factor = 1;
	for (int times = 0; times < 2; times++) {
		if (times == 1) {
			factor = -1;
			glFrontFace(GL_CW);
		}
		for (int i = 0; i < x-1; i++) {
			glBegin(GL_TRIANGLE_STRIP);
			GsVec p1,p2,n;
		
			p1 = *(cloth->particles[i][0]->position);
			p2 = *(cloth->particles[i+1][0]->position);

			n = factor * normals[i][0];
			glNormal3f(n.x,n.y,n.z);
			p1 += r * n;
			glVertex3f(p1.x,p1.y,p1.z);

			n = factor * normals[i+1][0];
			glNormal3f(n.x,n.y,n.z);
			p2 += r * n;
			glVertex3f(p2.x,p2.y,p2.z);
			for (int j = 1; j < y; j++) {
				p1 = *(cloth->particles[i][j]->position);
				p2 = *(cloth->particles[i+1][j]->position);

				n = factor * normals[i][j];
				glNormal3f(n.x,n.y,n.z);
				p1 += r * n;
				glVertex3f(p1.x,p1.y,p1.z);

				n = factor * normals[i+1][j];
				glNormal3f(n.x,n.y,n.z);
				p2 += r * n;
				glVertex3f(p2.x,p2.y,p2.z);
			}
			glEnd();
		}
		
	}
	glFrontFace(GL_CCW);

   /*int nfaces = int(resolution()*10.0f);
   if ( nfaces<3 ) nfaces = 3;
   
   float dang = gs2pi/float(nfaces);
   GsVec va = b-a; 
   va.normalize(); // axial vector
   GsVec minus_va = va * -1.0f;

   GsVec vra, vrb;
   float deg = GS_TODEG ( angle(GsVec::i,va) );

   if ( deg<10 || deg>170 )
     vra = cross ( GsVec::j, va );
   else
     vra = cross ( GsVec::i, va );
   
   vrb = vra;
   vra.len ( radiusa ); // radial vector
   vrb.len ( radiusb ); // radial vector

   GsQuat q ( va, dang );
   GsQuat q2 ( va, dang * 0.5f );
   GsQuat q1 = q2.inverse();
   q1.normalize();

   GsPnt a1 = a+vra;
   GsPnt b1 = b+vrb;
   GsPnt a2 = q.apply(a1);
   GsPnt b2 = q.apply(b1);

   if ( rm==gsRenderModePoints )
     glBegin ( GL_POINTS );
   else
     glBegin ( GL_QUADS );

   int i=1;
   GsArray<GsPnt> vlist(0,2*(nfaces+1));
   GsVec n1, n2, nf; // normals
   do { nf = cross(b1-a1,a1-a2);
        nf.normalize(); // normalize
        n1 = q1.apply(nf);
        n2 = q2.apply(nf);
        vlist.push()=a1; vlist.push()=b1;
        glNormal (n1); glVertex (b1,a1);
        glNormal (n2); glVertex (a2,b2);
        if ( i==nfaces ) break;
        a1=a2; b1=b2; a2=q.apply(a1); b2=q.apply(b1);
        i++;
      } while ( true );

   glEnd ();

   if ( rm!=gsRenderModePoints )
    { glBegin ( GL_POLYGON );
      glNormal ( minus_va );
      for ( i=vlist.size()-2; i>=0; i-=2 ) glVertex ( vlist[i] );
      glEnd ();
      glNormal ( va );
      glBegin ( GL_POLYGON );
      for ( i=1; i<vlist.size(); i+=2 ) glVertex ( vlist[i] );
      glEnd ();
    }*/
   # endif // GS_OPENGL
 }

GsOutput& operator<< ( GsOutput& o, const Partmesh& c )
 {
   return o << c.a << gspc << c.b << gspc << c.radiusa << gspc << c.radiusb;
 }

GsInput& operator>> ( GsInput& in, Partmesh& c )
 {
   return in >> c.a >> c.b >> c.radiusa >> c.radiusb;
 }

//================================ EOF =================================================

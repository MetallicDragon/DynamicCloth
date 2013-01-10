/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

#pragma once

/** particle mesh
 */

# include <gsim/sn_shape.h>
# include "cloth.h"

/*! \class Partmesh partmesh.h

    Partmesh represents a mesh of particles based on each particle's position. 

	*/
class Partmesh : public SnShape
 { public :
    GsPnt a, b; //!< do not forget to call SnShape::touch() after members are changed!
    float radiusa, radiusb;
    static const char* class_name; //!< constain the static string "Partmesh"
	Cloth* cloth;

   public :

    /*! Constructs a cylinder with endpoints (0,0,0) and (1,0,0) and radius 1 */
    Partmesh ();
	Partmesh (Cloth* cl);

    /*! Copy constructor */
    Partmesh ( const Partmesh& c );

    /*! Set radiusa and radiusb to the given value */
    void radius ( float r ) { radiusa=radiusb=r; touch(); }

   public :

    /*! Makes OpenGL calls to draw the mesh.
        The resolution value stored in SnShape
        represents 1/10th of the number of edges discretizing the base
        circle of the cylinder. For ex., resolution 1.0 results in
        10 edges, and gives 40 triangles to render the cylinder. */
    virtual void gl_render_node () const;

    /*! Returns the bounding box of all vertices used. The returned box can be empty. */
    virtual void get_bounding_box ( GsBox &b ) const;

    /*! Outputs in format "p1 p2 radius " */
    friend GsOutput& operator<< ( GsOutput& o, const Partmesh& c );

    /*! Input from format "p1 p2 radius " */
    friend GsInput& operator>> ( GsInput& in, Partmesh& c );
 };

//================================ End of File =================================================



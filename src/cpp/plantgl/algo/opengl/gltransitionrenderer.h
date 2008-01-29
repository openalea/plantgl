/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */



/*! \file actn_gltransitionrenderer.h
    \brief Definition of the action class GLTransitionRenderer.
*/


#ifndef __actn_gltransitionrenderer_h__
#define __actn_gltransitionrenderer_h__


#include "glrenderer.h"

/* ----------------------------------------------------------------------- */

PGL_BEGIN_NAMESPACE

/* ----------------------------------------------------------------------- */

class Scene;
typedef RCPtr<Scene> ScenePtr;

/* ----------------------------------------------------------------------- */

/**   
   \class GLTransitionRenderer
   \brief An action which draws a visual transition between 2 scenes
   using blending.
*/


class ALGO_API GLTransitionRenderer : public GLRenderer
{

public:

  /** Constructs a GLRenderer with the Discretizer \e discretizer and
      the normal display type \t. */
  GLTransitionRenderer( Discretizer& discretizer, 
						QGLWidget * widget = NULL, 
						uint_t step = 10);

  /// Destructor
  virtual ~GLTransitionRenderer( );

  /// Clears \e self.
  void clear( );

  /// Return the total step that renderer do.
  const uint_t getTotalStep() const;

  /// Set the total step that renderer do.
  void setTotalStep(const uint_t);

  /// Set the 2 scene \e scene1 and \e scene2 to display.
  void setScene(ScenePtr scene1, ScenePtr scene2);
  

  /// @name Pre and Post Processing
  //@{
  virtual bool endProcess();

  //@}
     
  /// @name Material
  //@{

  virtual bool process( Material * material );

  virtual bool process( MonoSpectral * monoSpectral );

  virtual bool process( MultiSpectral * multiSpectral );

  //@}
  
  /// make a render of the 2 scene at the step \e step.
  virtual bool rend(uint_t step);

protected:

  

  /// Number of total step
  uint_t __totalstep;

  /// Last step
  uint_t __laststep;

  /// Value of transparency
  GLfloat __transparency;

  /// The first scene.
  ScenePtr __scene1;

  /// The second scene.
  ScenePtr __scene2;

};
 
typedef RCPtr<GLTransitionRenderer> GLTransitionRendererPtr; 


/* ----------------------------------------------------------------------- */

PGL_END_NAMESPACE

/* ----------------------------------------------------------------------- */
  
// __actn_glsimplerenderer_h__
#endif                                             


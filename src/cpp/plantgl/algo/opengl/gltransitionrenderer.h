/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright CIRAD/INRIA/INRA
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr) et al. 
 *
 *  ----------------------------------------------------------------------------
 *
 *   This software is governed by the CeCILL-C license under French law and
 *   abiding by the rules of distribution of free software.  You can  use, 
 *   modify and/ or redistribute the software under the terms of the CeCILL-C
 *   license as circulated by CEA, CNRS and INRIA at the following URL
 *   "http://www.cecill.info". 
 *
 *   As a counterpart to the access to the source code and  rights to copy,
 *   modify and redistribute granted by the license, users are provided only
 *   with a limited warranty  and the software's author,  the holder of the
 *   economic rights,  and the successive licensors  have only  limited
 *   liability. 
 *       
 *   In this respect, the user's attention is drawn to the risks associated
 *   with loading,  using,  modifying and/or developing or reproducing the
 *   software by the user in light of its specific status of free software,
 *   that may mean  that it is complicated to manipulate,  and  that  also
 *   therefore means  that it is reserved for developers  and  experienced
 *   professionals having in-depth computer knowledge. Users are therefore
 *   encouraged to load and test the software's suitability as regards their
 *   requirements in conditions enabling the security of their systems and/or 
 *   data to be ensured and,  more generally, to use and operate it in the 
 *   same conditions as regards security. 
 *
 *   The fact that you are presently reading this means that you have had
 *   knowledge of the CeCILL-C license and that you accept its terms.
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
#ifndef PGL_CORE_WITHOUT_QT
                        QOpenGLWidget * widget = NULL,
#endif
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


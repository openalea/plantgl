/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: Modeling Plant Geometry
 *
 *       Copyright 2000-2006 - Cirad/Inria/Inra - Virtual Plant Team
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
 *
 *       Development site : https://gforge.inria.fr/projects/openalea/
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


#include <plantgl/algo/opengl/util_glu.h>
#include <plantgl/tool/util_assert.h>
#include "zbuffer.h"

PGL_USING_NAMESPACE

void ViewRayBuffer::setAt(size_t i, size_t j, void * buffer, size_t size,const Vector3& position) {
    RayHitList& res = getAt(i,j) ;
    GLuint names, *ptr;
    ptr = (GLuint *) buffer;
    GLuint id;
    GLuint zmin;
    GLuint zmax;
    GLdouble zmin2;
    GLdouble zmax2;
    GLint viewport[4];
    GLdouble modelMatrix[16], projMatrix[16];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    GLdouble winx = viewport[2]/2;
    GLdouble winy = viewport[3]/2;
    GLdouble objx,objx2, objy,objy2, objzmin , objzmax;
;

    if(size > 0){
        for(size_t i = 0 ; i < size ; i++){
            names = *ptr;
            ptr++;
            zmin = (float)*ptr; zmin2 = (GLdouble)zmin /(GLdouble)ULONG_MAX ; ptr++;
            zmax = (float)*ptr; zmax2 = (GLdouble)zmax /(GLdouble)ULONG_MAX ;ptr++;
            id = *ptr;
            if( geomUnProject(winx,winy, zmin2, modelMatrix, projMatrix, viewport,
                &objx,&objy, &objzmin)  &&
                geomUnProject(winx,winy, zmax2, modelMatrix, projMatrix, viewport,
                &objx2,&objy2, &objzmax) ){

                res.push_back(RayHit(id,norm(Vector3(objx,objy,objzmin)-position),norm(Vector3(objx2,objy2,objzmax)-position)));
            }
            for(unsigned int j = 0 ; j < names ; j++)ptr++;
        }
    }
}

ViewRayPointHitBuffer& ViewRayPointHitBuffer::operator+=(const ViewRayPointHitBuffer& buff)
{
  //arrays must have identical size
  int w = getRowSize();
  int h = getColumnSize();
  assert(buff.getRowSize() == w && buff.getColumnSize() == h && "Size of self and buff must be identical.");
  for(int r=0; r<h; ++r)
  {
      for(int c=0; c<w; ++c)
      {
        const RayPointHitList& hitList = buff.getAt(r,c);
        if(!hitList.empty())
        {
          RayPointHitList& myhitList = getAt(r,c);
          myhitList.insert(myhitList.end(),hitList.begin(),hitList.end());
        }
      }
  }
  return *this;
}

ViewRayPointHitBuffer ViewRayPointHitBuffer::operator+(const ViewRayPointHitBuffer& buff)const
{//arrays must have identical size
  ViewRayPointHitBuffer res (*this);
  res += buff;
  return res;
}


ViewZBuffer* ViewZBuffer::importglDepthBuffer(bool alldepth)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    int width = viewport[2];
    int height = viewport[3];
    ViewZBuffer * buffer = new ViewZBuffer(height,width);


    float  * zvalues = new float[width*height];
    // std::cerr << "Read Depth Buffer ... ";
    // glReadBuffer(GL_FRONT);
    glReadPixels(0,0,width,height,GL_DEPTH_COMPONENT, GL_FLOAT, zvalues);
    // std::cerr << "done." << std::endl;
    GLdouble modelMatrix[16], projMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    GLdouble objx, objy, objz;
    float  * iterzvalues = zvalues;
    // std::cerr << "Unproject Depth Buffer ... ";
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            buffer->getAt(i,j).depth = *iterzvalues ;
            if( alldepth ||( 0 < *iterzvalues && *iterzvalues < 1) ){
                if( geomUnProject(j,i, (GLdouble)*iterzvalues, modelMatrix, projMatrix, viewport, &objx,&objy, &objz) ){
                    buffer->getAt(i,j).pos = Vector3(objx,objy,objz);
                }
            }
            ++iterzvalues;
        }
    }
    // std::cerr << "done." << std::endl;
    delete [] zvalues;
    return buffer;
}

ViewZBuffer* ViewZBuffer::importglZBuffer(bool alldepth, bool invertalpha)
{
    ViewZBuffer * buffer = importglDepthBuffer(alldepth);
    int width = buffer->getRowSize();
    int height = buffer->getColumnSize();
    uchar  * colvalues = new uchar[4*width*height];
    // std::cerr << "Read Color Buffer ... ";
    glReadPixels(0,0,width,height,GL_RGBA, GL_UNSIGNED_BYTE, colvalues);
    // std::cerr << "done." << std::endl;
    uchar  * itercolvalues = colvalues;
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            buffer->getAt(i,j).color = Color4(*itercolvalues,itercolvalues[1],
                                               itercolvalues[2],invertalpha?255-itercolvalues[3]:itercolvalues[3]);
            itercolvalues+=4;
        }
    }
    delete [] colvalues;

    return buffer;
}

std::pair<Point3ArrayPtr,Color4ArrayPtr>
ViewZBuffer::importglZBufferPoints(bool invertalpha) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    int width = viewport[2];
    int height = viewport[3];

    float  * zvalues = new float[width*height];
    uchar  * colvalues = new uchar[4*width*height];

    glReadPixels(0,0,width,height,GL_DEPTH_COMPONENT, GL_FLOAT, zvalues);
    glReadPixels(0,0,width,height,GL_RGBA, GL_UNSIGNED_BYTE, colvalues);

    GLdouble modelMatrix[16], projMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    GLdouble objx, objy, objz;

    Point3ArrayPtr points(new Point3Array());
    Color4ArrayPtr colors(new Color4Array());

    float  * iterzvalues = zvalues;
    uchar  * itercolvalues = colvalues;
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            if ( 0 < *iterzvalues && *iterzvalues < 1) {
                if( geomUnProject(j,i, (GLdouble)*iterzvalues, modelMatrix, projMatrix, viewport, &objx,&objy, &objz) ){
                    points->push_back(Vector3(objx,objy,objz));
                    colors->push_back(Color4(*itercolvalues,itercolvalues[1],
                                               itercolvalues[2],invertalpha?255-itercolvalues[3]:itercolvalues[3]));
                }
            }
            ++iterzvalues;
            itercolvalues+=4;
        }
    }
    // std::cerr << "done." << std::endl;
    delete [] zvalues;
    delete [] colvalues;
    return std::pair<Point3ArrayPtr,Color4ArrayPtr> (points, colors);
}

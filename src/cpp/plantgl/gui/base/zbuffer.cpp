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
    delete [] zvalues;
    delete [] colvalues;
    return std::pair<Point3ArrayPtr,Color4ArrayPtr> (points, colors);
}

std::pair<Point3ArrayPtr,Color4ArrayPtr>
ViewZBuffer::importglZBufferPointsWithJitter(real_t jitter, int raywidth, bool invertalpha, bool mixcolor) {
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


    std::vector<Vector3 *> pointmatrix(width*height,NULL);
    std::vector<Color4 *> colormatrix(width*height,NULL);

    GLdouble originx, originy, originz;
    geomUnProject(0,0,0, modelMatrix, projMatrix, viewport, &originx,&originy, &originz);
    Vector3 origin;
    //printf("%f %f %f\n", originx, originy, originz);
    float  * iterzvalues = zvalues;
    uchar  * itercolvalues = colvalues;
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            if ( 0 < *iterzvalues && *iterzvalues < 1) {
                if( geomUnProject(j,i, (GLdouble)*iterzvalues, modelMatrix, projMatrix, viewport, &objx,&objy, &objz) ){
                    Vector3 pos(objx,objy,objz);
                    pointmatrix[i*width+j] = new Vector3(pos);
                    colormatrix[i*width+j] = new Color4(*itercolvalues,itercolvalues[1],
                                               itercolvalues[2],invertalpha?255-itercolvalues[3]:itercolvalues[3]);
                }
            }
            ++iterzvalues;
            itercolvalues+=4;
        }
    }
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            if (pointmatrix[i*width+j] != NULL) {
                uint_t count = 0;
                Vector3 sumvec;
                int red = 0;
                int green = 0;
                int blue = 0;
                int alpha = 0;
                if (!mixcolor) {
                    Color4& ccol = *colormatrix[i*width+j];
                    red = ccol.getRed(); 
                    green = ccol.getGreen(); 
                    blue = ccol.getBlue(); 
                    alpha = ccol.getAlpha();
                }
                for (int ii = pglMax(0,i-raywidth); ii <= pglMin(height-1,i+raywidth); ++ii){
                    for (int jj = pglMax(0,j-raywidth); jj <= pglMin(width-1,j+raywidth); ++jj){
                        if (pointmatrix[ii*width+jj] != NULL) {
                            sumvec += *pointmatrix[ii*width+jj];
                            Color4& ccol = *colormatrix[ii*width+jj];
                            if (mixcolor) {
                                red += ccol.getRed(); green += ccol.getGreen(); blue += ccol.getBlue(); alpha += ccol.getAlpha();
                            }
                            ++count;
                        }
                    }
                }
                sumvec /= count;
                if (jitter > 0) {
                    Vector3 nml = sumvec-origin;
                    real_t n = norm(nml);
                    if(n > GEOM_EPSILON){
                        nml /= n;
                        sumvec += nml*jitter*(-1+2.*(rand()/double(RAND_MAX)));
                    }
                }
                points->push_back(sumvec);
                if (mixcolor) {
                    colors->push_back(Color4(red/count,green/count,blue/count,alpha/count));
                }
                else {
                    colors->push_back(Color4(red,green,blue,alpha));                    
                }
            }
        }
    }
    // std::cerr << "done." << std::endl;
    delete [] zvalues;
    delete [] colvalues;
    return std::pair<Point3ArrayPtr,Color4ArrayPtr> (points, colors);
}

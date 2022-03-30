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


/*! \file view_reader.h
    \brief Definition of the viewer classes ViewGeomReader, ViewGeomStreamReader, ViewMultiGeomReader and ViewLinetreeReader.
*/

#ifndef __view_reader_h__
#define __view_reader_h__

/* ----------------------------------------------------------------------- */

#include "../gui_config.h"

#include <QtGlobal>
#ifndef QT_THREAD_SUPPORT
#ifdef __GNUC__
#warning "Qt compiled without openGL support"
#else
#pragma message "Qt compiled without openGL support"
#endif
#else

#include <QtCore/qthread.h>
#include <QtCore/qstring.h>
#include <plantgl/scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

class ViewGeomSceneGL;

/* ----------------------------------------------------------------------- */
/**
   \class ViewGeomReader
   \brief Create a new thread to read a geom file

*/

/* ----------------------------------------------------------------------- */

class VIEW_API ViewGeomReader : public QThread
{

    public :


      ViewGeomReader(const QString& f,
                     ViewGeomSceneGL * g,
                     int i = -1,
                         bool add = false);

    ~ViewGeomReader();

    virtual void setFilename(const QString& f);

    const QString& getFilename() const ;

    void setMaxError(int i);

    protected :

      virtual void run();

    /// The file to read.
    QString _filename;

    /// The frame for given result.
    ViewGeomSceneGL * _g;

    /// Nb d'erreur max.
    int maxerror;

        bool addition;
};


/* ----------------------------------------------------------------------- */

/**
   \class ViewGeomStreamReader
   \brief Create a new thread to read a geom stream

*/
/*
class VIEW_API ViewGeomStreamReader : public ViewGeomReader
{

    public :


      ViewGeomStreamReader(std::istream *stream,ViewGeomSceneGL * g,int i = -1) ;

    ~ViewGeomStreamReader();

    virtual void setStream(std::istream * stream);

    protected :

      virtual void run();

    /// The stream to read.
    std::istream * __stream;
};

*/
/* ----------------------------------------------------------------------- */
/**
   \class ViewMultiGeomReader
   \brief Create a new thread to read a geom stream

*/
class VIEW_API ViewMultiGeomReader : public ViewGeomReader
{

    public :


      ViewMultiGeomReader(const QString& f1,
                          const QString& f2,
                          ViewGeomSceneGL * g,
                          int i = -1) ;

    ~ViewMultiGeomReader();

    virtual void setSecond(const QString&);

    const QString& getSecond() const;

    protected :

      virtual void run();

    /// The second file to read.
    QString __second;

};


/* ----------------------------------------------------------------------- */

/**
   \class ViewLinetreeReader
   \brief Create a new thread to read a linetree file

*/
class VIEW_API ViewLinetreeReader : public  ViewGeomReader {
    public :

      ViewLinetreeReader(const QString& ligFile,
                     const QString& dtaFile,
                     const QString& smbPath,
                     ViewGeomSceneGL * g,
                     bool bigendian = true,
                     bool add = false);

    ~ViewLinetreeReader();

    const QString& getLigFile() const;

    void setLigFile(const QString& f);

    const QString& getDtaFile() const;

    void setDtaFile(const QString& f);

    const QString& getSmbPath() const;

    void setSmbPath(const QString& f);


    protected :

      virtual void run();

    QString _dtaFile;

    QString _smbPath;

    bool _bigendian;
};

/* ----------------------------------------------------------------------- */


#endif

#endif


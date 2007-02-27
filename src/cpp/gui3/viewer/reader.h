/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *                           UMR PIAF INRA-UBP Clermont-Ferrand
 *
 *       File author(s): F. Boudon
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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

/*! \file view_reader.h
    \brief Definition of the viewer classes ViewGeomReader3, ViewGeomStreamReader, ViewMultiGeomReader3 and ViewLinetreeReader3.
*/

#ifndef __view_reader_h__
#define __view_reader_h__

/* ----------------------------------------------------------------------- */

#include "../gui3_config.h"

#include "../gui3_config.h"

#ifndef QT_THREAD_SUPPORT
#ifdef __GNUC__
#warning "Qt compiled without openGL support"
#else
#pragma message "Qt compiled without openGL support"
#endif
#else

#include <qthread.h>
#include <qstring.h>
#include <scenegraph/scene/scene.h>

/* ----------------------------------------------------------------------- */

class ViewGeomSceneGL3;

/* ----------------------------------------------------------------------- */
/**
   \class ViewGeomReader3
   \brief Create a new thread to read a geom file

*/

/* ----------------------------------------------------------------------- */

class VIEW3_API ViewGeomReader3 : public QThread
{

    public :


      ViewGeomReader3(const QString& f,
                     ViewGeomSceneGL3 * g,
                     int i = -1,
                         bool add = false);

    ~ViewGeomReader3();

    virtual void setFilename(const QString& f);

    const QString& getFilename() const ;

    void setMaxError(int i);

    protected :

      virtual void run();

    /// The file to read.
    QString _filename;

    /// The frame for given result.
    ViewGeomSceneGL3 * _g;

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
class VIEW3_API ViewGeomStreamReader : public ViewGeomReader3
{

    public :


      ViewGeomStreamReader(std::istream *stream,ViewGeomSceneGL3 * g,int i = -1) ;

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
   \class ViewMultiGeomReader3
   \brief Create a new thread to read a geom stream

*/
class VIEW3_API ViewMultiGeomReader3 : public ViewGeomReader3
{

    public :


      ViewMultiGeomReader3(const QString& f1,
                          const QString& f2,
                          ViewGeomSceneGL3 * g,
                          int i = -1) ;

    ~ViewMultiGeomReader3();

    virtual void setSecond(const QString&);

    const QString& getSecond() const;

    protected :

      virtual void run();

    /// The second file to read.
    QString __second;

};


/* ----------------------------------------------------------------------- */

/**
   \class ViewLinetreeReader3
   \brief Create a new thread to read a linetree file

*/
class VIEW3_API ViewLinetreeReader3 : public  ViewGeomReader3 {
    public :

      ViewLinetreeReader3(const QString& ligFile,
                     const QString& dtaFile,
                     const QString& smbPath,
                     ViewGeomSceneGL3 * g,
					 bool bigendian = true,
                     bool add = false);

    ~ViewLinetreeReader3();

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


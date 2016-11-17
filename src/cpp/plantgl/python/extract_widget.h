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

#ifndef __extract_widget_h__
#define __extract_widget_h__

#include <boost/python.hpp>
#include <QtGlobal>
#if QT_VERSION >= 0x050000 
    #include <QtWidgets/QWidget>
#else
    #include <QtGui/QWidget>
#endif
#include "exception.h"

/* ----------------------------------------------------------------------- */
template<class Widget>
struct extract_widget {

    typedef Widget * result_type;

    extract_widget(boost::python::object obj):pyobj(obj) {}
    boost::python::object pyobj;

    result_type extract() const
    {
        Widget * widgetptr = NULL;
        boost::python::object pyid = pyobj.attr("winId")();
#if defined(Q_WS_WIN)
		int iwid = boost::python::extract<int>(pyid.attr("__int__")())();
        if(iwid != 0){
			WId wid = WId(iwid);
#else
        WId wid = boost::python::extract<WId>(pyid)();
        if(wid != 0){
#endif
            QWidget * widget = QWidget::find(wid);
            if (widget) {
#ifdef Q_CC_MSVC
                // By default qmake do not compile project with rtti information when
                // using msvc. So do a static cast
                widgetptr = static_cast<Widget *>(widget);
#else
                widgetptr = dynamic_cast<Widget *>(widget);
#endif    
            }
            else { throw PythonExc_ValueError("Cannot extract widget."); }
        }
        else { throw PythonExc_ValueError("Cannot extract widget"); }
        return widgetptr;
    }
    inline result_type operator()() const { return extract(); }
	inline operator result_type () const { return extract(); }

};

/* ----------------------------------------------------------------------- */

#endif

/* ----------------------------------------------------------------------- */

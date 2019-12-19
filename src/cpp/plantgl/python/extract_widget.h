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


#ifndef __extract_widget_h__
#define __extract_widget_h__

#include <boost/python.hpp>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
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

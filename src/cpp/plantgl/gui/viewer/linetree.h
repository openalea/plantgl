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


/*! \file view_linetree.h
    \brief Definition of the viewer class ViewReadLinetree.
*/

#ifndef __view_linetree_h__
#define __view_linetree_h__

/* ----------------------------------------------------------------------- */

#include <QtCore/qvariant.h>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    #include <QtWidgets/qdialog.h>
#else
    #include <QtGui/qdialog.h>
#endif
#include "../gui_config.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;
class QCheckBox;

/* ----------------------------------------------------------------------- */

/// A dialog widget to choose the files of a Linetree.
class VIEW_API ViewReadLinetree : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString LigFile READ getLigFile WRITE setLigFile );
    Q_PROPERTY(QString DtaFile READ getDtaFile WRITE setDtaFile );
    Q_PROPERTY(QString SMBPath READ getSMBPath WRITE setSMBPath );
    Q_PROPERTY(bool bigEndian  READ bigEndian  WRITE setEndianess );

public:
    ViewReadLinetree( bool open = true, QWidget* parent = 0, const char* name = 0, bool modal = true, Qt::WindowFlags fl = 0 );
    ~ViewReadLinetree();

    /// Get the Lig File
    QString getLigFile() const ;

    /// Get the Dta File
    QString getDtaFile() const ;

    /// Get the SMB Path
    QString getSMBPath() const ;

    /// Get Big Endianness
    bool bigEndian() const ;

    /// Get the SMB Path
    QString getKey() const ;

    /// Set the Lig File
    void setLigFile( const QString& f );

    /// Set the Dta File
    void setDtaFile( const QString& f );

    /// Set the SMB Path
    void setSMBPath( const QString& f );

    void setEndianess( bool bigendian );

    void checkKey();
    void checkCfg(const QString& path);

    public slots:

    void checkKey(const QString&);

    /// Select Lig File with file selection dialog.
    void SelectLigFile();

    /// Select Dta File with file selection dialog.
    void SelectDtaFile();

    /// Select SMB Path with path selection dialog.
    void SelectSMBPath();

    void resetKey();

    void ok();

    void testEndianess();

    protected :

    QToolButton* buttonLig;
    QToolButton* buttonDta;
    QToolButton* buttonSmb;
    QLabel* labelKey;
    QLineEdit* editLig;
    QLineEdit* editDta;
    QLineEdit* editSmb;
    QLineEdit * editKey;
    QPushButton* buttonCancel;
    QPushButton* buttonOk;
    QCheckBox * endianBox;
    bool __open;

};

/* ----------------------------------------------------------------------- */
#endif
 // READ_LINETREE_H

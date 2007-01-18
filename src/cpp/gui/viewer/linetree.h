/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): F. Boudon (frederic.boudon@cirad.fr)
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

/*! \file view_linetree.h
    \brief Definition of the viewer class ViewReadLinetree.
*/

#ifndef __view_linetree_h__
#define __view_linetree_h__

/* ----------------------------------------------------------------------- */

#include <qvariant.h>
#include <qdialog.h>
#include "Tools/config.h"

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
class GEOM_API ViewReadLinetree : public QDialog
{ 
    Q_OBJECT
	Q_PROPERTY(QString LigFile READ getLigFile WRITE setLigFile );
	Q_PROPERTY(QString DtaFile READ getDtaFile WRITE setDtaFile );
	Q_PROPERTY(QString SMBPath READ getSMBPath WRITE setSMBPath );
	Q_PROPERTY(bool bigEndian  READ bigEndian  WRITE setEndianess );

public:
    ViewReadLinetree( bool open = true, QWidget* parent = 0, const char* name = 0, bool modal = TRUE, WFlags fl = 0 );
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

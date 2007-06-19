#include "trayicon.h"
#include "qpopupmenu.h"

/*!
  \class TrayIcon3 qtrayicon.h
  \brief The TrayIcon3 class implements an entry in the system tray.
*/

/*!
  Creates a TrayIcon3 object. \a parent and \a name are propagated
  to the QObject constructor. The icon is initially invisible.

  \sa show
*/
TrayIcon3::TrayIcon3( QObject *parent, const char *name )
: QObject(parent, name), pop(0), d(0)
{
}

/*!
  Creates a TrayIcon3 object displaying \a icon and \a tooltip, and opening
  \a popup when clicked with the right mousebutton. \a parent and \a name are
  propagated to the QObject constructor. The icon is initially invisible.

  \sa show
*/
TrayIcon3::TrayIcon3( const QPixmap &icon, const QString &tooltip, QPopupMenu *popup, QObject *parent, const char *name )
: QObject(parent, name), pop(popup), pm(icon), tip(tooltip), d(0)
{
}

/*!
  Removes the icon from the system tray and frees all allocated resources.
*/
TrayIcon3::~TrayIcon3()
{
    sysRemove();
}

/*!
  Sets the context menu to \a popup. The context menu will pop up when the
  user clicks the system tray entry with the right mouse button.
*/
void TrayIcon3::setPopup( QPopupMenu* popup )
{
    pop = popup;
}

/*!
  Returns the current popup menu.

  \sa setPopup
*/
QPopupMenu* TrayIcon3::popup() const
{
    return pop;
}

/*!
  \property TrayIcon3::icon
  \brief the system tray icon.
*/
void TrayIcon3::setIcon( const QPixmap &icon )
{
    pm = icon;
    sysUpdateIcon();
}

QPixmap TrayIcon3::icon() const
{
    return pm;
}

/*!
  \property TrayIcon3::toolTip
  \brief the tooltip for the system tray entry
  
  On some systems, the tooltip's length is limited and will be truncated as necessary.
*/
void TrayIcon3::setToolTip( const QString &tooltip )
{
    tip = tooltip;
    sysUpdateToolTip();
}


void TrayIcon3::setBalloonMessage( const QString& title, 
								   const QString& mess, 
								   uint timeout )
{
    sysBalloonMsg(title,mess,timeout);
}


QString TrayIcon3::toolTip() const
{
    return tip;
}

/*!
  Shows the icon in the system tray.

  \sa hide
*/
void TrayIcon3::show()
{
    sysInstall();
}

/*!
  Hides the system tray entry.
*/
void TrayIcon3::hide()
{
    sysRemove();
}

/*!
  \reimp
*/
bool TrayIcon3::event( QEvent *e )
{
    switch ( e->type() ) {
    case QEvent::MouseMove:
	mouseMoveEvent( (QMouseEvent*)e );
	break;

    case QEvent::MouseButtonPress:
	mousePressEvent( (QMouseEvent*)e );
	break;

    case QEvent::MouseButtonRelease:
	mouseReleaseEvent( (QMouseEvent*)e );
	break;

    case QEvent::MouseButtonDblClick:
	mouseDoubleClickEvent( (QMouseEvent*)e );
	break;
    default:
	return QObject::event( e );
    }

    return TRUE;
}

/*!
  This event handler can be reimplemented in a subclass to receive
  mouse move events for the system tray entry.

  \sa mousePressEvent(), mouseReleaseEvent(), mouseDoubleClickEvent(),  QMouseEvent
*/
void TrayIcon3::mouseMoveEvent( QMouseEvent *e )
{
    e->ignore();
}

/*!
  This event handler can be reimplemented in a subclass to receive
  mouse press events for the system tray entry.

  \sa mouseReleaseEvent(), mouseDoubleClickEvent(),
  mouseMoveEvent(), QMouseEvent
*/
void TrayIcon3::mousePressEvent( QMouseEvent *e )
{
    e->ignore();
}

/*!
  This event handler can be reimplemented in a subclass to receive
  mouse release events for the system tray entry.

  The default implementations opens the context menu when the entry
  has been clicked with the right mouse button.

  \sa setPopup(), mousePressEvent(), mouseDoubleClickEvent(),
  mouseMoveEvent(), QMouseEvent
*/
void TrayIcon3::mouseReleaseEvent( QMouseEvent *e )
{
    switch ( e->button() ) {
    case RightButton:
	if ( pop ) {
	    // Necessary to make keyboard focus
	    // and menu closing work on Windows.
	    pop->setActiveWindow();
	    pop->popup( e->globalPos() );
	    pop->setActiveWindow();
	    e->accept();
	}
	break;
    case LeftButton:
	emit clicked( e->globalPos() );
	break;
    default:
	break;
    }
    e->ignore();
}

/*!
  This event handler can be reimplemented in a subclass to receive
  mouse double click events for the system tray entry.

  Note that the system tray entry gets a mousePressEvent() and a
  mouseReleaseEvent() before the mouseDoubleClickEvent().

  \sa mousePressEvent(), mouseReleaseEvent(),
  mouseMoveEvent(), QMouseEvent
*/
void TrayIcon3::mouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() == LeftButton )
	emit doubleClicked( e->globalPos() );
    e->ignore();
}

/*!
  \fn void TrayIcon3::clicked( const QPoint &p )

  This signal is emitted when the user clicks the system tray icon
  with the left mouse button, with \a p being the global mouse position
  at that moment.
*/

/*!
  \fn void TrayIcon3::doubleClicked( const QPoint &p )

  This signal is emitted when the user double clicks the system tray
  icon with the left mouse button, with \a p being the global mouse position
  at that moment.
*/

void TrayIcon3::mouseBalloonClickEvent( QMouseEvent *e )
{
    if ( e->button() == LeftButton )
	emit balloonClicked( e->globalPos() );
    e->ignore();
}

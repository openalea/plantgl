#include "trayicon.h"
#include "qpopupmenu.h"

/*!
  \class TrayIcon qtrayicon.h
  \brief The TrayIcon class implements an entry in the system tray.
*/

/*!
  Creates a TrayIcon object. \a parent and \a name are propagated
  to the QObject constructor. The icon is initially invisible.

  \sa show
*/
TrayIcon::TrayIcon( QObject *parent, const char *name )
: QObject(parent, name), pop(0), d(0)
{
}

/*!
  Creates a TrayIcon object displaying \a icon and \a tooltip, and opening
  \a popup when clicked with the right mousebutton. \a parent and \a name are
  propagated to the QObject constructor. The icon is initially invisible.

  \sa show
*/
TrayIcon::TrayIcon( const QPixmap &icon, const QString &tooltip, QPopupMenu *popup, QObject *parent, const char *name )
: QObject(parent, name), pop(popup), pm(icon), tip(tooltip), d(0)
{
}

/*!
  Removes the icon from the system tray and frees all allocated resources.
*/
TrayIcon::~TrayIcon()
{
    sysRemove();
}

/*!
  Sets the context menu to \a popup. The context menu will pop up when the
  user clicks the system tray entry with the right mouse button.
*/
void TrayIcon::setPopup( QPopupMenu* popup )
{
    pop = popup;
}

/*!
  Returns the current popup menu.

  \sa setPopup
*/
QPopupMenu* TrayIcon::popup() const
{
    return pop;
}

/*!
  \property TrayIcon::icon
  \brief the system tray icon.
*/
void TrayIcon::setIcon( const QPixmap &icon )
{
    pm = icon;
    sysUpdateIcon();
}

QPixmap TrayIcon::icon() const
{
    return pm;
}

/*!
  \property TrayIcon::toolTip
  \brief the tooltip for the system tray entry
  
  On some systems, the tooltip's length is limited and will be truncated as necessary.
*/
void TrayIcon::setToolTip( const QString &tooltip )
{
    tip = tooltip;
    sysUpdateToolTip();
}


void TrayIcon::setBalloonMessage( const QString& title, 
								   const QString& mess, 
								   uint timeout )
{
    sysBalloonMsg(title,mess,timeout);
}


QString TrayIcon::toolTip() const
{
    return tip;
}

/*!
  Shows the icon in the system tray.

  \sa hide
*/
void TrayIcon::show()
{
    sysInstall();
}

/*!
  Hides the system tray entry.
*/
void TrayIcon::hide()
{
    sysRemove();
}

/*!
  \reimp
*/
bool TrayIcon::event( QEvent *e )
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
void TrayIcon::mouseMoveEvent( QMouseEvent *e )
{
    e->ignore();
}

/*!
  This event handler can be reimplemented in a subclass to receive
  mouse press events for the system tray entry.

  \sa mouseReleaseEvent(), mouseDoubleClickEvent(),
  mouseMoveEvent(), QMouseEvent
*/
void TrayIcon::mousePressEvent( QMouseEvent *e )
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
void TrayIcon::mouseReleaseEvent( QMouseEvent *e )
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
void TrayIcon::mouseDoubleClickEvent( QMouseEvent *e )
{
    if ( e->button() == LeftButton )
	emit doubleClicked( e->globalPos() );
    e->ignore();
}

/*!
  \fn void TrayIcon::clicked( const QPoint &p )

  This signal is emitted when the user clicks the system tray icon
  with the left mouse button, with \a p being the global mouse position
  at that moment.
*/

/*!
  \fn void TrayIcon::doubleClicked( const QPoint &p )

  This signal is emitted when the user double clicks the system tray
  icon with the left mouse button, with \a p being the global mouse position
  at that moment.
*/

void TrayIcon::mouseBalloonClickEvent( QMouseEvent *e )
{
    if ( e->button() == LeftButton )
	emit balloonClicked( e->globalPos() );
    e->ignore();
}

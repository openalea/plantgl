
#define _WIN32_IE 0x0500
#include "trayicon.h"

#include <qwidget.h>
#include <qapplication.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qcursor.h>
#include <qlibrary.h>

#include <qt_windows.h>

#define FORCE_BALLOON_MSG

#ifndef NIF_INFO

#ifdef FORCE_BALLOON_MSG

#define NIF_INFO        0x00000010
#define NIIF_INFO       0x00000001

typedef struct _MYNOTIFYICONDATAA {
        DWORD cbSize;
        HWND hWnd;
        UINT uID;
        UINT uFlags;
        UINT uCallbackMessage;
        HICON hIcon;
        CHAR   szTip[128];
        DWORD dwState;
        DWORD dwStateMask;
        CHAR   szInfo[256];
        union {
            UINT  uTimeout;
            UINT  uVersion;
        } DUMMYUNIONNAME;
        CHAR   szInfoTitle[64];
        DWORD dwInfoFlags;
        GUID guidItem;
} MYNOTIFYICONDATAA, *MYPNOTIFYICONDATAA;

typedef struct _MYNOTIFYICONDATAW {
        DWORD cbSize;
        HWND hWnd;
        UINT uID;
        UINT uFlags;
        UINT uCallbackMessage;
        HICON hIcon;
        WCHAR  szTip[128];
        DWORD dwState;
        DWORD dwStateMask;
        WCHAR  szInfo[256];
        union {
            UINT  uTimeout;
            UINT  uVersion;
        } DUMMYUNIONNAME;
        WCHAR  szInfoTitle[64];
        DWORD dwInfoFlags;
        GUID guidItem;
} MYNOTIFYICONDATAW, *MYPNOTIFYICONDATAW;


#define BALLOON_MSG
#endif

#else

#define BALLOON_MSG
#define MYNOTIFYICONDATAW NOTIFYICONDATAW
#define MYNOTIFYICONDATAA NOTIFYICONDATAA

#endif

#ifndef NIN_BALLOONUSERCLICK 
#define NIN_BALLOONUSERCLICK (WM_USER + 5)
#endif

static uint MYWM_TASKBARCREATED = 0;
#define MYWM_NOTIFYICON	(WM_APP+101)
#define MYWM_NOTIFYBALLOON	(WM_APP+102)

typedef BOOL (WINAPI *PtrShell_NotifyIcon)(DWORD,PNOTIFYICONDATA);
static PtrShell_NotifyIcon ptrShell_NotifyIcon = 0;

static void resolveLibs()
{
    QLibrary lib("shell32");
    lib.setAutoUnload( FALSE );
    static bool triedResolve = FALSE;
    if ( !ptrShell_NotifyIcon && !triedResolve ) {
	triedResolve = TRUE;
	ptrShell_NotifyIcon = (PtrShell_NotifyIcon) lib.resolve( "Shell_NotifyIconW" );
    }
}

class TrayIcon3::TrayIcon3Private : public QWidget
{

public:
    TrayIcon3Private( TrayIcon3 *object ) 
	: QWidget( 0 ), hIcon( 0 ), hMask( 0 ), iconObject( object )
    {
	if ( !MYWM_TASKBARCREATED ) {
#if defined(UNICODE)
	    if ( qWinVersion() & Qt::WV_NT_based )
		MYWM_TASKBARCREATED = RegisterWindowMessageW( (TCHAR*)"TaskbarCreated" );
	    else
#endif
		MYWM_TASKBARCREATED = RegisterWindowMessageA( "TaskbarCreated" );
	}
    }

    ~TrayIcon3Private()
    {
	if ( hMask )
	    DeleteObject( hMask );
	if ( hIcon )
	    DestroyIcon( hIcon );	
    }

    // the unavoidable A/W versions. Don't forget to keep them in sync!
    bool trayMessageA( DWORD msg ) 
    {
		bool res;
		
		NOTIFYICONDATAA tnd;
		memset( &tnd, 0, sizeof(NOTIFYICONDATAA) );
		tnd.cbSize		= sizeof(NOTIFYICONDATAA);
		tnd.hWnd		= winId();
		
		if ( msg != NIM_DELETE ) {
			tnd.uFlags		= NIF_MESSAGE|NIF_ICON|NIF_TIP;
			tnd.uCallbackMessage= MYWM_NOTIFYICON;
			tnd.hIcon		= hIcon;
			if ( !iconObject->toolTip().isNull() ) {
				// Tip is limited to 63 + NULL; lstrcpyn appends a NULL terminator.
				QString tip = iconObject->toolTip().left( 63 ) + QChar();
				lstrcpynA(tnd.szTip, (const char*)tip.local8Bit(), QMIN( tip.length()+1, 64 ) );
			}
		}
		
		res = Shell_NotifyIconA(msg, &tnd);
		
		return res;
    }

    bool trayBalloonMessageA( const QString& title, const QString& mess, uint timeout ) 
    {
#ifdef BALLOON_MSG
		bool res;
		resolveLibs();
		if ( ! (ptrShell_NotifyIcon && qWinVersion() & Qt::WV_NT_based) )
			return false; 

		
		MYNOTIFYICONDATAA tnd;
		memset( &tnd, 0, sizeof(MYNOTIFYICONDATAA) );
		tnd.cbSize		= sizeof(MYNOTIFYICONDATAA);
		tnd.hWnd		= winId();
		
		tnd.uFlags		= NIF_INFO | NIF_MESSAGE;
		tnd.dwInfoFlags = NIIF_INFO;
		tnd.uTimeout		= timeout;
		tnd.uCallbackMessage= MYWM_NOTIFYBALLOON;
		QString mymess = mess.left( 254 ) + QChar();
		lstrcpynA(tnd.szInfo, (const char*)mymess.local8Bit(), QMIN( mymess.length()+1, 255 ) );
		QString mytitle = title.left( 62 ) + QChar();
		lstrcpynA(tnd.szInfoTitle, (const char*)mytitle.local8Bit(), QMIN( mytitle.length()+1, 63 ) );
		
		res = Shell_NotifyIconA(NIM_MODIFY, (PNOTIFYICONDATAA)&tnd);
		return res;
#else
		return false;
#endif
    }

#if defined(UNICODE)

    bool trayMessageW( DWORD msg ) 
    {
	bool res;
	resolveLibs();
	if ( ! (ptrShell_NotifyIcon && qWinVersion() & Qt::WV_NT_based) )
	    return trayMessageA( msg );
	NOTIFYICONDATAW tnd;
	memset( &tnd, 0, sizeof(NOTIFYICONDATAW) );
	tnd.cbSize		= sizeof(NOTIFYICONDATAW);
	tnd.hWnd		= winId();

	if ( msg != NIM_DELETE ) {
	    tnd.uFlags		= NIF_MESSAGE|NIF_ICON|NIF_TIP;
	    tnd.uCallbackMessage= MYWM_NOTIFYICON;
	    tnd.hIcon		= hIcon;
	    if ( !iconObject->toolTip().isNull() ) {
		// Tip is limited to 63 + NULL; lstrcpyn appends a NULL terminator.
		QString tip = iconObject->toolTip().left( 63 ) + QChar();
		lstrcpynW(tnd.szTip, (TCHAR*)tip.unicode(), QMIN( tip.length()+1, 64 ) );
	    }
	}
	res = ptrShell_NotifyIcon(msg, &tnd);
	return res;
    }


    bool trayBalloonMessageW( const QString& title, const QString& mess, uint timeout ) 
    {
#ifdef BALLOON_MSG
	bool res;
	resolveLibs();
	if ( ! (ptrShell_NotifyIcon && qWinVersion() & Qt::WV_NT_based) )
	    return false; 

	MYNOTIFYICONDATAW tnd;
	memset( &tnd, 0, sizeof(MYNOTIFYICONDATAW) );
	tnd.cbSize		= sizeof(MYNOTIFYICONDATAW);
	tnd.hWnd		= winId();

    tnd.uFlags		= NIF_INFO;
	tnd.dwInfoFlags = NIIF_INFO;
    tnd.uTimeout		= timeout;
	QString mymess = mess.left( 254 ) + QChar();
	lstrcpynW(tnd.szInfo, (TCHAR*)mymess.unicode(), QMIN( mymess.length()+1, 255 ) );
	QString mytitle = title.left( 62 ) + QChar();
	lstrcpynW(tnd.szInfoTitle, (TCHAR*)mytitle.unicode(), QMIN( mytitle.length()+1, 63 ) );

	res = ptrShell_NotifyIcon(NIM_MODIFY, (PNOTIFYICONDATAW) &tnd);
	return res;
#else
		return false;
#endif
    }

#endif

    bool iconDrawItem(LPDRAWITEMSTRUCT lpdi)
    {
	if (!hIcon)
	    return FALSE;

	DrawIconEx(lpdi->hDC, lpdi->rcItem.left, lpdi->rcItem.top, hIcon,
		0, 0, 0, NULL, DI_NORMAL );

	return TRUE;
    }

    bool winEvent( MSG *m )
    {
		switch(m->message) {
		case WM_DRAWITEM:
			return iconDrawItem( (LPDRAWITEMSTRUCT)m->lParam );
		case MYWM_NOTIFYICON:
			{
				QMouseEvent *e = 0;
				QPoint gpos = QCursor::pos();
				bool balloon = false;
				switch (m->lParam)
				{
				case WM_MOUSEMOVE:
					e = new QMouseEvent( QEvent::MouseMove, mapFromGlobal( gpos ), gpos, 0, 0 );
					break;
				case WM_LBUTTONDOWN:
					e = new QMouseEvent( QEvent::MouseButtonPress, mapFromGlobal( gpos ), gpos, LeftButton, LeftButton );
					break;
				case WM_LBUTTONUP:
					e = new QMouseEvent( QEvent::MouseButtonRelease, mapFromGlobal( gpos ), gpos, LeftButton, LeftButton );
					break;
				case WM_LBUTTONDBLCLK:
					e = new QMouseEvent( QEvent::MouseButtonDblClick, mapFromGlobal( gpos ), gpos, LeftButton, LeftButton );
					break;
				case WM_RBUTTONDOWN:
					e = new QMouseEvent( QEvent::MouseButtonPress, mapFromGlobal( gpos ), gpos, RightButton, RightButton );
					break;
				case WM_RBUTTONUP:
					e = new QMouseEvent( QEvent::MouseButtonRelease, mapFromGlobal( gpos ), gpos, RightButton, RightButton );
					break;
				case WM_RBUTTONDBLCLK:
					e = new QMouseEvent( QEvent::MouseButtonDblClick, mapFromGlobal( gpos ), gpos, RightButton, RightButton );
					break;
				case WM_MBUTTONDOWN:
					e = new QMouseEvent( QEvent::MouseButtonPress, mapFromGlobal( gpos ), gpos, MidButton, MidButton );
					break;
				case WM_MBUTTONUP:
					e = new QMouseEvent( QEvent::MouseButtonRelease, mapFromGlobal( gpos ), gpos, MidButton, MidButton );
					break;
				case WM_MBUTTONDBLCLK:
					e = new QMouseEvent( QEvent::MouseButtonDblClick, mapFromGlobal( gpos ), gpos, MidButton, MidButton );
					break;
				case WM_CONTEXTMENU:
					e = new QMouseEvent( QEvent::MouseButtonRelease, mapFromGlobal( gpos ), gpos, RightButton, RightButton );
					break;
				case NIN_BALLOONUSERCLICK :
					e = new QMouseEvent( QEvent::MouseButtonRelease, mapFromGlobal( gpos ), gpos, LeftButton, LeftButton );
					balloon = true;
					break;
				default:
					break;
				}
				if ( e ) {
					bool res = true;
					if(balloon) iconObject->mouseBalloonClickEvent( e );
					else res = QApplication::sendEvent( iconObject, e );
					delete e;
					return res;
				}
			}
			break;
		default:
			if ( m->message == MYWM_TASKBARCREATED ) {
#if defined(UNICODE)
				if ( qWinVersion() & Qt::WV_NT_based )
					trayMessageW( NIM_ADD );
				else
#endif
					trayMessageA( NIM_ADD );
			}
			break;
		}
		
		return QWidget::winEvent( m );
    }
	
    HICON		hIcon;
    HBITMAP		hMask;
    TrayIcon3		*iconObject;
};

static HBITMAP createIconMask( const QPixmap &qp )
{
    QImage bm = qp.convertToImage();
    int w = bm.width();
    int h = bm.height();
    int bpl = ((w+15)/16)*2;			// bpl, 16 bit alignment
    uchar *bits = new uchar[bpl*h];
    bm.invertPixels();
    for ( int y=0; y<h; y++ )
	memcpy( bits+y*bpl, bm.scanLine(y), bpl );
    HBITMAP hbm = CreateBitmap( w, h, 1, 1, bits );
    delete [] bits;
    return hbm;
}

static HICON createIcon( const QPixmap &pm, HBITMAP &hbm )
{
    QPixmap maskpm( pm.size(), pm.depth(), QPixmap::NormalOptim );
    QBitmap mask( pm.size(), FALSE, QPixmap::NormalOptim );
    if ( pm.mask() ) {
        maskpm.fill( Qt::black );			// make masked area black
        bitBlt( &mask, 0, 0, pm.mask() );
    } else {
        maskpm.fill( Qt::color1 );
    }
    
    bitBlt( &maskpm, 0, 0, &pm);
    ICONINFO iconInfo;
    iconInfo.fIcon    = TRUE;
    iconInfo.hbmMask  = createIconMask(mask);
    hbm = iconInfo.hbmMask;
    iconInfo.hbmColor = maskpm.hbm();

    return CreateIconIndirect( &iconInfo );
}

void TrayIcon3::sysInstall()
{
    if ( d )
	return;

    d = new TrayIcon3Private( this );
    d->hIcon = createIcon( pm, d->hMask );

#if defined(UNICODE)
    if ( qWinVersion() & Qt::WV_NT_based )
	d->trayMessageW( NIM_ADD );
    else
#endif
	d->trayMessageA( NIM_ADD );
}

void TrayIcon3::sysRemove()
{
    if ( !d )
	return;

#if defined(UNICODE)
    if ( qWinVersion() & Qt::WV_NT_based )
	d->trayMessageW( NIM_DELETE );
    else
#endif
	d->trayMessageA( NIM_DELETE );

    delete d;
    d = 0;
}

void TrayIcon3::sysUpdateIcon()
{
    if ( !d )
	return;

    if ( d->hMask )
	DeleteObject( d->hMask );
    if ( d->hIcon )
	DestroyIcon( d->hIcon );

    d->hIcon = createIcon( pm, d->hMask );

#if defined(UNICODE)
    if ( qWinVersion() & Qt::WV_NT_based )
	d->trayMessageW( NIM_MODIFY );
    else
#endif
	d->trayMessageA( NIM_MODIFY );
}

void TrayIcon3::sysUpdateToolTip()
{
    if ( !d )
	return;

#if defined(UNICODE)
    if ( qWinVersion() & Qt::WV_NT_based )
	d->trayMessageW( NIM_MODIFY );
    else
#endif
	d->trayMessageA( NIM_MODIFY );
}

void TrayIcon3::sysBalloonMsg(const QString& title, const QString& mess, uint timeout)
{
    if ( !d )
	return;

#if defined(UNICODE)
    if ( qWinVersion() & Qt::WV_NT_based )
	d->trayBalloonMessageW( title, mess,timeout );
    else
#endif
	d->trayBalloonMessageA( title, mess,timeout );
}

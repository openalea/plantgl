#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#ifndef _USE_OLD_IOSTREAMS

using namespace std;

#endif

// maximum mumber of lines the output console should have

static const WORD MAX_CONSOLE_LINES = 1000;
static bool isrunning = false;
#endif

void RedirectIOToConsole()

{
#ifdef _WIN32
        if(!isrunning){
        int hConHandle;
        long lStdHandle;

        CONSOLE_SCREEN_BUFFER_INFO coninfo;
        FILE *fp;

        // allocate a console for this app
        AllocConsole();

        // set the screen buffer to be big enough to let us scroll text
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
        coninfo.dwSize.X = MAX_CONSOLE_LINES/4;
        coninfo.dwSize.Y = MAX_CONSOLE_LINES;
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);


        // redirect unbuffered STDOUT to the console
        lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        fp = _fdopen( hConHandle, "w" );
        *stdout = *fp;

        setvbuf( stdout, NULL, _IONBF, 0 );

        // redirect unbuffered STDIN to the console

        lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        fp = _fdopen( hConHandle, "r" );
        *stdin = *fp;
        setvbuf( stdin, NULL, _IONBF, 0 );

        // redirect unbuffered STDERR to the console

        lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        fp = _fdopen( hConHandle, "w" );
        *stderr = *fp;
        setvbuf( stderr, NULL, _IONBF, 0 );

        // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
        // point to console as well
		ios::sync_with_stdio();
        if(!ios::sync_with_stdio(true)) 
		  printf("* Error for sync_with_stdio of cout, wcout, cin, wcin, wcerr, cerr, wclog and clog\n");

        isrunning = true;
        printf("* In/Output Console for STD[IN|OUT|ERR]\n");
        cerr <<"* In/Output Console for c[in|err]" << endl;
        cout <<"* In/Output Console for cout" << endl;
        }
#endif
}


//End of File




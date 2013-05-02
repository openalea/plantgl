"""
This script apply bison and flex on the file of the plantgl project.
Usefull when working with Visual Studio or non classical environment.
"""
import sys, os


def apply_bison(fname,symbol_prefix = 'scne_yy',outdir=None):
    cmd = os.path.join(sys.exec_prefix,'Lib','site-packages','bisonflex-2.4.1_2.5.35-win32.egg','bin','bison.exe')
    if not os.path.exists(cmd) :
        cmd = 'bison' 
    cmd += ' -dv '
    if symbol_prefix:
        cmd += '-p '+symbol_prefix+' '
    cmd += '"'+fname+'"'
    outfile = os.path.splitext(fname)[0]+".cpp"
    if outdir:
     outfile = join(outdir,outfile)
    cmd += ' -o "'+outfile+'"'
    print cmd
    os.system(cmd)

  
def apply_flex(fname,symbol_prefix = 'scne_yy',outdir=None):
    cmd = os.path.join(sys.exec_prefix,'Lib','site-packages','bisonflex-2.4.1_2.5.35-win32.egg','bin','flex.exe')
    if not os.path.exists(cmd) :
        cmd = 'flex' 
    cmd += ' '
    if symbol_prefix:
        cmd += '-P'+symbol_prefix+' '
    outfile = os.path.splitext(fname)[0]+".cpp"
    if outdir:
     outfile = join(outdir,outfile)
    cmd += ' -o "'+outfile+'" '
    cmd += '"'+fname+'"'
    print cmd
    os.system(cmd)

if __name__ == '__main__':
    outdir = None # "build-scons"
    apply_flex("src/cpp/plantgl/algo/codec/scne_scanner.l",outdir=outdir)
    apply_bison("src/cpp/plantgl/algo/codec/scne_parser.y",outdir=outdir)
    
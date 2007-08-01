# Postinstall scripts


def install():
    
    from openalea.deploy.shortcut import create_win_shortcut, set_win_reg, create_fd_shortcut
    from openalea.deploy import get_base_dir
    import sys, os
    from os.path import join as pj

    # Get the location of the installed egg
    base_dir = get_base_dir('plantgl')

    bin_dir = pj(base_dir, "bin")
    print bin_dir
    
    create_win_shortcut(name = 'PlantGL Viewer',
                        target = pj(bin_dir, "pglviewer.exe",),
                        arguments = "",
                        startin = "", 
                        icon = pj(bin_dir, "pglviewer.exe",),
                        description = "PlantGL Viewer",
                        menugroup = "OpenAlea")
        

    # Windows registery (list of (key, subkey, name, value))
    winreg = [ ('HKCR', '.smb',   '', 'PGLFile') ,
               ('HKCR', '.lig',   '', 'PGLFile') ,
               ('HKCR', '.geom',  '', 'PGLFile') ,
               ('HKCR', '.bgeom', '', 'PGLFile') ,
               ('HKCR', 'PGLFile', '', 'PlantGL File') ,
               ('HKCR', 'PGLFile\\DefaultIcon', '', pj(bin_dir,'pglviewer.exe')+",1") ,
               ('HKCR', 'PGLFile\\shell', '', "open") ,
               ('HKCR', 'PGLFile\\shell\\open', '', "Open") ,
               ('HKCR', 'PGLFile\\shell\\open\\command', '', '"'+pj(bin_dir,'pglviewer.exe')+'" "%1"') ,
               ('HKCR', 'PGLFile\\shell\\add', '', "Add") ,
               ('HKCR', 'PGLFile\\shell\\add\\command', '', '"'+pj(bin_dir,'pglviewer.exe')+'" -a "%1"') ,
               # ('HKCR', 'PGLFile\\shell\\edit', '', "Edit") ,
               # ('HKCR', 'PGLFile\\shell\\edit\\command', '', '"'+pj(config.bin_dir,'pgleditor.exe')+'" "%1"') ,
               # ('HKCR', 'PGLFile\\shell\\geom2pov', '', "Translate To Povray") ,
               # ('HKCR', 'PGLFile\\shell\\geom2pov\\command', '', '"'+pj(config.bin_dir,'pgl2pov.exe')+'" "%1"') ,
               ]


    for t in winreg:
        set_win_reg(*t)

   


def uninstall():
    pass

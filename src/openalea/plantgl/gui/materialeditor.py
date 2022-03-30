from openalea.plantgl.all import *
from openalea.plantgl.gui.qt import qt    
from OpenGL.GL import *
from OpenGL.GLU import *
import os

from openalea.plantgl.gui.qt.QtCore import QObject, QPoint, QTimer, Qt, pyqtSignal
from openalea.plantgl.gui.qt.QtGui import QCursor, QImageReader, QPixmap, QGuiApplication
from openalea.plantgl.gui.qt.QtWidgets import QApplication, QDialog, QDockWidget, QFileDialog, QMenu, QMessageBox, QScrollArea, QSplashScreen, QVBoxLayout, QWidget
from openalea.plantgl.gui.qt.QtOpenGL import QGLWidget 

class MaterialPanelView (QGLWidget):
    valueChanged = pyqtSignal()

    def __init__(self,parent):
        QGLWidget.__init__(self,parent)
        self.unitsize = 30
        self.sphere = None
        self.spherelist = None
        self.darkcheck = [0.5,0.5,0.5]
        self.lightcheck = [0.9,0.9,0.9]
        self.checklist = None
        self.selectedBorderList = None
        self.turtle = PglTurtle()
        self.mousepos = None
        self.initpos = None
        self.menuselection = None        
        self.selectionbegin = None
        self.selectionend = None
        self.cursorselection = None
        self.cutaction = False
        self.preview = None
        self.previewselection = None
        self.previewtrigger = QTimer(self)
        self.previewtrigger.setInterval(1000)
        self.previewtrigger.setSingleShot(True)
        self.clipboard = None
        self.previewtrigger.timeout.connect(self.displayPreview)
        self.valueChanged.connect(self.computeSize)
        
        self.lastTextureDir = '.'
        self.preview_start_show = False
        self.defaultmat = Material('default')
        self.setAcceptDrops(True)

    def setTurtle(self,turtle):
        self.turtle = turtle
        self.cutaction = False
        self.computeSize()
        if self.isVisible(): self.updateGL()
    def getTurtle(self):
        return self.turtle
    def getMaterial(self,index):
        return self.turtle.getMaterial(index)
    def setMaterial(self,index,value):
        self.turtle.setMaterial(index,value)
    def insertMaterial(self,index,value):
        self.turtle.insertMaterial(index,value)
    def delMaterial(self,index):
        self.turtle.removeColor(index)
    def nbMaterial(self):
        return self.turtle.getColorListSize()
    def swapMaterial(self,index1,index2):
        mat1,mat2 = self.getMaterial(index1),self.getMaterial(index2)
        mat1.name = 'Color_'+str(index2)
        mat2.name = 'Color_'+str(index1)
        self.setMaterial(index1,mat2),self.setMaterial(index2,mat1)
    def setAppearanceList(self,appList):
        # for i,mat in enumerate(materialList):
        #     self.setMaterial(i,mat)
        # if self.nbMaterial() > len(materialList):
        #     for i in xrange(self.nbMaterial()-1,len(materialList)-1,-1):
        #         self.delMaterial(i)
        self.turtle.setColorList(appList)
        self.cutaction = False
        self.computeSize()
        if self.isVisible(): self.updateGL()

    def getAppearanceList(self):
        return self.turtle.getColorList()

    def resizeGL(self,w,h):
        if w == 0 or h == 0: return
        self.computeSize()
    def computeSize(self,w=None,h=None):
        if w is None: w = self.width()
        if h is None: h = self.height()
        if w == 0 or h == 0: return
        nbcitem, nbritem = self.getNbColRow(w,h)
        pcolor = self.positionColor(self.nbMaterial())
        maxwidth = pcolor[0]+(2*self.unitsize)
        self.setMinimumSize(maxwidth,2*self.unitsize)

    def initializeGL(self):
        glClearColor(1.0,1.0,1.0,1.0)

        glShadeModel(GL_SMOOTH)
        glLightfv(GL_LIGHT0,GL_POSITION,[1000,1000,2000,0])
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,[1.0,1.0,1.0,1.0]) 
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA)  

        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_BLEND)

        if self.sphere:
            gluDeleteQuadric(self.sphere)
        self.sphere = gluNewQuadric()
        gluQuadricDrawStyle(self.sphere,GLU_FILL)
        gluQuadricNormals(self.sphere,GLU_SMOOTH)

        self.spherelist = glGenLists(1)
        glNewList(self.spherelist,GL_COMPILE)
        gluSphere(self.sphere,self.unitsize*0.8,80,80)
        glEndList()
        
        checkdepth = -self.unitsize
        self.checklist = glGenLists(1)
        glNewList(self.checklist,GL_COMPILE)
        glDisable(GL_LIGHTING)
        glBegin(GL_QUADS)
        glColor3fv(self.lightcheck)
        glVertex3f(-self.unitsize,self.unitsize,checkdepth)
        glVertex3f(0,self.unitsize,checkdepth)
        glVertex3f(0,0,checkdepth)
        glVertex3f(-self.unitsize,0,checkdepth)

        glVertex3f(0,0,checkdepth)
        glVertex3f(self.unitsize,0,checkdepth)
        glVertex3f(self.unitsize,-self.unitsize,checkdepth)
        glVertex3f(0,-self.unitsize,checkdepth)

        glColor3fv(self.darkcheck)
        glVertex3f(0,self.unitsize,checkdepth)
        glVertex3f(self.unitsize,self.unitsize,checkdepth)
        glVertex3f(self.unitsize,0,checkdepth)
        glVertex3f(0,0,checkdepth)

        glVertex3f(-self.unitsize,0,checkdepth)
        glVertex3f(0,0,checkdepth)
        glVertex3f(0,-self.unitsize,checkdepth)
        glVertex3f(-self.unitsize,-self.unitsize,checkdepth)
        glEnd()
        glEndList()
        
        self.texturedQuadList = glGenLists(1)
        glNewList(self.texturedQuadList,GL_COMPILE)
        glBegin(GL_QUADS)
        glTexCoord2d(0,1)
        glVertex3f(-self.unitsize,-self.unitsize,checkdepth)
        glTexCoord2d(1,1)
        glVertex3f(self.unitsize,-self.unitsize,checkdepth)
        glTexCoord2d(1,0)
        glVertex3f(self.unitsize,self.unitsize,checkdepth)
        glTexCoord2d(0,0)
        glVertex3f(-self.unitsize,self.unitsize,checkdepth)
        glEnd()
        glEndList()
        
        borderdepth = -self.unitsize+1
        self.selectedBorderList = glGenLists(1)
        glNewList(self.selectedBorderList,GL_COMPILE)
        glLineWidth(3)
        glColor3f(0.3,0.3,0.3)
        glBegin(GL_LINE_STRIP)
        glVertex3f(-self.unitsize,-self.unitsize,borderdepth)
        glVertex3f(self.unitsize,-self.unitsize,borderdepth)
        glVertex3f(self.unitsize,self.unitsize,borderdepth)
        glVertex3f(-self.unitsize,self.unitsize,borderdepth)
        glVertex3f(-self.unitsize,-self.unitsize,borderdepth)
        glEnd()
        glLineWidth(1)
        glColor3f(0.0,0.0,0.0)
        borderdepth += 1 
        glBegin(GL_LINE_STRIP)
        glVertex3f(-self.unitsize,-self.unitsize,borderdepth)
        glVertex3f(self.unitsize,-self.unitsize,borderdepth)
        glVertex3f(self.unitsize,self.unitsize,borderdepth)
        glVertex3f(-self.unitsize,self.unitsize,borderdepth)
        glVertex3f(-self.unitsize,-self.unitsize,borderdepth)
        glEnd()
        glEndList()

    def paintGL(self):
        cursorselection = -1
        if not self.isVisible(): return
        w,h = self.width(), self.height()
        if w == 0 or h == 0 : return
        if self.mousepos != None and self.geometry().contains(self.mousepos):            
            cursorselection = self.selectedColor(self.mousepos.x(),self.mousepos.y())
        scaling = self.window().devicePixelRatio()
        glViewport(0,0,int(w*scaling),int(h*scaling))
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0,w,h,0,-3000,1000)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        try:
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
        except: 
            # The visible attribute is not correctly set on mac. 
            # The widget is not really visible and the initialization failed.
            return
        glShadeModel(GL_SMOOTH)
        nbcitem, nbritem = self.getNbColRow(w,h)
        colindex = 0
        d = Discretizer()
        glr = GLRenderer(d)
        selectionbegin = self.selectionbegin
        selectionend = self.selectionend
        for i in range(0,int(nbcitem)):
            for j in range(0,int(nbritem)):
                if colindex < self.nbMaterial():
                    curmat = self.getMaterial(colindex)
                else:
                    curmat = self.defaultmat
                istexture = curmat.isTexture()
                inselection = self.isInSelection(colindex)
                glPushMatrix()
                glTranslate(self.unitsize+2*self.unitsize*i,self.unitsize+2*self.unitsize*j,0)
                if inselection and not self.initpos is None:
                    glCallList(self.checklist)
                    decal = self.mousepos-self.initpos
                    glTranslate(decal.x(),decal.y(),2*self.unitsize)
                if istexture:
                    glCallList(self.checklist)
                    glTranslate(0,0,1)
                    glEnable(GL_LIGHTING)
                    curmat.apply(glr)
                    glCallList(self.texturedQuadList)
                    glDisable(GL_TEXTURE_2D)
                    if inselection:
                        glCallList(self.selectedBorderList)
                else:
                    glCallList(self.checklist)
                    if inselection:
                        glCallList(self.selectedBorderList)
                    glEnable(GL_LIGHTING)
                    curmat.apply(glr)
                    if colindex == cursorselection:
                        glScalef(1.3,1.3,1.3)
                    elif inselection:
                        glScalef(1.2,1.2,1.2)
                    glCallList(self.spherelist)
                glPopMatrix()
                glColor3f(0,0,0)
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL)
                self.renderText(2*self.unitsize*i,2*self.unitsize*(j+1),4*self.unitsize,str(colindex))
                colindex += 1
            glPushMatrix()
            glTranslate(self.unitsize+2*self.unitsize*i,self.unitsize+2*self.unitsize*nbritem,0)
            glCallList(self.checklist)
            glPopMatrix()

    def getNbColRow(self,w,h):
        nbcol = (w / (2*self.unitsize)) +1
        nbrow, rest = divmod(h ,2*self.unitsize)
        if rest > self.unitsize:
            nbrow+=1
        if nbrow == 0: nbrow = 1
        return nbcol,nbrow
    def isInSelection(self,i):
        if self.selectionend is None: return False
        if self.selectionbegin is None: return False
        if self.selectionbegin <= i <= self.selectionend : return True
        if self.selectionend <= i <= self.selectionbegin : return True
        return False
    def lenSelection(self):
        if self.selectionend is None: return 0
        if self.selectionbegin is None: return 0
        return abs(self.selectionend-self.selectionbegin)+1
    def setSelection(self, beg, end = None):
        self.selectionbegin = beg
        if end is None:
            self.selectionend = beg
        else:
            self.selectionend = end
    def getSelection(self):
        return range(self.selectionbegin,self.selectionend+1)
    def selectedColor(self,x,y):
        w = self.width()
        h = self.height()
        nbcol,nbrow = self.getNbColRow(w,h)
        nbcolselection = x // (2*self.unitsize)
        nbrowselection = y // (2*self.unitsize)
        if nbcolselection >= nbcol or nbrowselection >= nbrow : return -1        
        id = nbcolselection*nbrow + nbrowselection    
        return id     
    def positionColor(self,id):        
        w = self.width()
        h = self.height()
        nbcol,nbrow = self.getNbColRow(w,h)
        idcol, idrow = divmod(id, nbrow)
        return (idcol * (2*self.unitsize),idrow * (2*self.unitsize))
    def mouseDoubleClickEvent(self,event):
        x,y = event.pos().x(),event.pos().y()
        self.edition(self.selectedColor(x,y))
        self.initpos = None
        event.accept()
        self.updateGL()
    def mousePressEvent(self,event):
      if self.preview and self.preview.isVisible():
           self.preview.hide()
      if event.button()  == Qt.LeftButton:
        self.initpos = event.pos()
        self.mousepos = self.initpos
        x,y = event.pos().x(),event.pos().y()
        if event.modifiers() & Qt.ShiftModifier :
            self.selectionend = self.selectedColor(x,y)
        else:
            self.setSelection(self.selectedColor(x,y))
        self.updateGL()

    def mouseReleaseEvent(self,event):
      if event.button()  == Qt.LeftButton:
        x,y = event.pos().x(),event.pos().y()

        if event.modifiers() & Qt.ShiftModifier :
            self.selectionend = self.selectedColor(x,y)
        else:
            initialselection = self.selectionbegin
            self.setSelection(self.selectedColor(x,y))

        if self.selectionbegin == self.selectionend:
            if initialselection != self.selectionbegin:
                self.showMessage("Swap colors "+str(initialselection)+" and "+str(self.selectionbegin),2000) 
                self.swapMaterial(initialselection,self.selectionbegin)
                self.valueChanged.emit()
            else:
                self.showMessage("Click on color "+str(self.selectionbegin),2000) 

        else:
            self.selectionbegin, self.selectionend = min(self.selectionbegin, self.selectionend),max(self.selectionbegin, self.selectionend)
            self.showMessage("Selected colors from "+str(self.selectionbegin)+' to '+str(self.selectionend),2000)        
        self.initpos = None
        self.updateGL()

    def mouseMoveEvent(self,event):  
        self.mousepos = event.pos()
        lastcursorselection = self.cursorselection
        self.cursorselection = self.selectedColor(self.mousepos.x(),self.mousepos.y())
        if self.cursorselection < 0 or not self.geometry().contains(self.mousepos):
            self.cursorselection = lastcursorselection
        if event.buttons()  == Qt.LeftButton :
            if self.previewtrigger.isActive():
                self.previewtrigger.stop()
            if event.modifiers() & Qt.ShiftModifier :
                self.selectionend = self.cursorselection
            # else:
            #     if self.cursorselection != lastcursorselection and not lastcursorselection is None:
            #         p1 = QPoint(*self.positionColor(lastcursorselection))
            #         p2 = QPoint(*self.positionColor(self.cursorselection))
            #         self.initpos = self.initpos - p1 + p2 
            #         self.setSelection(self.cursorselection)
        elif event.buttons()  == Qt.NoButton and not self.cursorselection is None and self.cursorselection >= 0:
            cmat = self.getMaterial(self.cursorselection)
            if cmat.isTexture():
                if lastcursorselection != self.cursorselection or (self.preview is None and not self.previewtrigger.isActive()):                    
                    self.setToolTip('Texture '+str(self.cursorselection)+' : "'+cmat.image.filename+'"')
                    if not self.preview is None:
                        self.preview.hide()
                    self.previewselection = self.cursorselection
                    self.previewtrigger.start()
                elif self.preview and self.preview.isVisible():
                    self.preview.move(event.globalPos()+QPoint(2,2))
            else:
                if lastcursorselection != self.cursorselection:
                    self.setToolTip('Color '+str(self.cursorselection))
                if not self.preview is None and self.preview.isVisible():
                    self.preview.hide()
                if self.previewtrigger.isActive():
                    self.previewtrigger.stop()
        self.showMessage("Mouse on color "+str(self.cursorselection),2000) 
        self.updateGL()
    def displayPreview(self):
        self.preview = QSplashScreen(self,QPixmap(self.getMaterial(self.previewselection).image.filename))
        self.preview.move(QCursor.pos()+QPoint(2,2))
        self.preview.show()
    def leaveEvent(self,event):
        if not self.geometry().contains(self.mapFromGlobal(QCursor.pos())):
            self.mousepos = None
            self.initpos = None
            self.setMouseTracking(False)
            self.updateGL()
            if self.preview and self.preview.isVisible() :
                self.preview.hide()
            if self.previewtrigger.isActive():
                self.previewtrigger.stop()
        QGLWidget.leaveEvent(self,event)
    def enterEvent(self,event):
        self.mousepos = None
        self.setMouseTracking(True)
        QGLWidget.enterEvent(self,event)
    def showMessage(self,msg,timeout):
        if hasattr(self,'statusBar'):
            self.statusBar.showMessage(msg,timeout)
    def edition(self,id):
        self.showMessage("Edit color = "+str(id),2000)
        if id >= 0:
            color = self.getMaterial(id)
            if not color.isTexture():
                try:
                    res = editMaterialInDialog(color) # Warning Here! QLayout problem.
                    if res is None or res == QDialog.Accepted:
                        self.valueChanged.emit()
                except Exception as e:
                    print(e)
                    print('editMaterialInDialog not supported by your version of PlantGL')
            else:
                self.edittexture(color.image.filename)     
    def contextMenuEvent(self,event):
        self.menuselection = self.selectedColor(event.x(),event.y())
        if self.lenSelection() > 0:
            if not self.isInSelection(self.menuselection) :
                self.setSelection(self.menuselection)
                self.updateGL()
        else:
            self.setSelection(self.menuselection)
            self.updateGL()
        menu = QMenu("Color Edit",self)
        menu.addAction("Copy",self.copymaterial)        
        menu.addAction("Cut",self.cutmaterial)        
        action = menu.addAction("Paste",self.pastematerial)
        if self.clipboard is None:
            action.setEnabled(False)
        action = menu.addAction("Load texture",self.loadtexture)
        if self.lenSelection() > 1 :
            action.setEnabled(False)
        if self.lenSelection() == 1 and self.getMaterial(self.getSelection()[0]).isTexture():
            menu.addAction("Open repository", self.opentexturerepository)
        menu.addAction("Remove",self.removematerial)
        action = menu.addAction("Interpolate",self.interpolatematerial)
        if self.lenSelection() < 3 :
            action.setEnabled(False)
        menu.exec_(event.globalPos())
    def opentexturerepository(self):
        import os, sys
        cmat = self.getMaterial(self.getSelection()[0])

        fname = os.path.abspath(cmat.image.filename)
        mdir = os.path.dirname(fname)
        if sys.platform == 'win32':
                import subprocess
                subprocess.call('explorer /select,"'+fname+'"')
        elif sys.platform == 'linux2':
                os.system('xdg-open "'+mdir+'"')
        else:
                os.system('open "'+mdir+'"')
    def copymaterial(self):
        self.clipboard = [(i,self.getMaterial(i)) for i in self.getSelection()]
        self.cutaction = False
        self.menuselection = None
    def cutmaterial(self):
        self.clipboard = [(i,self.getMaterial(i)) for i in self.getSelection()]
        self.cutaction = True
        self.menuselection = None
    def pastematerial(self):
        if self.clipboard is None or not self.lenSelection() in [1,len(self.clipboard)] :
            QMessageBox.warning(self,'Copy error','Cannot copy materials ! Source and target are inconsistents')
        else:
            if self.lenSelection() == 1:
                sel = self.selectionbegin
                if self.cutaction == True:
                    sourceindices = [i[0] for i in  self.clipboard]
                    if sel in sourceindices:
                        sel = min(sourceindices)
                        sourceindices.pop(sourceindices.index(sel))
                    for i in reversed(sourceindices):
                        self.delMaterial(i)
                        if i < sel:
                            sel -= 1
                print('Copy ',self.clipboard[0][0],'to',sel)
                self.setMaterial(sel,self.clipboard[0][1].deepcopy())
                for i,color in reversed(self.clipboard[1:]):
                    self.insertMaterial(sel+1,color.deepcopy())
                self.setSelection(sel,sel+len(self.clipboard)-1)
            else:
                for color,pos in zip(self.clipboard,self.getSelection()):
                    self.setMaterial(pos,color[1].deepcopy())
                if self.cutaction == True:
                    iminus = 0
                    for pos, color in reversed(self.clipboard):
                        if not self.isInSelection(pos):
                            self.delMaterial(pos)
                            if pos < self.selectionbegin:
                                iminus -= 1
                    self.setSelection(self.selectionbegin-iminus,self.selectionend-iminus)
            self.cutaction = None
            self.clipboard = None
            self.valueChanged.emit()
        self.menuselection = None
        self.updateGL()
    def loadtexture(self):
        self.edittexture(self.menuselection)
    def edittexture(self,i,initialfile = None):
        fname, ffilter = QFileDialog.getOpenFileName(self,"Texture Selection",self.lastTextureDir if initialfile is None else initialfile,"All files (*.*)")
        if len(fname) == 0: return
        self.lastTextureDir = os.path.dirname(str(fname))
        format = QImageReader.imageFormat(fname)
        if len(format) == 0: 
            QMessageBox.warning(self,"Format error!","Format not supported !")
            self.edittexture(i,initialfile)
        else : 
            self.setMaterial(i,ImageTexture(str(fname)))
            self.valueChanged.emit()
    def removematerial(self):
        if not self.menuselection is None and self.nbMaterial() > self.menuselection:
            if not self.selectionend is None :
                for i in self.getSelection():
                    self.delMaterial(self.selectionbegin)
                self.selectionbegin = None
                self.selectionend = None
            else:
                self.delMaterial(self.menuselection)
            self.valueChanged.emit()
    def interpolatematerial(self):
        if not self.selectionend is None :
            beg = self.selectionbegin
            end = self.selectionend
            self.showMessage("Interpolate colors from "+str(beg)+" to "+str(end),2000)
            deltaselection = end - beg
            ratio = 1/float(deltaselection)
            fmat = self.getMaterial(beg)
            lmat = self.getMaterial(end)
            iratio = 0
            for i in range(beg+1,end):
                iratio += ratio
                self.setMaterial(i,fmat.interpolate(lmat,iratio))
            self.selectionbegin,self.selectionend = None,None
            self.valueChanged.emit()
            
    def dragEnterEvent(self,event):
        event.acceptProposedAction()

    def dropEvent(self,event):
        if event.mimeData().hasUrls() :
            self.fileDropEvent(self.selectedColor(event.pos().x(),event.pos().y()),str(event.mimeData().urls()[0].toLocalFile()))

    def fileDropEvent(self,pos,fname):
        from openalea.lpy.cpfgcompat.data_import import import_colormap, import_materialmap
        base , ext = os.path.splitext(fname)
        if ext == '.map' or ext == '.mat':
            if ext == '.map':
                newcolors = import_colormap(fname)
            else:
                newcolors = import_materialmap(fname)
            if len(newcolors) > 0:
                self.setMaterials(newcolors)
            else:
                QMessageBox.warning(self,'Data Error','Read no data from file "'+os.path.basename(fname)+'"')
        else:
            format = QImageReader.imageFormat(fname)
            if len(format) != 0: 
                self.setMaterial(pos,ImageTexture(str(fname)))
                self.valueChanged.emit()            

class MaterialPanelWidget(QWidget):
    valueChanged = pyqtSignal()
    def __init__(self,parent):
        QWidget.__init__(self,parent)
        self.setObjectName("materialPanelContents")
        self.verticalLayout = QVBoxLayout(self)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setContentsMargins(0,0,0,0)
        self.verticalLayout.setObjectName("materialPanelLayout")
        
        self.materialpanel = QScrollArea(self)
        self.view = MaterialPanelView(self.materialpanel)
        self.materialpanel.setWidget(self.view)
        self.materialpanel.setWidgetResizable(True)
        self.materialpanel.setObjectName("materialPanelArea")        
        self.verticalLayout.addWidget(self.materialpanel)
        
        self.view.valueChanged.connect(self.__updateStatus)
    
    def __updateStatus(self):
        self.valueChanged.emit()
    
    def setTurtle(self,turtle):
        self.view.setTurtle(turtle)

    def getTurtle(self):
        return self.view.getTurtle()

    def setAppearanceList(self, matlist):
        self.view.setAppearanceList(matlist)

    def getAppearanceList(self):
        return self.view.getAppearanceList()

MaterialEditor = MaterialPanelWidget

class MaterialPanelDock (QDockWidget):
    valueChanged = pyqtSignal()
    def __init__(self,parent,name = None):
        QDockWidget.__init__(self,parent)
        if name:
            self.setObjectName(name.replace(' ','_'))
        self.setWindowTitle('Color Map')
        self.view = MaterialPanelWidget(self)        
        self.setWidget(self.view)
        self.view.valueChanged.connect(self.__updateStatus)
    
    def setStatusBar(self,st):
        self.view.statusBar = st

    def showMessage(self,msg,timeout):
        if hasattr(self,'statusBar'):
            self.statusBar.showMessage(msg,timeout)
        else:
            print(msg)
    
    def __updateStatus(self):
        self.valueChanged.emit()

        
if __name__ == '__main__':
    qapp = QApplication([])
    m = MaterialPanelDock(None)
    m.show()
    qapp.exec_()
    

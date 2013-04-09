from openalea.vpltk.qt import QtGui, QtCore
import openalea.plantgl.all as pgl
import code

class MyThread(QtCore.QThread):
    def __init__(self):
        QtCore.QThread.__init__(self)
        self.mutex = QtCore.QMutex()
        self.waitcondition = QtCore.QWaitCondition()
        self.endmutex = QtCore.QMutex()
        self.endwaitcondition = QtCore.QWaitCondition()
        
    def start(self):
        self.mutex.lock()
        QtCore.QThread.start(self)
        self.waitcondition.wait(self.mutex)
        self.mutex.unlock()
    
    def waitEnd(self):
        self.endwaitcondition.wait(self.endmutex)
    
    def run(self):
        app = QtGui.QApplication([])
        while not self.mutex.tryLock():
            pass
        self.mutex.unlock()
        self.waitcondition.wakeAll()
        pgl.Viewer.threaded = False
        pgl.Viewer.start()
        print "execute"
        QtCore.QThread.exec_(self)
        self.endwaitcondition.wakeAll()

    def doit(self):
        self.start()
        self.endwaitcondition.wait(self.endmutex,2000)
        Viewer.update()
        self.waitEnd()
        while self.isRunning():
            pass

# m = MyThread()
# m.doit()

class MyWidget(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)
        self.mutex = QtCore.QMutex()
        self.waitcondition = QtCore.QWaitCondition()
    def customEvent(self,event):
        print "Receive Event"
        while not self.mutex.tryLock():
            pass
        self.mutex.unlock()
        self.waitcondition.wakeAll()        
    def sendAnEvent(self,event = QEvent(QEvent.User)):
        self.mutex.lock()
        QtGui.QApplication.postEvent(self,event)
        self.waitcondition.wait(self.mutex)
        self.mutex.unlock()

def test():
	pgl.Viewer.display(pgl.Sphere())

class MyThread2(QtCore.QThread):
    def __init__(self):
        QtCore.QThread.__init__(self)
    
    def run(self):
        code.interact("Sub thread interpretation",local=globals())
        QtGui.QApplication.quit()
    def doit(self):
        app = QtGui.QApplication([])
        # self.widget = MyWidget()
        # self.widget.show()
        pgl.Viewer.start()
        self.start()
        app.exec_()

m = MyThread2()
m.doit()

class MyThread3(QtCore.QThread):
    def __init__(self):
        QtCore.QThread.__init__(self)
        self.mutex = QtCore.QMutex()
        self.waitcondition = QtCore.QWaitCondition()
    def run(self):
        app = QtGui.QApplication([])
        while not self.mutex.tryLock():
            pass
        self.mutex.unlock()
        self.waitcondition.wakeAll()
        print 'app.exec()'
        app.exec_()
    def start(self):
        self.mutex.lock()
        QtCore.QThread.start(self)
        self.waitcondition.wait(self.mutex)
        self.mutex.unlock()
    def doit(self):
        self.start()
        pgl.Viewer.start()

m = MyThread3()
m.doit()

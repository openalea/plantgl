cd ~/.local
mkdir lib bin include  
cd lib
mkdir python2.7 python2.7/site-packages


cd ~
mkdir Develop
cd Develop
git clone https://github.com/openalea/deploy.git
git clone https://github.com/openalea/sconsx.git
cd deploy
python setup.py develop --user
cd ../sconsx
python setup.py develop --user
cd ..

git clone https://github.com/VirtualPlants/plantgl.git
cd plantgl

sudo apt-get install libqt4-dev bison flex libboost-dev  
sudo apt-get install libeigen3-dev libcgal-dev  libcgal-qt4-dev libann-dev libqhull-dev 
python setup.py develop --user

"""
cgal_includes  = '/usr/include'
cgal_libpath   = '/usr/lib'
eigen_includes = '/usr/include/eigen3'

"""
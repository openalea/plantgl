if [ `uname` = "Darwin" ];  then 
    cd /opt 
    wget https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.10.sdk.tar.xz
    tar xvzf MacOSX10.10.sdk.tar.xz
    cd -
fi
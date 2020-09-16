if [ `uname` = "Darwin" ];  then 
    wget -nv https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.9.sdk.tar.xz
    tar xzf MacOSX10.9.sdk.tar.xz
    echo "CONDA_BUILD_SYSROOT:
 - `pwd`/MacOSX10.9.sdk
 " > conda/conda_build_config.yaml
    more conda/conda_build_config.yaml
fi
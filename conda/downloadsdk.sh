if [ `uname` = "Darwin" ];  then 
    wget https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.10.sdk.tar.xz
    tar xvzf MacOSX10.10.sdk.tar.xz
    echo "CONDA_BUILD_SYSROOT:\n - `pwd`/MacOSX10.10.sdk        # [osx]" > conda/conda_build_config.yaml
fi
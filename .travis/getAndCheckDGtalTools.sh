## Get and test if DGtalTools compiles in default mode
pwd
git clone --depth 1 git://github.com/DGtal-team/DGtalTools.git
cd DGtalTools
mkdir build ; cd build
cmake .. -DDGtal_DIR=/home/travis/build/dcoeurjo/DGtal
make -j 2
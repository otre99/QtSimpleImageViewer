QT_VERSION=6.9.2

export QT_QMAKE_EXECUTABLE=/home/programs/QtSDK/${QT_VERSION}/gcc_64/bin/qmake
export QT_DIR=/home/programs/QtSDK/${QT_VERSION}/gcc_64/lib/cmake/Qt6
export CMAKE_PREFIX_PATH=/home/programs/QtSDK/${QT_VERSION}/gcc_64
export PATH=/home/programs/QtSDK/${QT_VERSION}/gcc_64/bin/:$PATH

CURR_DIR=`pwd`

mkdir -p ${CURR_DIR}/AppBinaries/build_linux && cd ${CURR_DIR}/AppBinaries/build_linux 
qt-cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${LINUX_DEPLOY} ${CURR_DIR}/src
make -j6 


cd ${CURR_DIR}
APP_NAME=ImageViewer
/home/programs/APPS/linuxdeploy-x86_64.AppImage --appdir appDir -e ${CURR_DIR}/AppBinaries/build_linux/${APP_NAME} -d src/resources/${APP_NAME}.desktop -i src/resources/${APP_NAME}.png --plugin qt --output appimage



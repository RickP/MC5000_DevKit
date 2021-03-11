cd ..
mkdir images
mkdir build
cd build
qmake ..
make
QML_SOURCES_PATHS=.. linuxdeploy --plugin qt --appdir AppDir -d ../imagebuilder/MC5000.desktop -e MC5000-GUI -i ../icons/icon.png
appimagetool AppDir
mv *.AppImage ../images/
cd ../imagebuilder
rm -r ../build

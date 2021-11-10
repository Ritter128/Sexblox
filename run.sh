./upd.sh
./loaddepends.sh
mkdir build
cmake -S . -B build
cp -r src/Textures build/
cd build
make
./SEXBLOX

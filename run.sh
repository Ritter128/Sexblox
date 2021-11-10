./upd.sh
cmake -S . -B build
cp -r src/Textures build/
cd build
make
./SEXBLOX

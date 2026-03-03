Baixar o GLFW
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local -DBUILD_SHARED_LIBS=OFF
make -j$(nproc)
make install

baixar o glew:
wget https://github.com/nigels-com/glew/releases/download/glew-2.3.1/glew-2.3.1.tgz
tar -xzf glew-2.3.1.tgz
cd glew-2.3.1

# Compila
make

# Copia na mão para a home
mkdir -p $HOME/.local/lib
mkdir -p $HOME/.local/include/GL

cp lib/libGLEW.a $HOME/.local/lib/
cp include/GL/glew.h $HOME/.local/include/GL/
cp include/GL/glxew.h $HOME/.local/include/GL/

baixar o glm:
git clone https://github.com/g-truc/glm.git
cp -r glm/glm /usr/local/include/
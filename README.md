# 0.добавить проект на git (только для разработчика)
git init

git add .gitignore

git add .

git commit -m "commit"

git branch -M main

git remote add origin https://github.com/spectraldv/cpp_qt_cmake.git

git push -u origin main

git push

git status

git rm --cached *.exe

# 1.скачать проект с git

git clone https://github.com/spectraldv/cpp_qt_cmake.git

(1.1. и 1.2. только если потребуется)

1.1.обновить проект с git(удалив все свои изменения)

git fetch --all

git reset --hard

git pull

(git pull origin main)

1.2.добавить свою ветку с git

git checkout -b ubuntu

# 2. build for Linux

//openGL

sudo apt update

//sudo apt install libgl sdl

Дистрибутивы на основе Debian/Ubuntu

sudo apt install mesa-utils build-essential libgl1-mesa-dev

Системы на основе Arch Linux

sudo pacman -S mesa mesa-utils base-devel

Red Hat/Fedora/CentOS

sudo dnf install mesa-libGL mesa-libGL-devel mesa-utils


//qt5

sudo apt-get install qtbase5-dev

sudo apt-get install qtdeclarative5-dev

qtchooser -print-env       

qmake -v                                   


//qt6

sudo apt install qt6-base-dev

//cmake .

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

make

./main

# 3. source qt for Windows (not build)

https://www.qt.io/development/offline-installers

./configure -prefix %CD%\qtbase

//./configure -prefix C:\Qt\Qt6 -confirm-license -nomake examples -nomake tests

cmake --install . --prefix C:/qtbase

//mingw32-make  # или nmake для MSVC

//mingw32-make install  # или nmake install


//not found bin/syncqt.exe 

# 4. aqt qt for Windows and build

Установите aqtinstall

pip install aqtinstall

Скачайте готовую Qt 6.5.3 LTS (стабильная версия)

aqt install-qt windows desktop 6.5.3 win64_mingw -O C:/Qt

Или Qt 6.7.2 (более новая)

aqt install-qt windows desktop 6.7.2 win64_mingw -O C:/Qt

//build

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.7.2/mingw_64

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.7.2/mingw_64 -DCMAKE_BUILD_TYPE=Release

cmake --build build --target install

cmake --build build --config Release --target install


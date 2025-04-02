# Syft4Golog

## Dependencies

You need to install the following dependencies:

```
sudo apt install libgmp-dev tcl8.6-dev tk8.6-dev
```

The project depends on gologpp, see https://github.com/MASKOR/gologpp 

In turn, gologpp depends on several dependencies, see https://github.com/MASKOR/gologpp/wiki

The project also depends on LydiaSyft, see https://github.com/GianmarcoDIAG/LydiaSyft

### ECliPSe

```
git clone git://git.code.sf.net/p/eclipse-clp/git eclipse-clp-git
```

On Ubuntu 22.04 with 64-bits architecture, this should work:

```
export ECLIPSEARCH=x86_64_linux 
export ARCH=x86_64_linux 
export ECLIPSETHIRDPARTY=/vol/Eclipse/thirdparty
CONFIG_SITE=config.$ECLIPSEARCH
./configure
make -f Makefile.$ECLIPSEARCH
./RUNME
export PATH="{your/eclipse/path}:$PATH"
source ~/.bashrc
```

### Readylog

```
git clone git@github.com:MASKOR/readylog.git --recursive
```

Tell gologpp where to find readylog's interpreter

```
echo 'export READYLOG_PL=~/readylog/interpreter' >> ~/.bashrc
source ~/.bashrc
```

### LydiaSyft

```
git clone git@github.com:GianmarcoDIAG/LydiaSyft.git --recursive
```

Follow the instructions in their README.md to complete the installation

## Build

```
mkdir build && cd build
cmake .. && make -j4
```

If you get an error due to `-leclipse` you can fix it with

```
cp {your/eclipse/path}/lib/x_86_84/libeclipse.so /usr/local/lib/
```

## Execute

```
./GologExecutable
```















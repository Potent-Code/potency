#!/bin/sh

# if no arguments are given, we just want to build
if [ "$1" == "" ]; then
	echo "Building Potency..."
	bam $BAMFLAGS -a Mode=Debug
elif [ "$1" == "install" ]; then
	cp build/usr/local/lib/* /usr/local/lib/ &&
	cp build/usr/local/include/* /usr/local/include/ &&
	cp build/usr/local/bin/* /usr/local/bin/ &&
	echo "build/usr/local/lib/* -> /usr/local/lib/"
	echo "build/usr/local/include/* -> /usr/local/lib/"
	echo "build/usr/local/bin/* -> /usr/local/bin/"
elif [ "$1" == "clean" ]; then
	echo "removing build/"
	rm -fR build
else
	echo "Usage:"
	echo -e "\t$0\t\tBuild"
	echo -e "\t$0 install\tInstall to /usr/local/lib, /usr/local/include, and /usr/local/bin"
	echo -e "\t$0 clean\t\tClean build outputs"
fi

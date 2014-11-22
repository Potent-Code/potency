#!/bin/sh

# if no arguments are given, we just want to build
if [ "${1}" = "" ]; then
	echo "Building Potency..."
	bam ${BAMFLAGS} -a Mode=Debug
elif [ "${1}" = "install" ]; then
	cp build/usr/local/lib/* /usr/local/lib/ &&
	cp build/usr/local/include/* /usr/local/include/ &&
	cp build/usr/local/bin/* /usr/local/bin/ &&
	echo "build/usr/local/lib/* -> /usr/local/lib/"
	echo "build/usr/local/include/* -> /usr/local/lib/"
	echo "build/usr/local/bin/* -> /usr/local/bin/"
elif [ "${1}" = "uninstall" ]; then
	potency_libs=`find /usr/local/lib/ -name "libpotency*"`
	potency_bins=`find /usr/local/bin/ -name "potency_*"`
	if [ "${potency_libs}" != "" ]; then
		rm -f ${potency_libs}
		echo "Removing potency libraries...${potency_libs}"
	fi
	if [ "${potency_bins}" != "" ]; then
		rm -f ${potency_bins}
		echo "Removing potency binaries...${potency_bins}"
	fi
	rm -f /usr/local/include/potency.h
	echo "Removing potency header.../usr/local/include/potency.h"
elif [ "${1}" = "clean" ]; then
	echo "removing build/"
	rm -fR build
else
	echo "Usage:"
	echo -e "  ${0}\t\t\tBuild"
	echo -e "  ${0} install\t\tInstall to /usr/local/lib, /usr/local/include, and /usr/local/bin"
	echo -e "  ${0} uninstall\t\tUninstall potency"
	echo -e "  ${0} clean\t\tClean build outputs"
fi


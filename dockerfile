FROM ubuntu:24.04

ARG CURL_VERSION=7.62.0
ARG LLVM_VERSION=18
ARG XWIN_VERSION=0.6.5
ARG XWIN_TRIPLE=x86_64-unknown-linux-musl
ARG WINDOWS_CRT_VERSION=14.29.16.11
ARG WINDOWS_SDK_VERSION=10.0.22621


ADD "https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/xwin-${XWIN_VERSION}-${XWIN_TRIPLE}.tar.gz" \
	/tmp/xwin.tar.gz
RUN <<-EOF
	set -eux

	cd /tmp
	tar -xf xwin.tar.gz
	mv xwin-*/xwin /usr/local/bin/
	rm -r xwin.tar.gz xwin-*
EOF

RUN xwin --accept-license --arch x86 \
	--crt-version ${WINDOWS_CRT_VERSION} --sdk-version ${WINDOWS_SDK_VERSION} \
	splat --output /opt/xwin --include-debug-libs

RUN <<-EOF
	set -eux

	export DEBIAN_FRONTEND=noninteractive
	apt update
	apt install -y \
		build-essential cmake cppcheck gdb ninja-build valgrind \
		clang-${LLVM_VERSION} clang-tools-${LLVM_VERSION} \
		lld-${LLVM_VERSION} llvm-${LLVM_VERSION}
	apt autoremove -y
	apt clean -y
EOF

ENV LLVM_VERSION=${LLVM_VERSION}

ADD "https://gist.githubusercontent.com/19wintersp/084cd0d4810a25c11559107491525655/raw/31fce25c66837af5538857f1624ff7acb409d7a9/insensitive.c" \
	/tmp/insensitive.c
RUN <<-EOF
	set -eux

	mkdir /opt/xwin/bin
	cd /opt/xwin/bin

	cc -O0 -std=c2x -fPIC -shared -o libinsensitive.so /tmp/insensitive.c

	<<-EOF2 cat > cc
		#!/bin/env bash
		LD_PRELOAD=/opt/xwin/bin/libinsensitive.so \
			clang-cl-$LLVM_VERSION \
				-Wno-microsoft --target=i686-pc-windows-msvc /EHa /arch:SSE \
				/imsvc /opt/xwin/crt/include      /imsvc /opt/xwin/sdk/include/shared \
				/imsvc /opt/xwin/sdk/include/ucrt /imsvc /opt/xwin/sdk/include/um \
				\$@
	EOF2
	<<-EOF2 cat > lib
		#!/bin/env bash
		LD_PRELOAD= \
			lld-link-$LLVM_VERSION /lib \
				/libpath:/opt/xwin/crt/lib/x86      /libpath:/opt/xwin/sdk/lib/shared/x86 \
				/libpath:/opt/xwin/sdk/lib/ucrt/x86 /libpath:/opt/xwin/sdk/lib/um/x86 \
				\$@
	EOF2
	<<-EOF2 cat > link
		#!/bin/env bash
		LD_PRELOAD=/opt/xwin/bin/libinsensitive.so \
			lld-link-$LLVM_VERSION \
				/libpath:/opt/xwin/crt/lib/x86      /libpath:/opt/xwin/sdk/lib/shared/x86 \
				/libpath:/opt/xwin/sdk/lib/ucrt/x86 /libpath:/opt/xwin/sdk/lib/um/x86 \
				\$@
	EOF2
	<<-EOF2 cat > rc
		#!/bin/env bash
		LD_PRELOAD=/opt/xwin/bin/libinsensitive.so \
			llvm-rc-$LLVM_VERSION \
				/I /opt/xwin/crt/include      /I /opt/xwin/sdk/include/shared \
				/I /opt/xwin/sdk/include/ucrt /I /opt/xwin/sdk/include/um \
				\$@
	EOF2
	ln -s `which llvm-mt-$LLVM_VERSION` mt

	chmod u+x cc lib link rc

	<<-EOF2 cat > xwin.cmake
		set(CMAKE_SYSTEM_NAME Windows)

		set(CMAKE_AR           "/opt/xwin/bin/lib")
		set(CMAKE_C_COMPILER   "/opt/xwin/bin/cc")
		set(CMAKE_CXX_COMPILER "/opt/xwin/bin/cc")
		set(CMAKE_LINKER       "/opt/xwin/bin/link")
		set(CMAKE_MT           "/opt/xwin/bin/mt")
		set(CMAKE_RC_COMPILER  "/opt/xwin/bin/rc")
	EOF2
EOF

ENV TOOLCHAIN=/opt/xwin/bin/xwin.cmake

ADD "https://curl.se/download/curl-${CURL_VERSION}.tar.gz" \
	/tmp/curl.tar.gz
RUN <<-EOF
	set -eux

	cd /tmp
	tar -xf curl.tar.gz
	mv curl-* /opt/curl
EOF

RUN <<-EOF
	set -eux

	cd /opt/curl
	sed -ie "s/\\\\xa9/(c)/g" lib/libcurl.rc src/curl.rc # curl/curl#7765
	cmake --fresh \
		-DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
		-DBUILD_CURL_EXE=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF \
		-DCURL_CA_PATH=none -DCURL_USE_LIBPSL=OFF \
		-G Ninja -B build
	ninja -C build

	cd build/lib
	cp libcurl_imp.lib libcurl_a.lib
	cp libcurl_imp.lib libcurl_a_debug.lib
EOF

ENV CURL_INCLUDEDIR=/opt/curl/include/
ENV CURL_LIBRARYDIR=/opt/curl/build/lib/
ENV CURL_DEBUG_LIBRARYDIR=/opt/curl/build/lib/

CMD ["/bin/bash"]

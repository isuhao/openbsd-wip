# $OpenBSD$

ONLY_FOR_ARCHS =	${GCC4_ARCHS}
BROKEN-powerpc =	never built - gcc ICE

.include "Makefile.version"

VERSION ?=		${QT5_VERSION}
DIST_VERSION ?=		${QT5_DIST_VERSION}
DISTNAME ?=		${QT5NAME:L}-opensource-src-${DIST_VERSION}

PKGNAME ?=		${DISTNAME:C/-opensource-src//:C/-${DIST_VERSION}/-${VERSION}/}
PKGNAME-main ?=		${PKGNAME}
PKGNAME-examples ?=	${PKGNAME:C/(-[0-9])/-examples\1/}
PKGNAME-html ?=		${PKGNAME:C/(-[0-9])/-html\1/}
PKGNAME-qch ?=		${PKGNAME:C/(-[0-9])/-qch\1/}

COMMENT-examples ?=	${QT5NAME} development examples
COMMENT-html ?=		HTML documentation for ${QT5NAME}
COMMENT-qch ?=		qdoc-compiled documentation for ${QT5NAME}

PKG_ARCH-html =		*
PKG_ARCH-qch =		*

DPB_PROPERTIES =	parallel nojunk

CATEGORIES +=		x11 x11/qt5 devel
HOMEPAGE ?=		http://www.qt-project.org/
MAINTAINER ?=		KDE porting team <openbsd-kde@googlegroups.com>

WANTLIB-html =
LIB_DEPENDS-html =
RUN_DEPENDS-html ?=

WANTLIB-qch =
LIB_DEPENDS-qch =
RUN_DEPENDS-qch ?=

NEXT_VERSION ?=		${QT5_NEXT_VERSION}

.if defined(MULTI_PACKAGES)
. if ${MULTI_PACKAGES:M-main}
RUN_DEPENDS-qch +=	${BASE_PKGPATH},-main>=${VERSION:R},<${NEXT_VERSION}
RUN_DEPENDS-html +=	${BASE_PKGPATH},-main>=${VERSION:R},<${NEXT_VERSION}

# XXX this will be wrong for Enginio
RUN_DEPENDS-examples ?=	${RUN_DEPENDS}
RUN_DEPENDS-examples +=	${BASE_PKGPATH},-main>=${VERSION:R},<${NEXT_VERSION}
LIB_DEPENDS-examples ?=	${LIB_DEPENDS}
LIB_DEPENDS-examples +=	${BASE_PKGPATH},-main>=${VERSION:R},<${NEXT_VERSION}
. endif
.endif

# Mostly LGPLv2.1 or LGPLv3 for code; FDLv1.3 for documentation.
# Some third-party parts are BSD-licensed.
# Also, many parts have are dual-licensed having either commercial, GPL,
# Apache 2.0 or other type of license as an alternative option.
# A few components, including QtWebEngine, are LGPLv3 only, no LGPLv2.1.
PERMIT_PACKAGE_CDROM ?=	Yes

MASTER_SITES ?=		http://download.qt.io/official_releases/qt/${VERSION:R}/${DIST_VERSION}/submodules/
DIST_SUBDIR ?=		qt

CONFIGURE_STYLE ?=	qmake
MODULES +=		devel/qmake x11/qt5

DESTDIRNAME =		INSTALL_ROOT

MAKE_ENV +=		REAL_BUILD_ROOT=${WRKBUILD}

MAKE_FLAGS =		-B -j ${MAKE_JOBS} \
			PYTHON=${MODPY_BIN} \
			RUBY=${RUBY}
.for _l _v in ${SHARED_LIBS}
  MAKE_FLAGS +=		LIB${_l}_VERSION=${_v}
.endfor

# qmake supports so-called "shadow" builds: it's like normal
# out-of-source build but build directory is required to be located
# under source root.
SEPARATE_BUILD ?=	Yes
WRKBUILD =		${WRKSRC}/build-${ARCH}

# readability macros
QT_BASEDIR =	${PREFIX}/lib/qt5
QT_INCDIR =	${PREFIX}/include/X11/qt5
QT_EXAMPLES =	${QT_BASEDIR}/examples
QT_DOC =	${PREFIX}/share/doc/qt5
QT_PLUGINSDIR =	${QT_BASEDIR}/plugins
QT_BINDIR =	${QT_BASEDIR}/bin
QT_PKGCFGDIR =	${QT_BASEDIR}/pkgconfig
QT_CMAKEDIR =	${QT_BASEDIR}/cmake

CONFIGURE_ENV +=	MAKE=make \
			MAKEFLAGS="-j ${MAKE_JOBS}" \
			EXTRA_CPPFLAGS="-DLOCALBASE=\\\"${LOCALBASE}\\\" -DX11BASE=\\\"${X11BASE}\\\""

.if ${MACHINE_ARCH} == "powerpc"
CONFIGURE_ENV += LDFLAGS="-Wl,--relax"
.endif

# QtWebkit (at least, maybe others) plays dirty games with inter-target
# dependencies, so we'd better use the target qmake always creates
# first, named "first".
ALL_TARGET ?=		first
#ALL_TARGET ?=		all
FAKE_TARGET ?=		install
TEST_TARGET ?=		check

.include <bsd.port.arch.mk>

FLAVOR ?=

# Force re-running qmake before generating documentation
# to allow qdoc and qhelpgenerator do their work.
#.if ${BUILD_PACKAGES:M-html} || ${BUILD_PACKAGES:M-qch}
#ALL_TARGET +=		qmake_all
#.endif

.if ${BUILD_PACKAGES:M-html}
ALL_TARGET +=		html_docs
FAKE_TARGET +=		install_html_docs
.endif

.if ${BUILD_PACKAGES:M-qch}
ALL_TARGET +=		qch_docs
FAKE_TARGET +=		install_qch_docs
.endif

.if ${CONFIGURE_STYLE:Mqmake}
. if !${BUILD_PACKAGES:M-examples}
MODQMAKE_ARGS +=	CONFIG-=examples
. endif
.endif

.if ${FLAVOR:Mno_tests}
NO_TEST =		Yes
.endif

SUBST_VARS +=		VERSION

DESTDIRNAME =	INSTALL_ROOT
WRKDIST ?=	${WRKDIR}/${DISTNAME:C/${DIST_VERSION}/${VERSION}/}

PROGRAMS5 =	assistant designer linguist lrelease lupdate moc pixeltool \
		qcollectiongenerator qdbus qdoc qmake qml qtdiag rcc uic

.PHONY: qt5-post-install

qt5-post-install:
	# programs
.for p in ${PROGRAMS5}
	! test -e ${PREFIX}/lib/qt5/bin/$p || \
	    ln -sf ../lib/qt5/bin/$p ${PREFIX}/bin/$p-qt5
.endfor
	# shared libraries
.for l v in ${SHARED_LIBS}
	cd ${PREFIX}/lib; \
	f=qt5/lib$l.so.$v; \
	! test -f $$f || ln -sf $$f .
.endfor
.for l v in ${SHARED_LIBS-examples}
	cd ${PREFIX}/lib; \
	f=qt5/examples/*/*/lib$l.so.$v; \
	! test -f $$f || ln -sf $$f .
.endfor
	# pkg-config files
	@if [ -d ${QT_PKGCFGDIR} ]; then \
		for f in ${QT_PKGCFGDIR}/*; do \
			echo $${f##*/}; \
			perl -p -e 's@ -L$${WRKBUILD}\S+@@g;' \
			        -e 's@ -Wl,-whole-archive.*@@;' \
			    $$f >${PREFIX}/lib/pkgconfig/$${f##*/}; \
		done; \
	fi
	# CMake files get values from QMake config "as is"
	find ${QT_CMAKEDIR} -type f -print0 | xargs -0r perl -pi \
		-e 's,\$$\(LOCALBASE\),${LOCALBASE},g;' \
		-e 's,\$$\(X11BASE\),${X11BASE},g;'
	# remove unpatched files
	-find ${QT_BASEDIR}/mkspecs -name '*${PATCHORIG}' | xargs -r rm

post-install: qt5-post-install

# Generates DESCR-* from DESCR-main, and PLIST-* from PLIST-main.
# Don't forget to run "rm pkg/PLIST-*; make plist" before this.
gen_pkg:
	test -e ${PKGDIR}/PLIST-main
	test -e ${PKGDIR}/DESCR-main
.if ${BUILD_PACKAGES:M-examples}
	echo '@comment $$OpenBSD$$' >${PKGDIR}/PLIST-examples
	egrep '^(@.* )?lib/qt5/examples/' ${PKGDIR}/PLIST-main >>${PKGDIR}/PLIST-examples
	sed -Ei '\,^(@.* )?lib/qt5/examples/,d' ${PKGDIR}/PLIST-main
	sed '$$s/.*/This package contains ${QT5NAME} compiled examples and their sources./' \
		<${PKGDIR}/DESCR-main >${PKGDIR}/DESCR-examples
.endif
# note that -qch should be handled before -html here
.if ${BUILD_PACKAGES:M-qch}
	echo '@comment $$OpenBSD$$' >${PKGDIR}/PLIST-qch
	egrep '^(@.* )?share/doc/qt5/[^/]+\.qch?$$' ${PKGDIR}/PLIST-main >>${PKGDIR}/PLIST-qch
	sed -Ei '\,^(@.* )?share/doc/qt5/.*\.qch,d' ${PKGDIR}/PLIST-main
	sed '$$s/.*/This package contains compiled ${QT5NAME} documentation./' \
		<${PKGDIR}/DESCR-main >${PKGDIR}/DESCR-qch
.endif
.if ${BUILD_PACKAGES:M-html}
	echo '@comment $$OpenBSD$$' >${PKGDIR}/PLIST-html
	egrep '^(@.* )?share/doc/qt5/' ${PKGDIR}/PLIST-main >>${PKGDIR}/PLIST-html
	sed -Ei '\,^(@.* )?share/doc/qt5/,d' ${PKGDIR}/PLIST-main
	sed '$$s/.*/This package contains ${QT5NAME} HTML documentation./' \
		<${PKGDIR}/DESCR-main >${PKGDIR}/DESCR-html
.endif

show_deps:
	@find ${WRKSRC} -name '*.pr[iof]' -print0 | \
		xargs -0r egrep -A 2 '\<(qtHaveModule|QT_CONFIG)\>'

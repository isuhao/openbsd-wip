# $OpenBSD: Makefile.template,v 1.72 2015/04/08 17:53:19 sthen Exp $

COMMENT =		image processing and analysis library

GH_ACCOUNT =		ukoethe
GH_PROJECT =		vigra
GH_TAGNAME = 		Version-1-11-0
DISTNAME =		vigra-1.11.0

SHARED_LIBS =		vigraimpex 0.0 # 5.1

CATEGORIES =		graphics

HOMEPAGE =		http://ukoethe.github.io/vigra/

MAINTAINER =		Kirill Bychkov <kirby@openbsd.org>

# MIT
PERMIT_PACKAGE_CDROM = 	Yes

WANTLIB += Half Iex IlmImf IlmThread Imath boost_atomic-mt boost_chrono-mt
WANTLIB += boost_date_time-mt boost_system-mt boost_thread-mt
WANTLIB += fftw3 jpeg m png pthread tiff z

MODULES =		devel/cmake gcc4
MODGCC4_LANGS += 	c++
MODGCC4_ARCHS = 	*
LIB_DEPENDS =		devel/boost \
			graphics/jpeg \
			graphics/openexr \
			graphics/png \
			graphics/tiff \
			math/fftw3

CONFIGURE_ARGS +=	-DWITH_VIGRANUMPY=0 \
			-DDOCINSTALL=share/doc/ \
			-DWITH_BOOST_THREAD=1 \
			-DWITH_OPENEXR=1

.include <bsd.port.mk>

# $OpenBSD$

QT5NAME =		QtWebChannel
COMMENT-main =		Qt5 multimedia components

SHARED_LIBS =		Qt5WebChannel		1.0

MULTI_PACKAGES =	-main -examples -qch -html

WANTLIB-main =		${WANTLIB}
WANTLIB-main +=		GL Qt5Core Qt5Gui Qt5Network Qt5Qml Qt5Quick m pthread

WANTLIB-examples =	${WANTLIB}
WANTLIB-examples +=	GL Qt5Core Qt5Gui Qt5Network Qt5Qml Qt5WebChannel Qt5WebSockets
WANTLIB-examples +=	Qt5Widgets c m pthread

BUILD_DEPENDS =		x11/qt5/qtwebsockets>=${QT5_VERSION},<${QT5_NEXT_VERSION}
RUN_DEPENDS-main =	x11/qt5/qtwebsockets>=${QT5_VERSION},<${QT5_NEXT_VERSION}

LIB_DEPENDS =		x11/qt5/qtdeclarative>=${QT5_VERSION},<${QT5_NEXT_VERSION}

FLAVORS =		debug
PSEUDO_FLAVORS =	no_examples no_qch no_html
FLAVOR ?=

.include <bsd.port.mk>

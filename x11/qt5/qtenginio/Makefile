# $OpenBSD$

QT5NAME =		QtEnginio
VERSION =		${ENGINIO_VERSION}
DIST_VERSION =		${ENGINIO_VERSION}
NEXT_VERSION =		${ENGINIO_NEXT_VERSION}
COMMENT-main =		Qt5 multimedia components

MULTI_PACKAGES =	-main -examples -qch -html

SHARED_LIBS =		Enginio			2.0

WANTLIB-main =		${WANTLIB}
WANTLIB-main +=		Qt5Core Qt5Network Qt5Qml m pthread

WANTLIB-examples =	${WANTLIB}
WANTLIB-examples +=	Enginio GL Qt5Core Qt5Gui Qt5Network Qt5Qml Qt5Quick
WANTLIB-examples +=	Qt5Widgets c m pthread

LIB_DEPENDS =		x11/qt5/qtdeclarative>=${QT5_VERSION},<${QT5_NEXT_VERSION}

FLAVORS =		debug
PSEUDO_FLAVORS =	no_examples no_qch no_html
FLAVOR ?=

SUBST_VARS =		ENGINIO_VERSION

.include <bsd.port.mk>

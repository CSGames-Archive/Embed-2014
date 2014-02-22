INCLUDEPATH += $$PWD

unix {
    greaterThan(QT_MAJOR_VERSION, 4) {
        contains(QT_CONFIG, libudev) {
            DEFINES += LINK_LIBUDEV
            INCLUDEPATH += $$QMAKE_INCDIR_LIBUDEV
            LIBS_PRIVATE += $$QMAKE_LIBS_LIBUDEV
        }
    } else {
        packagesExist(libudev) {
            CONFIG += link_pkgconfig
            DEFINES += LINK_LIBUDEV
            PKGCONFIG += libudev
        }
    }
}

PUBLIC_HEADERS += \
    $$PWD/qserialportglobal.h \
    $$PWD/qserialport.h \
    $$PWD/qserialportinfo.h

PRIVATE_HEADERS += \
    $$PWD/qserialport_p.h \
    $$PWD/qserialportinfo_p.h

SOURCES += \
    $$PWD/qserialport.cpp \
    $$PWD/qserialportinfo.cpp

unix:!symbian {
    PRIVATE_HEADERS += \
        $$PWD/qserialport_unix_p.h

    SOURCES += \
        $$PWD/qserialport_unix.cpp \
        $$PWD/qserialportinfo_unix.cpp

    macx {
        SOURCES += $$PWD/qserialportinfo_mac.cpp

        LIBS_PRIVATE += -framework IOKit -framework CoreFoundation
    }
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

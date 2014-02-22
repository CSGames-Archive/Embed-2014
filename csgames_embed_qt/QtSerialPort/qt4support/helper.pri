unix:!symbian {
    SOURCES += \
        $$PWD/src/qlockfile.cpp \
        $$PWD/src/qlockfile_unix.cpp
}

# This is a quick workaround for generating forward header with Qt4.

QTSERIALPORT_PROJECT_QT4SUPPORT_INCLUDEDIR = $$PWD/include
INCLUDEPATH += \
               $$QTSERIALPORT_PROJECT_QT4SUPPORT_INCLUDEDIR \
               $$QTSERIALPORT_PROJECT_QT4SUPPORT_INCLUDEDIR/QtCore \
               $$QTSERIALPORT_PROJECT_QT4SUPPORT_INCLUDEDIR/private

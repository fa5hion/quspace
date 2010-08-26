# -------------------------------------------------
# Project created by QtCreator 2009-12-10T16:44:33
# -------------------------------------------------
QT += network \
    webkit
TARGET = quspace
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    webwidget.cpp \
    spaceitem.cpp \
    itemtoolbar.cpp \
    manager.cpp \
    session.cpp \
    itemeventfilter.cpp \
    space.cpp \
    workspacecomponent.cpp \
    logincomponent.cpp \
    itemdata.cpp \
    toolscomponent.cpp \
    taskbar.cpp
HEADERS += mainwindow.h \
    webwidget.h \
    spaceitem.h \
    itemtoolbar.h \
    manager.h \
    session.h \
    itemeventfilter.h \
    space.h \
    workspacecomponent.h \
    logincomponent.h \
    itemdata.h \
    toolscomponent.h \
    taskbar.h
FORMS += mainwindow.ui \
    workspacecomponent.ui \
    logincomponent.ui \
    toolscomponent.ui
RESOURCES += quspace.qrc
INCLUDEPATH += include
unix:LIBS += -Llib -lqjson
win32:LIBS += .\lib\libqjson.dll

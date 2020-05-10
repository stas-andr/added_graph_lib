TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -L/usr/local/lib -lfltk -lXext -lX11 -lfltk_images -lm


SOURCES += \
        GUI.cpp \
        Graph.cpp \
        Window.cpp \
        main.cpp
HEADERS += \
    GUI.h \
    Graph.h \
    Point.h \
    Simple_window.h \
    Window.h \
    fltk.h \


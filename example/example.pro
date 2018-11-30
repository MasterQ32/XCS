TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17 -Werror=wrong-ident
QMAKE_LFLAGS += -std=c++17

include($$PWD/../XCS.pri)

SOURCES += \
        main.cpp \
    componenta.cpp \
    componentb.cpp

HEADERS += \
    componenta.hpp \
    componentb.hpp

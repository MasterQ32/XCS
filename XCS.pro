TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
QMAKE_LFLAGS += -std=c++17

SOURCES += \
        main.cpp \
    componenta.cpp \
    componentb.cpp \
    xcs.cpp

HEADERS += \
    componenta.hpp \
    componentb.hpp \
    xcs.hpp

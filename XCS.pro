TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
QMAKE_LFLAGS += -std=c++17

SOURCES += \
        main.cpp \
    ecs.cpp \
    componenta.cpp \
    componentb.cpp

HEADERS += \
    ecs.hpp \
    componenta.hpp \
    componentb.hpp

# Generator binary

ROOT_DIR = ../..
DEPENDENCIES = generator routing storage indexer platform geometry coding base \
               osrm gflags expat sgitess jansson protobuf tomcrypt o5mreader \
               succinct stats_client

include($$ROOT_DIR/common.pri)

INCLUDEPATH *= $$ROOT_DIR/3party/gflags/src

CONFIG += console warn_on
CONFIG -= app_bundle
TEMPLATE = app

# needed for Platform::WorkingDir() and unicode combining
QT *= core

win32: LIBS *= -lShell32
macx-*: LIBS *= "-framework Foundation"

SOURCES += \
    generator_tool.cpp \

HEADERS += \

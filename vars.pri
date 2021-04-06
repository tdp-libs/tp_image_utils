TARGET = tp_image_utils
TEMPLATE = lib

DEFINES += tp_image_utils_LIBRARY

#SOURCES += src/Globals.cpp
HEADERS += inc/tp_image_utils/Globals.h

SOURCES += src/ByteMap.cpp
HEADERS += inc/tp_image_utils/ByteMap.h

SOURCES += src/ColorMap.cpp
HEADERS += inc/tp_image_utils/ColorMap.h

SOURCES += src/ColorMapF.cpp
HEADERS += inc/tp_image_utils/ColorMapF.h

SOURCES += src/Point.cpp
HEADERS += inc/tp_image_utils/Point.h

SOURCES += src/Grid.cpp
HEADERS += inc/tp_image_utils/Grid.h

SOURCES += src/LoadImages.cpp
HEADERS += inc/tp_image_utils/LoadImages.h

SOURCES += src/SaveImages.cpp
HEADERS += inc/tp_image_utils/SaveImages.h


SOURCES += src/ToGray.cpp
HEADERS += inc/tp_image_utils/ToGray.h

SOURCES += src/ToMono.cpp
HEADERS += inc/tp_image_utils/ToMono.h

SOURCES += src/ToFloat.cpp
HEADERS += inc/tp_image_utils/ToFloat.h

SOURCES += src/Scale.cpp
HEADERS += inc/tp_image_utils/Scale.h


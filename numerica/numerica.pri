# adding c++11
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    plot/pswriter.cpp \
    plot/plotter2d.cpp \
    numerica/tools/nvector.cpp \
    numerica/tests/test_plot.cpp

HEADERS += \
    plot/pswriter.h \
    plot/plotter2d.h \
    numerica/numerica.h \
    numerica/tools/nvector.h \
    numerica/tests/test_plot.h \
    numerica/tests/test_all.h

QT       += core gui openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


SOURCES += \
    AdapterWidget.cpp \
    QtOsgView.cpp \
    firstpersonmanipulator.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AdapterWidget.h \
    MainWindow.h \
    QtOsgView.h \
    firstpersonmanipulator.h

FORMS += \
    MainWindow.ui


INCLUDEPATH += $$PWD/../../build/include
DEPENDPATH += $$PWD/../../build/include

INCLUDEPATH += $$PWD/../../OpenSceneGraph/include
DEPENDPATH += $$PWD/../../OpenSceneGraph/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgDB
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgDBd

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/lib/ -losg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgd


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgGA
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgGAd

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgUtild

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgViewer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build/lib/ -losgViewerd

win32: LIBS += -L$$PWD/../../build/lib/ -lOpenThreadsd

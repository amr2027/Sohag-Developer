#=======================================================================================================================
# Sohag Developer Project
# This Project Aims To Create A PostgreSQL Tools
# To Help Users Using That Database
# At That Stage The Sohag Developer Is Generating Qt/C++ Files
# But Sohag Developer's Goal Is To Create A Compelete IDE For PostgreSQL
# Also The Project Will Be Extended To Include Other Databases As Long AS Possible
# For More Details Please Visit Sohag Developer website (http://sohag-developer.com/)
# This Software Is Licensed Under GPLv3 for more details visit (https://www.gnu.org/licenses/gpl-3.0.en.html)
# This Software is Supported By AlHuda Software الهدى للبرمجيات(http://alhuda-software.com/)
#=======================================================================================================================


QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sohag-developer
TEMPLATE = app


SOURCES += main.cpp\
        sohagmain.cpp\
    connectiondialog.cpp \
    sohagnewprowizard.cpp\
    projectui.cpp \
    sohageditprowizard.cpp \
    filedownloader.cpp \
    sohageditclass.cpp \
    welcomepage.cpp \
    classattributes.cpp \
    classinfo.cpp \
    sqldml.cpp \
    aboutsohagdeveloper.cpp \
    schemainfo.cpp \
    databasetable.cpp \
    sohagnewclass.cpp \
    tableattributes.cpp \
    newclassinfo.cpp



HEADERS  += sohagmain.h\
    connectiondialog.h \
    sohaggen.h \
    sohagnewprowizard.h\
    projectui.h \
    sohageditprowizard.h \
    filedownloader.h \
    sohageditclass.h \
    welcomepage.h \
    classattributes.h \
    classinfo.h \
    sqldml.h \
    aboutsohagdeveloper.h \
    schemainfo.h \
    databasetable.h \
    sohagnewclass.h \
    tableattributes.h \
    newclassinfo.h\
    interface.h



FORMS    += sohagmain.ui\
    connectiondialog.ui \
    sqldml.ui \
    aboutsohagdeveloper.ui

RESOURCES += \
    sohag-developer.qrc

RC_FILE = sohagdeveloper.rc

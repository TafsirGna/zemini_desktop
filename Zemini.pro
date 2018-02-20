#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T11:24:14
#
#-------------------------------------------------

QT       += core gui

QT       += network

QT       += widgets

QT       += sql

TARGET = Zemini
TEMPLATE = app

RC_FILE = Config/zemini.rc

INCLUDEPATH += C:\OpenCV\OpenCV3.1\build\include

LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_core310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_highgui310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_imgproc310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_features2d310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_calib3d310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_imgcodecs310.dll

INCLUDEPATH += C:\OpenSSL-Win64\include

LIBS += -LC:\OpenSSL-Win64\lib -llibeay32
LIBS += -LC:\OpenSSL-Win64\lib -lssleay32

SOURCES += main.cpp\
    #Controller sources
    Config/Parameters.cpp \
    Controllers/MainController.cpp \
    Controllers/PreferencesForm.cpp \
    Controllers/RegisterForm.cpp \
    Controllers/LogInForm.cpp \
    Controllers/AbstractController.cpp \
    #Database sources
    Database/Entities/File.cpp \
    Database/Entities/User.cpp \
    Database/EntityManagers/UserManager.cpp \
    Database/EntityManagers/FileManager.cpp \
    Database/Entities/Category.cpp \
    Database/Entities/Type.cpp \
    Database/EntityManagers/TypeManager.cpp \
    Database/EntityManagers/CategoryManager.cpp \
    Database/DatabaseManager.cpp \
    Database/EntityManagers/AbstractManager.cpp \
    # Service sources
    Services/LocalDbService.cpp \
    Services/NetworkService.cpp \
    Services/DirectoryService.cpp \
    Tests/TestsManager.cpp \
    Services/ServiceContainer.cpp \
    Services/ZeminiService.cpp

HEADERS  += Config\Parameters.h \
    Config/Functions.h \
    # Controller headers
    Controllers/PreferencesForm.h \
    Controllers/RegisterForm.h \
    Controllers/LogInForm.h \
    Controllers/MainController.h \
    Controllers/AbstractController.h \
    # Database headers
    Database/Entities/File.h \
    Database/Entities/Category.h \
    Database/Entities/Type.h \
    Database/EntityManagers/TypeManager.h \
    Database/EntityManagers/CategoryManager.h \
    Database/Entities/User.h \
    Database/EntityManagers/UserManager.h \
    Database/DatabaseManager.h \
    Database/EntityManagers/AbstractManager.h \
    Database/EntityManagers/FileManager.h \
    # Service headers
    Services/DirectoryService.h \
    Services/NetworkService.h \
    Services/LocalDbService.h \
    Tests/TestsManager.h \
    Services/ServiceContainer.h \
    Services/ZeminiService.h


FORMS    += Views\PreferencesForm.ui \
    Views/LogInForm.ui \
    Views/RegisterForm.ui

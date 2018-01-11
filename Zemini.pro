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

RC_FILE = Config\zemini.rc

SOURCES += main.cpp\
    #Controller sources
    Controllers\MainController.cpp \
    Controllers\PreferencesForm.cpp \
    Controllers\RegisterForm.cpp \
    Controllers\LogInForm.cpp \
    #Database sources
    Database\Entities\User.cpp \
    Database\EntityManagers\UserManager.cpp \
    Database\EntityManagers\FileManager.cpp \
    Database\Entities\File.cpp \
    Database\Entities\Category.cpp \
    Database\Entities\Type.cpp \
    Database\EntityManagers\TypeManager.cpp \
    Database\EntityManagers\CategoryManager.cpp \
    Database\Entities\AbstractFile.cpp \
    Database\Entities\Directory.cpp \
    Database\EntityManagers\DirectoryManager.cpp \
    Database\EntityManagers\DatabaseManager.cpp \
    Database\EntityManagers\AbstractManager.cpp \
    # Service sources
    Services\LocalDbService.cpp \
    Services\NetworkService.cpp \
    Services\DirectoryService.cpp \

HEADERS  += Config\Parameters.h \
    # Controller headers
    Controllers\PreferencesForm.h \
    Controllers\RegisterForm.h \
    Controllers\LogInForm.h \
    Controllers\MainController.h \
    # Database headers
    Database\Entities\File.h \
    Database\Entities\Category.h \
    Database\Entities\Type.h \
    Database\Entities\Directory.h \
    Database\Entities\AbstractFile.h \
    Database\EntityManagers\DirectoryManager.h \
    Database\EntityManagers\TypeManager.h \
    Database\EntityManagers\CategoryManager.h \
    Database\Entities\User.h \
    Database\EntityManagers\UserManager.h \
    Database\EntityManagers\DatabaseManager.h \
    Database\EntityManagers\AbstractManager.h \
    Database\EntityManagers\FileManager.h \
    # Service headers
    Services\DirectoryService.h \
    Services\NetworkService.h \
    Services\LocalDbService.h


FORMS    += Views\PreferencesForm.ui \
    Views\LogInForm.ui \
    Views\RegisterForm.ui

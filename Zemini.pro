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

# Firebase inclusion
INCLUDEPATH += C:\firebase_cpp_sdk_4.5.1\firebase_cpp_sdk\include\firebase

# OpenCV inclusion
INCLUDEPATH += C:\OpenCV\OpenCV3.1\build\include
INCLUDEPATH += C:/Qt/zlib-1.2.11
INCLUDEPATH += C:/Qt/quazip-0.7/quazip

LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_core310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_highgui310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_imgproc310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_features2d310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_calib3d310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_imgcodecs310.dll
LIBS += C:\OpenCV\OpenCV3.1\build\bin\libopencv_videoio310.dll

# Compression and decompression tools inclusion
LIBS += -LC:/Qt/zlib-1.2.11 -lz
LIBS += -LC:/Qt/quazip-0.7/quazip/release -lquazip

# OpenSSL inclusion
INCLUDEPATH += C:\OpenSSL\OpenSSL-Win64\include

#LIBS += -LC:\OpenSSL\OpenSSL-Win64\lib -llibeay32
#LIBS += -LC:\OpenSSL\OpenSSL-Win64\lib -lssleay32
#LIBS += -LC:\OpenSSL\OpenSSL-Win32\lib -llibcrypto

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
    Database/Entities/FileType.cpp \
    Database/EntityManagers/FileTypeManager.cpp \
    Database/EntityManagers/CategoryManager.cpp \
    Database/DatabaseManager.cpp \
    Database/EntityManagers/AbstractManager.cpp \
    # Service sources
    Services/LocalDbService.cpp \
    Services/NetworkService.cpp \
    Services/DirectoryService.cpp \
    Services/StaticFileService.cpp \
    Tests/TestsManager.cpp \
    Services/ServiceContainer.cpp \
    Services/ZeminiService.cpp \
    Controllers/VideoWatcherForm.cpp \
    Config/Functions.cpp \
    #Services/CypherService.cpp \
    Database/Entities/AppData.cpp \
    Database/EntityManagers/AppDataManager.cpp \
    Database/Entities/DriveType.cpp \
    Database/EntityManagers/DriveTypeManager.cpp \
    Database/Entities/Drive.cpp \
    Database/EntityManagers/DriveManager.cpp \
    Database/Entities/DbEntity.cpp \
    Database/Entities/FileFormat.cpp \
    Database/EntityManagers/FileFormatManager.cpp \
    Config/NetRequest.cpp \
    Controllers/UploadingForm.cpp \
    # Vendors' library
    #Vendor/OpenSSL_Wrapper/base64.cpp \
    #Vendor/OpenSSL_Wrapper/Crypto.cpp \
    Vendor/QtWaitingSpinner/waitingspinnerwidget.cpp \
    Vendor/qt-firebaseapi-master/firebase.cpp \
    Vendor/qt-firebaseapi-master/datasnapshot.cpp \
    Vendor/qt-firebaseapi-master/json.cpp \
    Vendor/qt-http-server-master/lib/QtHttpServer.cpp \
    Vendor/qt-http-server-master/lib/QtHttpRequest.cpp \
    Vendor/qt-http-server-master/lib/QtHttpReply.cpp \
    Vendor/qt-http-server-master/lib/QtHttpHeader.cpp \
    Vendor/qt-http-server-master/lib/QtHttpClientWrapper.cpp \
#    Vendor/QMediaPlayerExample/histogramwidget.cpp \
#    Vendor/QMediaPlayerExample/playercontrols.cpp \
#    Vendor/QMediaPlayerExample/playlistmodel.cpp \
#    Vendor/QMediaPlayerExample/videowidget.cpp \
    Controllers/AboutForm.cpp \
    Services/DownloadManager.cpp \
    Controllers/DriveTypeDialog.cpp

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
    Database/Entities/FileType.h \
    Database/EntityManagers/FileTypeManager.h \
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
    Services/ZeminiService.h \
    Services/StaticFileService.h \
    Controllers/VideoWatcherForm.h \
    #Services/CypherService.h \
    Database/Entities/AppData.h \
    Database/EntityManagers/AppDataManager.h \
    Database/Entities/DriveType.h \
    Database/EntityManagers/DriveTypeManager.h \
    Database/Entities/Drive.h \
    Database/EntityManagers/DriveManager.h \
    Database/Entities/DbEntity.h \
    Database/Entities/FileFormat.h \
    Database/EntityManagers/FileFormatManager.h \
    Config/NetRequest.h \
    Controllers/UploadingForm.h \
    #Vendors' libray
    Vendor/QtWaitingSpinner/waitingspinnerwidget.h \
    Vendor/qt-firebaseapi-master/firebase.h \
    Vendor/qt-firebaseapi-master/datasnapshot.h \
    Vendor/qt-http-server-master/lib/QtHttpServer.h \
    Vendor/qt-http-server-master/lib/QtHttpRequest.h \
    Vendor/qt-http-server-master/lib/QtHttpReply.h \
    Vendor/qt-http-server-master/lib/QtHttpHeader.h \
    Vendor/qt-http-server-master/lib/QtHttpClientWrapper.h \
#    Vendor/QMediaPlayerExample/histogramwidget.h \
#    Vendor/QMediaPlayerExample/playercontrols.h \
#    Vendor/QMediaPlayerExample/playlistmodel.h \
#    Vendor/QMediaPlayerExample/videowidget.h \
    Controllers/AboutForm.h \
    #Vendor/OpenSSL_Wrapper/base64.h \
    #Vendor/OpenSSL_Wrapper/Crypto.h
    Services/DownloadManager.h \
    Controllers/DriveTypeDialog.h


FORMS    += Views\PreferencesForm.ui \
    Views/LogInForm.ui \
    Views/RegisterForm.ui \
    Views/UploadingForm.ui \
    Views/AboutForm.ui \
    Views/VideoWatcherForm.ui \
    Views/DriveTypeDialog.ui

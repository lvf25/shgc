//#include "mainwindow.h"



#include <QtGlobal>
#include <QApplication>
#include <QIcon>
#include <QSslSocket>
#include <QProcessEnvironment>
#include <QHostAddress>
#include <QUdpSocket>
#include <QtPlugin>
#include <QStringListModel>
#include "QGCApplication.h"
//#include "AppMessages.h"

#include "sqlinit.h"

#ifndef __mobile__
    #include "QGCSerialPortInfo.h"
//    #include "RunGuard.h"
#endif

#include <iostream>


///* SDL does ugly things to main() */
#ifdef main
#undef main
#endif



#ifndef __mobile__
#ifndef NO_SERIAL_LINK
    Q_DECLARE_METATYPE(QGCSerialPortInfo)
#endif
#endif


#if defined(__android__) && !defined(NO_SERIAL_LINK)
#include <jni.h>
#include "qserialport.h"

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    Q_UNUSED(reserved);

    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    QSerialPort::setNativeMethods();

    return JNI_VERSION_1_6;
}
#endif


#ifdef __android__
#include <QtAndroid>
bool checkAndroidWritePermission() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
             return false;
        }
   }
   return true;
}
#endif



int main(int argc, char *argv[])
{
    QGCApplication* app = new QGCApplication(argc, argv);
    Q_CHECK_PTR(app);

    if(!createConnection())
    {
        return -1;
    }

#ifdef Q_OS_LINUX
    QApplication::setWindowIcon(QIcon("./resources/icons/QGC.ico"));
#endif /* Q_OS_LINUX */



#ifndef NO_SERIAL_LINK
    qRegisterMetaType<QSerialPort::SerialPortError>();
#endif
#ifdef QGC_ENABLE_BLUETOOTH
    qRegisterMetaType<QBluetoothSocket::SocketError>();
    qRegisterMetaType<QBluetoothServiceInfo>();
#endif
    qRegisterMetaType<QAbstractSocket::SocketError>();
#ifndef __mobile__
#ifndef NO_SERIAL_LINK
    qRegisterMetaType<QGCSerialPortInfo>();
#endif
#endif


    qRegisterMetaType<QList<QPair<QByteArray,QByteArray> > >();
    qRegisterMetaType<QGeoCoordinate>();

    app->_initCommon();

    int exitCode = 0;

#ifdef __android__
        checkAndroidWritePermission();
#endif

    if (!app->_initForNormalAppBoot()) {
        return -1;
    }

    exitCode = app->exec();

    app->_shutdown();
    delete app;

    qDebug() << "After app delete";

    return exitCode;
}

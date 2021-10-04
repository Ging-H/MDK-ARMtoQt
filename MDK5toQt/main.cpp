#include <QCoreApplication>
#include <QtXml>
#include <qDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QString>
#include <stdbool.h>
#include "mdk5doc.h"


#if _MSC_VER >=1600 //VS2010版本号是1600
#pragma execution_character_set("utf-8")
#endif

QDomDocument openXmlFile(QString filePath, bool &isOk);
void read_save_files( QDomDocument *doc,   QDir baseAddr, QString prjname);
void read_save_include( QDomDocument *doc, QDir baseAddr, QString prjname);
void read_save_config( QDomDocument *doc,  QDir baseAddr, QString prjname);
void save_creator(QString baseAddr,QString prjname);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList arguments = app.arguments();
    if( argc <= 1 ){
        qDebug()<<arguments[0];
        qDebug()<<"把*.uvprojx拖动到本程序打开...";
        return app.exec();
    }


    /* 提取uvprojx的地址信息 */
    QFileInfo fileInfo( arguments[1] );
    QString path = QDir::fromNativeSeparators(fileInfo.path());
    QString prjName = fileInfo.fileName().remove(".uvprojx");

    qDebug()<< "project path: "<<path;
    qDebug()<< "project name: "<<prjName;

    /* 验证文件 */
    bool isOk;
    QDomDocument doc = openXmlFile(arguments[1], isOk);
    if( !isOk ){
        return app.exec();
    }
    QDir dir(path);
//    dir.cdUp();
    /* 提取路径和保存新文件 */
    read_save_files(&doc, dir, prjName);
    read_save_include(&doc, dir, prjName);
    read_save_config(&doc, dir, prjName);
    dir.cdUp();
    save_creator( dir.path(), prjName);
    qDebug()<< "create "+ prjName + ".creator in " + dir.path() ;
//    return app.exec();
    return 1;
}

/**
 * @brief openXmlFile
 * @param filePath
 * @param isOk
 * @return
 */
QDomDocument openXmlFile(QString filePath, bool &isOk)
{
    QFile *file = new QFile(filePath);
    QDomDocument doc;
    isOk = true;
    if(!doc.setContent(file)){	//也可以传入字符串
        qDebug()<<"xml file set content fail";
        qDebug()<<"not a value file";
        isOk = false;
    }
    file->close();
    delete file;
    return doc;
}
/**
 * @brief read_save_files
 * @param doc
 * @param baseAddr
 * @param prjname
 */
void read_save_files( QDomDocument *doc, QDir basePath, QString prjname)
{
    /* 读取并转换文件路径:相对路径->绝对路径 */
    QDomElement rootElement = doc->documentElement();
    QDomNodeList elementList = rootElement.elementsByTagName("File");
    qint32 count = elementList.count();
    QStringList pathList;
    QDir prjPath = basePath;
    qDebug()<<"files:";
    for( qint32 i=0; i<count; i++ ){
        prjPath = basePath;
        /* 路径转换 */
        QDomElement tmp = elementList.at(i).toElement().firstChildElement("FilePath");
        QString path = QDir::fromNativeSeparators( tmp.text() );
        while( path.startsWith("../") ){
            prjPath.cdUp();
            path = path.mid(3);
        }
        pathList<<prjPath.absoluteFilePath( path );
        qDebug()<<pathList.last();
    }
    /* 保存文件 */
    basePath.cdUp();
    QFile files(basePath.path()  + "/" + prjname + ".files" );
    files.open( QIODevice::WriteOnly );
    foreach (auto path, pathList) {
        path = "./" + basePath.relativeFilePath(path);
        files.write( path.toLocal8Bit() + "\r\n" );
    }
    files.close();
}
/**
 * @brief read_save_include
 * @param doc
 */
void read_save_include( QDomDocument *doc, QDir basePath, QString prjname)
{
    /* 读取并转换文件路径:相对路径->绝对路径 */
    QDomElement rootElement = doc->documentElement();
    QDomElement node = rootElement.elementsByTagName("Cads").at(0).firstChildElement("VariousControls");

    QString tmp;
    tmp = node.elementsByTagName("IncludePath").at(0).toElement().text();
    QStringList dirList = tmp.split(';');
    QStringList pathList;

    QDir prjPath = basePath;
    qDebug()<<"include path:";
    foreach (QString path, dirList) {
        prjPath = basePath;
        /* 路径转换 */
        path = QDir::fromNativeSeparators( path.trimmed() );
        while( path.startsWith("../") ){
            prjPath.cdUp();
            path = path.mid(3);
        }
        pathList<< prjPath.absoluteFilePath( path );
        qDebug()<<pathList.last();
    }
    basePath.cdUp();
    /* 保存文件 */
    QFile files( basePath.path()  + "/" + prjname + ".includes" );
    files.open( QIODevice::WriteOnly );
    foreach (auto path, pathList) {
        path = "./" + basePath.relativeFilePath( path );
        files.write( path.toLocal8Bit() + "\r\n" );
    }
    files.close();
}
/**
 * @brief read_save_config
 * @param doc
 * @param baseAddr
 * @param prjname
 */
void read_save_config( QDomDocument *doc, QDir basePath, QString prjname)
{
    /* 读取并转换文件路径:相对路径->绝对路径 */
    QDomElement rootElement = doc->documentElement();
    QDomElement node = rootElement.elementsByTagName("Cads").at(0).firstChildElement("VariousControls");
    QString  tmp;
    tmp = node.elementsByTagName("Define").at(0).toElement().text();
    QStringList defList = tmp.split(',');

    QStringList define;
    qDebug()<<"define:";
    foreach( QString def, defList){
        QStringList tmp = def.trimmed().split('=');
        if( tmp.count() > 1 ){
            define<<"#define " + tmp.at(0) + "    " + tmp.at(1); // #define (0) (1)
            qDebug()<<define.last();
        }else{
            define<<"#define " + tmp.at(0);                      // #define (0)
            qDebug()<<define.last();
        }
    }
    define<< ("#define __cplusplus");
    /* 保存文件 */
    basePath.cdUp();
    QFile files( basePath.path() + "/" + prjname + ".config" );

    files.open( QIODevice::WriteOnly );
    foreach (auto def, define) {
        files.write( def.toLocal8Bit() + "\r\n" );
    }
    files.close();
}
/**
 * @brief save_creator 创建creator文件
 * @param baseAddr
 * @param prjname
 */
void save_creator(QString baseAddr, QString prjname)
{
    /* 保存文件 */
    QFile files( baseAddr + "/" + prjname + ".creator" );
    files.open( QIODevice::WriteOnly );
    files.write( "[General]\r\n" );
    files.close();
}

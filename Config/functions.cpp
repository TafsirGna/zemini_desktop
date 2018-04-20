#include <Config/Functions.h>

using namespace cv;

/**
 * @brief fromJsonToUser
 * @param variantMap
 * @return
 */
User * Functions::fromJsonToUser(QVariantMap variantMap)
{
    User * user = new User();

    user->setId(variantMap["id"].toInt());
    //user->setFamilyname(variantMap["familyname"].toString());
    //user->setFirstname(variantMap["firstname"].toString());
    user->setEmail(variantMap["email"].toString());
    user->setPassword(variantMap["password"].toString());
    //user->setDateInscription(variantMap["dateinscription"].toString());
    user->setUsername(variantMap["username"].toString());
    return user;
}

/**
 * @brief isEmailValid
 * @param email
 * @return
 */
bool Functions::isEmailValid(QString email)
{
    return true;
}

/**
 * @brief fromJsonToCategories
 * @param variantMap
 * @return
 */
QList<Category> *  Functions::fromJsonToCategories(QVariant variant)
{
    //qDebug() << "Initializing Category : " << variant << endl;
    QList<Category> * categories = new QList<Category>();

    if (variant.canConvert<QVariantList>())
    {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        // Can use iterators:
        QSequentialIterable::const_iterator it = iterable.begin();
        const QSequentialIterable::const_iterator end = iterable.end();
        for ( ; it != end; ++it) {
            QMap<QString, QVariant> map = (*it).toMap();
            categories->append(Category(map["id"].toInt(), map["name"].toString()));
        }
    }
    return categories;
}

QList<FileFormat> *  Functions::fromJsonToFileFormats(QVariant variant)
{
    QList<FileFormat> * fileFormats = new QList<FileFormat>();
    /*
    for (QVariantMap::const_iterator iter = variantMap.begin(); iter != variantMap.end(); ++iter){
        if (iter.key() != "requestCode"){
            Category * cat = new Category(iter.key().toInt(), iter.value().toString());
            categories->append(*cat);
            //qDebug() << "id : " << cat->getId() << " name : " << cat->getName()<< endl;
        }
    }
    */
    return fileFormats;
}

QList<FileType> *  Functions::fromJsonToFileTypes(QVariant variant)
{
    QList<FileType> * fileTypes = new QList<FileType>();
    /*
    for (QVariantMap::const_iterator iter = variantMap.begin(); iter != variantMap.end(); ++iter){
        if (iter.key() != "requestCode"){
            Category * cat = new Category(iter.key().toInt(), iter.value().toString());
            categories->append(*cat);
            //qDebug() << "id : " << cat->getId() << " name : " << cat->getName()<< endl;
        }
    }
    */
    return fileTypes;
}

/**
 * @brief Functions::makeLinkToZeminiFolder
 */
void Functions::makeLinkToZeminiFolder()
{
    QString dirPath= QDir().homePath()+"/Zemini";
    dirPath.replace("\\","/");
    QString linkPath = QDir().homePath()+"/Links/Zemini.lnk";
    linkPath.replace("\\","/");
    QFile dir (dirPath);
    bool link_created = dir.link(linkPath);
    if (!link_created)
        qDebug("Failed to create the link");
}

/**
 * @brief Functions::getRelativePath
 * @param path
 * @return
 */
QString Functions::getRelativePath(QString path)
{
    return path.remove(0, path.indexOf("/"+Parameters::ROOT_DIR_NAME));
}

/**
 * @brief DirectoryService::generateThumbnails
 * @param fileInfo
 * @return
 */
QFileInfo *Functions::generateThumbnails(QFileInfo fileInfo)
{
    if (isVideoFile(fileInfo))
        return extractThumbnail(fileInfo);

    if (isImageFile(fileInfo)){
        return NULL;
    }

    // if it's not a either a video or an image then we extract the file type's icon out of it
    QFileIconProvider *provider = new QFileIconProvider();
    QImage *image = new QImage(provider->icon(fileInfo).pixmap(128,128).toImage());
    QString thumbPath = Parameters::THUMBS_DIR+"/"+QString::number(QDateTime::currentMSecsSinceEpoch())+".png";
    if (!image->save(thumbPath)){
        qDebug() << "An error occured while saving the icon" << endl;
        return NULL;
    }
    return new QFileInfo(thumbPath);

    return NULL;
}

QFileInfo *Functions::extractThumbnail(QFileInfo fileInfo)
{
    // i use opencv to extract a picture out of the video, that's gonna be the video thumbnail
    VideoCapture vcap(cv::String(fileInfo.absoluteFilePath().toStdString()));
    if (!vcap.isOpened()){
        qDebug() << "Failed in opening the video file" << endl;
        return NULL;
    }
    // Everything's ok then
    int frameIndex = (int) vcap.get(CV_CAP_PROP_FRAME_COUNT);
    frameIndex /= Parameters::FRAME_COUNT_DIVIDER;

    Mat frame;
    int i(0);
    while(i < frameIndex){
        vcap >> frame;
        i++;
    }

    QString thumbName = QString::number(QDateTime::currentMSecsSinceEpoch())+".png";
    QString thumbPath = Parameters::THUMBS_DIR+"/"+thumbName;

    //qDebug() << "in gen thumbnails " << endl;
    if (imwrite(cv::String(thumbPath.toStdString()), frame))
        return new QFileInfo(thumbPath);

    return NULL;
}

/**
 * @brief Functions::isVideoFile Check if a file is a video file
 * @param fileInfo
 * @return
 */
bool Functions::isVideoFile(QFileInfo fileInfo)
{
    if (Parameters::VIDEO_FORMATS.contains(fileInfo.suffix()))
        return true;
    return false;
}

bool Functions::isImageFile(QFileInfo)
{
    return false;
}

QString Functions::getDriveAbsPath(QString fileInfoAbsPath)
{
    return fileInfoAbsPath.remove(fileInfoAbsPath.indexOf("/"+Parameters::ROOT_DIR_NAME), fileInfoAbsPath.length());
}

File * Functions::fromSqlRecord2File(QSqlRecord sqlRecord)
{
    QMap<QString, QString> driveProperties, fileTypeProperties, categoryProperties, folderProperties;
    driveProperties.insert("id", sqlRecord.value(4).toString());
    fileTypeProperties.insert("id", sqlRecord.value(2).toString());
    categoryProperties.insert("id", sqlRecord.value(3).toString());
    folderProperties.insert("id", sqlRecord.value(1).toString());

    File * file = new File(sqlRecord.value(0).toInt(), sqlRecord.value(7).toString(), sqlRecord.value(8).toString(), sqlRecord.value(5).toDateTime(), sqlRecord.value(6).toDateTime(), sqlRecord.value(9).toInt(), sqlRecord.value(10).toInt(), ((sqlRecord.value(11).toString() == "") ? NULL : new QFileInfo(sqlRecord.value(11).toString())),FileTypeManager::getOneBy(fileTypeProperties), CategoryManager::getOneBy(categoryProperties), FileManager::getOneBy(folderProperties), DriveManager::getOneBy(driveProperties));
    return file;
}

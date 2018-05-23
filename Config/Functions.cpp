#include <Config/Functions.h>

using namespace cv;

/**
 * @brief fromJsonToUser
 * @param variantMap
 * @return
 */
User * Functions::fromJsonToUser(QMap<QString, QVariant> map)
{
    User * user = new User();

    user->setId(map["id"].toInt());
    //user->setFamilyname(variantMap["familyname"].toString());
    //user->setFirstname(variantMap["firstname"].toString());
    user->setEmail(map["email"].toString());
    user->setPassword(map["password"].toString());
    //user->setDateInscription(variantMap["dateinscription"].toString());
    user->setUsername(map["username"].toString());
    return user;
}

double Functions::fromOctect2Ko(int size)
{
    return (size/pow(1024, 2));
}

double Functions::fromOctect2Mo(int size)
{
    return (size/pow(1024, 2));
}

double Functions::fromOctect2Go(int size)
{
    return size/(size/pow(1024, 3));
}

QMap<QString, QString> Functions::fromOctect2Normal(int size)
{
    QMap<QString, QString> map;
    if (size > pow(1024,3)){
        map.insert("label", "Go");
        map.insert("value", QString::number(fromOctect2Go(size)));
        return map;
    }
    else if (size > pow(1024,2)){
        map.insert("label", "Mo");
        map.insert("value", QString::number(fromOctect2Mo(size)));
        return map;
    }
    else if (size > 1024){
        map.insert("label", "Ko");
        map.insert("value", QString::number(fromOctect2Ko(size)));
        return map;
    }
    else{
        map.insert("label", "o");
        map.insert("value", QString::number(size));
        return map;
    }
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
QList<Category*> *  Functions::fromJsonToCategories(QVariant variant)
{
    //qDebug() << "Initializing Category : " << variant << endl;
    QList<Category*> * categories = new QList<Category*>();

    if (variant.canConvert<QVariantList>())
    {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        // Can use iterators:
        QSequentialIterable::const_iterator it = iterable.begin();
        const QSequentialIterable::const_iterator end = iterable.end();
        for ( ; it != end; ++it) {
            QMap<QString, QVariant> map = (*it).toMap();
            categories->append(new Category(map["id"].toInt(), map["name"].toString()));
        }
    }
    return categories;
}

QList<FileFormat*> *  Functions::fromJsonToFileFormats(QVariant variant)
{
    QList<FileFormat*> * fileFormats = new QList<FileFormat*>();
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

QList<FileType*> *  Functions::fromJsonToFileTypes(QVariant variant)
{
    QList<FileType*> * fileTypes = new QList<FileType*>();
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
bool Functions::makeLinkToRootFolder(QDir rootDir)
{
    QString dirPath= rootDir.absolutePath();
    QString linkPath = QDir().homePath()+"/Links/"+Parameters::ROOT_DIR_NAME+".lnk";
    linkPath.replace("\\","/");
    QFile dir(dirPath);
    bool link_created = dir.link(linkPath);
    if (!link_created){
        qDebug("Failed to create the link");
        QMessageBox::warning(0, "Link", "An error occured when makeing the link to the main folder. Check the reason and restart later please!", QMessageBox::Yes);
        return false;
    }
    return true;
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

    try {

    } catch (...) {
    }

    // if it's not a either a video or an image then we extract the file type's icon out of it
    QFileIconProvider *provider = new QFileIconProvider();
    QImage *image = new QImage(provider->icon(fileInfo).pixmap(128,128).toImage());


    AppData * appData = (AppData *) AppDataManager::getByKey(AppDataManager::STORAGE_DIR_KEY);
    QString thumbsPath = appData->getValue()+"/"+Parameters::THUMBS_DIR_NAME;
    if (!QDir(thumbsPath).exists()){
        QDir(thumbsPath).mkdir(".");
    }
    QString thumbPath = thumbsPath+"/"+QString::number(QDateTime::currentMSecsSinceEpoch())+".png";
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

    AppData * appData = (AppData *) AppDataManager::getByKey(AppDataManager::STORAGE_DIR_KEY);
    QString thumbsPath = appData->getValue()+"/"+Parameters::THUMBS_DIR_NAME;
    if (!QDir(thumbsPath).exists()){
        QDir(thumbsPath).mkdir(".");
    }
    QString thumbPath = thumbsPath+"/"+QString::number(QDateTime::currentMSecsSinceEpoch())+".png";

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

    //qDebug() << sqlRecord.value(8).toString() <<" :/ " << sqlRecord.value(11).toString() << endl;
    QString absPath = AppDataManager::getByKey(AppDataManager::STORAGE_DIR_KEY)->getValue();
    QStringList pathList = absPath.split("/");
    pathList.removeLast();

    File * file = new File(sqlRecord.value(0).toInt(), sqlRecord.value(8).toString(), sqlRecord.value(9).toString(), sqlRecord.value(5).toDateTime(), sqlRecord.value(6).toDateTime(), sqlRecord.value(7).toDateTime(), sqlRecord.value(10).toInt(), sqlRecord.value(11).toInt(), ((sqlRecord.value(12).toString() == "") ? NULL : new QFileInfo(pathList.join("/")+sqlRecord.value(12).toString())),FileTypeManager::getOneBy(fileTypeProperties), CategoryManager::getOneBy(categoryProperties), FileManager::getOneBy(folderProperties), DriveManager::getOneBy(driveProperties));
    return file;
}

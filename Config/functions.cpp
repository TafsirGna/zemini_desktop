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

int Functions::getLength(QFileInfo fileInfo)
{
    // trying to get the duration of a given file
    int length(0);
    if (fileInfo.isFile() && Functions::isMediaFile(fileInfo)){
        VideoCapture vcap(cv::String(fileInfo.absoluteFilePath().toStdString()));
        if (!vcap.isOpened()){
            qDebug() << "Failed in opening the video file" << endl;
        }
        else{
            length = (int) vcap.get(CV_CAP_PROP_FRAME_COUNT);
            length /= vcap.get(CV_CAP_PROP_FPS);
            qDebug() << "length is " << length << endl;
        }
    }
    return length;
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
    if (variant.canConvert<QVariantList>())
    {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        // Can use iterators:
        QSequentialIterable::const_iterator it = iterable.begin();
        const QSequentialIterable::const_iterator end = iterable.end();
        for ( ; it != end; ++it) {
            fileFormats->append(Functions::fromJsonToFileFormat(*it));
        }
    }
    return fileFormats;
}

FileFormat*  Functions::fromJsonToFileFormat(QVariant variant)
{
    if (variant.toMap().empty())
        return NULL;
    QMap<QString, QVariant> map = variant.toMap();
    return new FileFormat(map["id"].toInt(), map["name"].toString());
}

QList<FileType*> *  Functions::fromJsonToFileTypes(QVariant variant)
{
    QList<FileType*> * fileTypes = new QList<FileType*>();
    if (variant.canConvert<QVariantList>())
    {
        QSequentialIterable iterable = variant.value<QSequentialIterable>();
        // Can use iterators:
        QSequentialIterable::const_iterator it = iterable.begin();
        const QSequentialIterable::const_iterator end = iterable.end();
        for ( ; it != end; ++it) {
            QMap<QString, QVariant> map = (*it).toMap();
            FileFormat * fileFormat = Functions::fromJsonToFileFormat(map["format"]);
            if (fileFormat != NULL){
                QMap<QString, QString> fileFormatProperties;
                fileFormatProperties.insert("name", fileFormat->getName());
                fileFormat = FileFormatManager::getOneBy(fileFormatProperties);
            }
            fileTypes->append(new FileType(map["id"].toInt(), map["name"].toString(), map["suffix"].toString(), fileFormat));
            //qDebug() << "Rhoor" << map["format"] << endl;
            //(new FileType(map["id"].toInt(), map["name"].toString(), map["suffix"].toString(), fileFormat))->toString();
        }
    }
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
    QString path = fileInfo.absoluteFilePath();
    path = path.remove(path.indexOf("/"+Parameters::ROOT_DIR_NAME), path.size()-1)+"/"+Parameters::ROOT_DIR_NAME+"/"+Parameters::THUMBS_DIR_NAME;

    qDebug() << "pathoo " << path << endl;
    QDir thumbsDir(path);
    if (!thumbsDir.exists()){
        thumbsDir.mkdir(".");
    }

    QDir fileThumbsFolder(thumbsDir.absolutePath()+"/"+QString::number(QDateTime::currentMSecsSinceEpoch()));
    if (!fileThumbsFolder.mkdir(".")){
        qDebug() << "Failed to create the file thumbnails folder" << endl;
        return NULL;
    }

    if (isVideoFile(fileInfo)){
        extractVideoThumbs(fileInfo, fileThumbsFolder);
    }

    try {
    } catch (...) {
    }

    // if it's not a either a video or an image then we extract the file type's icon out of it
    QFileIconProvider *provider = new QFileIconProvider();
    QImage *image = new QImage(provider->icon(fileInfo).pixmap(128,128).toImage());

    if (!image->save(fileThumbsFolder.path()+"/logo.png")){
        qDebug() << "An error occured while saving the icon" << endl;
    }

    JlCompress::compressDir(fileThumbsFolder.path()+".zip", fileThumbsFolder.path());
    fileThumbsFolder.removeRecursively();
    return new QFileInfo(fileThumbsFolder.path()+".zip");
}

void Functions::extractVideoThumbs(QFileInfo fileInfo, QDir folder)
{

    // i use opencv to extract a picture out of the video, that's gonna be the video thumbnail
    VideoCapture vcap(cv::String(fileInfo.absoluteFilePath().toStdString()));
    if (!vcap.isOpened()){
        qDebug() << "Failed in opening the video file" << endl;
        return;
    }

    // Everything's ok then
    int frameCount = (int) vcap.get(CV_CAP_PROP_FRAME_COUNT);
    int sectorFrameCount = (int) (frameCount / (Parameters::NB_THUMBS_PER_FILE));

    Mat frame;
    int i(0), sectorFrameIndex(0), count(0);
    while(i < frameCount){
        if (sectorFrameIndex == sectorFrameCount){
            count++;
            vcap >> frame;

            if (!imwrite(cv::String((folder.path()+"/"+QString::number(count)+".png").toStdString()), frame))
                qDebug() << "Failed to generate the thumbnail n°" << count << endl;

            sectorFrameIndex = 0;
        }
        sectorFrameIndex++;
        i++;
    }
}

/**
 * @brief Functions::isVideoFile Check if a file is a video file
 * @param fileInfo
 * @return
 */
bool Functions::isVideoFile(QFileInfo fileInfo)
{
    QMap<QString, QString> fileTypeProperties, fileFormatProperties;
    fileFormatProperties.insert("name", "video");
    fileTypeProperties.insert("format_id", QString::number(FileFormatManager::getOneBy(fileFormatProperties)->getId()));
    QList<FileType * > * fileTypes = FileTypeManager::getBy(fileTypeProperties);
    qDebug() << "After cooler " << endl;

    for (int i(0); i < FileTypeManager::getAll()->size(); i++){
        //qDebug () << "inc " << i << endl;
        //FileTypeManager::getAll()->at(i)->toString();
    }

    QStringList suffixesList;
    int size = fileTypes->size();
    for (int i(0); i < size; i++){
        suffixesList.append(fileTypes->at(i)->getSuffix());
    }

    if (suffixesList.contains(fileInfo.suffix()))
        return true;
    return false;
}

bool Functions::isImageFile(QFileInfo)
{
    return false;
}

bool Functions::isMediaFile(QFileInfo fileInfo)
{
    if (Functions::isVideoFile(fileInfo))
        return true;
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

    Drive *drive = DriveManager::getOneBy(driveProperties);

    File * file = new File(sqlRecord.value(0).toInt(), sqlRecord.value(6).toString(), sqlRecord.value(7).toString(), sqlRecord.value(5).toDateTime(), sqlRecord.value(8).toInt(), sqlRecord.value(9).toInt(), ((sqlRecord.value(10).toString() == "") ? NULL : new QFileInfo(drive->getAbsolutepath()+"/"+Parameters::ROOT_DIR_NAME+"/"+Parameters::THUMBS_DIR_NAME+"/"+sqlRecord.value(10).toString())), sqlRecord.value(11).toInt(), FileTypeManager::getOneBy(fileTypeProperties), CategoryManager::getOneBy(categoryProperties), FileManager::getOneBy(folderProperties), drive);
    return file;
}

Drive *Functions::fromSqlRecord2Drive(QSqlRecord sqlRecord)
{
    QMap<QString, QString> properties;
    properties.insert("id", sqlRecord.value(2).toString());
    return new Drive(sqlRecord.value(0).toInt(), sqlRecord.value(1).toString(),
                     DriveTypeManager::getOneBy(properties), ((sqlRecord.value(3).toInt() == 1) ? true : false ));
}

FileFormat *Functions::fromSqlRecord2FileFormat(QSqlRecord sqlRecord)
{
    FileFormat * fileFormat = new FileFormat(sqlRecord.value(0).toInt(), sqlRecord.value(1).toString());
    return fileFormat;
}

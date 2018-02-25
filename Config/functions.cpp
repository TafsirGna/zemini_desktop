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
QList<Category> *  Functions::fromJsonToCategories(QVariantMap variantMap)
{
    QList<Category> * categories = new QList<Category>();
    for (QVariantMap::const_iterator iter = variantMap.begin(); iter != variantMap.end(); ++iter){
        if (iter.key() != "requestCode"){
            Category * cat = new Category(iter.key().toInt(), iter.value().toString());
            categories->append(*cat);
            //qDebug() << "id : " << cat->getId() << " name : " << cat->getName()<< endl;
        }
    }
    return categories;
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
    VideoCapture vcap(cv::String(fileInfo.absoluteFilePath().toStdString()));
    if (!vcap.isOpened()){
        qDebug() << "Failed in opening the video file" << endl;
        return NULL;
    }
    // Everything's ok then
    Mat frame;
    vcap >> frame;
    QString thumbName = fileInfo.baseName()+".png";
    QString thumbPath = Parameters::THUMBS_DIR+"/"+thumbName;

    //qDebug() << "in gen thumbnails " << endl;
    if (imwrite(cv::String(thumbPath.toStdString()), frame))
        return new QFileInfo(thumbPath);

    return NULL;
}

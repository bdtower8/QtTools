#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QImage>
#include <QDebug>

#define TOP_BAR_HEIGHT 31

int main(int argc, char *argv[])
{
    if(argc < 2) {
        qDebug() << "Expected usage: \"./ScreenShotCleaner \"folder_name\" top_bar_height_in_pixels\"";
        return -1;
    }

    QCoreApplication app(argc, argv);
    QString folderName = app.arguments().at(1);

    int topBarHeight = TOP_BAR_HEIGHT;
    if(argc == 3) {
        topBarHeight = app.arguments().at(2).toInt();
    }

    QDir dirImages(folderName);
    QStringList filesInFolder = dirImages.entryList(QStringList() << "*.JPG" << "*.PNG", QDir::Files);

    // make an output folder
    QString processedFolder = folderName + "/Processed";
    dirImages.mkdir("Processed");

    // for all the image files, remove top bar and borders
    foreach(const QString& filename, filesInFolder) {
        qDebug() << "Processing" << filename;

        QString fullFilename = folderName + "/" + filename;
        QString fullNewFilename = processedFolder + "/" + filename;

        if(QFile::exists(fullFilename)) {
            QImage imageOrig(fullFilename);
            QRect newRect(1, topBarHeight, imageOrig.width() - 2, imageOrig.height() - topBarHeight - 1);
            QImage newImage = imageOrig.copy(newRect);
            newImage.save(fullNewFilename);
        }
    }

    return 0;
}

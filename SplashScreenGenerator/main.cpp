#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <QMap>
#include <QDebug>

int main(int argc, char *argv[])
{
    if(argc != 2) {
        qDebug() << "Expected usage: \"./SplashScreenGenerator filename.png\"";
        return -1;
    }

    QCoreApplication a(argc, argv);
    QString sImageFilename = a.arguments().at(1);

    QMap<QString, QSize> mapImageSizes;
    mapImageSizes.insert("Default.png", QSize(320, 480));
    mapImageSizes.insert("Default@2x.png", QSize(640, 960));
    mapImageSizes.insert("Default@3x.png", QSize(1242, 2208));
    mapImageSizes.insert("Default-568h@2x.png", QSize(640, 1136));
    mapImageSizes.insert("Default-667h@2x.png", QSize(750, 1334));
    mapImageSizes.insert("Default-736h@3x.png", QSize(1242, 2208));
    mapImageSizes.insert("Default-Portrait.png", QSize(768, 1024));
    mapImageSizes.insert("Default-Portrait@2x.png", QSize(1536, 2048));
    mapImageSizes.insert("Default-Portrait@3x.png", QSize(2304, 3072));

    QFile textFile("info.plist");
    QTextStream textStream(&textFile);

    if(!textFile.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "Failed to open info.plist for writing";
        return -1;
    }

    textStream << "<key>UILaunchImages</key>\n<array>\n";

    if(QFile::exists(sImageFilename)) {
        // open the input image
        qDebug() << "Processing image - " + sImageFilename;
        QImage imageOrig(sImageFilename);

        // for all of the desired sizes
        foreach(const QString& key, mapImageSizes.keys()) {
            // scale the image down (make it larger to preserve aspect ratio)
            QImage thisImage = imageOrig.scaled(mapImageSizes[key], Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

            // if we had to grow the image to preserve the aspect, cut off the edges
            int iExtraWidth = thisImage.width() - mapImageSizes[key].width();
            int iExtraHeight = thisImage.height() - mapImageSizes[key].height();

            // center the image
            QRect rectCrop(iExtraWidth / 2, iExtraHeight / 2, mapImageSizes[key].width(), mapImageSizes[key].height());
            QImage thisImageCropped = thisImage.copy(rectCrop);

            // save the image
            thisImageCropped.save(key);

            qDebug() << "Saved " + key + " (" + QString::number(mapImageSizes[key].width()) +
                        " x " + QString::number(mapImageSizes[key].height()) + ")";

            // add the entry to info.plist
            textStream << "\t<dict>\n\t\t<key>UILaunchImageMinimumOSVersion</key>\n\t\t<string>7.0</string>\n\t\t<key>UILaunchImageName</key>\n\t\t<string>" +
                          key.mid(0, key.size() - 4) + "</string>\n\t\t<key>UILaunchImageOrientation</key>\n\t\t<string>Portrait</string>\n\t\t" +
                          "<key>UILaunchImageSize</key>\n\t\t<string>{" + QString::number(mapImageSizes[key].width()) + ", " +
                          QString::number(mapImageSizes[key].height()) + "}</string>\n\t</dict>\n";
        }

        textStream << "</array>";
        textFile.close();

    } else {
        qDebug() << "Image file doesn't exist - " + sImageFilename;
        return -1;
    }

    return 0;
}

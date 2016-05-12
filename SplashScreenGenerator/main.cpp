#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <QDebug>

class SizeNamePair {
public:
    SizeNamePair() {}

    SizeNamePair(const QString &sFilename, const QSize& size) {
        m_sFilename = sFilename;
        m_size = size;
    }

    SizeNamePair(const SizeNamePair& rhs) {
        m_sFilename = rhs.Filename();
        m_size = rhs.Size();
    }

    QString Filename() const { return m_sFilename; }
    QSize Size() const { return m_size; }

private:
    QString m_sFilename;
    QSize m_size;
};

int main(int argc, char *argv[])
{
    if(argc != 2) {
        qDebug() << "Expected usage: \"./SplashScreenGenerator filename.png\"";
        return -1;
    }

    QCoreApplication a(argc, argv);
    QString sImageFilename = a.arguments().at(1);

    QVector<SizeNamePair> vImageSizes;
    vImageSizes.push_back(SizeNamePair("small.png", QSize(480, 640)));
    vImageSizes.push_back(SizeNamePair("large.png", QSize(1080, 1920)));

    if(QFile::exists(sImageFilename)) {
        // open the input image
        qDebug() << "Processing image - " + sImageFilename;
        QImage imageOrig(sImageFilename);

        // for all of the desired sizes
        foreach(const SizeNamePair thisSize, vImageSizes) {
            // scale the image down (make it larger to preserve aspect ratio)
            QImage thisImage = imageOrig.scaled(thisSize.Size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

            // if we had to grow the image to preserve the aspect, cut off the edges
            int iExtraWidth = thisImage.width() - thisSize.Size().width();
            int iExtraHeight = thisImage.height() - thisSize.Size().height();

            // center the image
            QRect rectCrop(iExtraWidth / 2, iExtraHeight / 2, thisSize.Size().width(), thisSize.Size().height());
            QImage thisImageCropped = thisImage.copy(rectCrop);

            // save the image
            thisImageCropped.save(thisSize.Filename());

            qDebug() << "Saved " + thisSize.Filename() + " (" + QString::number(thisSize.Size().width()) +
                        " x " + QString::number(thisSize.Size().height()) + ")";
        }

    } else {
        qDebug() << "Image file doesn't exist - " + sImageFilename;
        return -1;
    }

    return 0;
}


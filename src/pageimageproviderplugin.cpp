#include "pageimageproviderplugin.h"
#include <QDebug>

PageImageProvider::PageImageProvider(PdfRenderer *renderer)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_renderer(renderer)
{
}

QImage PageImageProvider::requestImage(const QString &id, QSize *size,
                                        const QSize &requestedSize)
{
    // Parse id: "<pageNum>" or "<pageNum>/<zoom>"
    QStringList parts = id.split('/');
    int pageNum = 0;
    qreal dpi = 0; // 0 = use renderer's zoom

    if (!parts.isEmpty())
        pageNum = parts[0].toInt();
    if (parts.size() > 1)
        dpi = 72.0 * parts[1].toDouble();

    QImage img = m_renderer->renderPage(pageNum, dpi);

    if (img.isNull()) {
        // Return a small placeholder on error
        img = QImage(100, 100, QImage::Format_Grayscale8);
        img.fill(200);
    }

    if (size)
        *size = img.size();

    // Scale if requestedSize is set (QML can request a specific size)
    if (requestedSize.isValid() && requestedSize.width() > 0 && requestedSize.height() > 0) {
        img = img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    return img;
}

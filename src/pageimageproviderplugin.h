#ifndef PAGEIMAGEPROVIDERPLUGIN_H
#define PAGEIMAGEPROVIDERPLUGIN_H

#include <QQuickImageProvider>
#include "pdfrenderer.h"

/**
 * PageImageProvider bridges PdfRenderer and QML's Image element.
 *
 * QML requests images via: "image://page/<pageNum>/<zoom>"
 * The provider renders the requested page and returns a QImage.
 */
class PageImageProvider : public QQuickImageProvider
{
public:
    explicit PageImageProvider(PdfRenderer *renderer);

    QImage requestImage(const QString &id, QSize *size,
                        const QSize &requestedSize) override;

private:
    PdfRenderer *m_renderer;
};

#endif // PAGEIMAGEPROVIDERPLUGIN_H

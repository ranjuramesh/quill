#ifndef PDFRENDERER_H
#define PDFRENDERER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QSizeF>
#include <QRectF>
#include <memory>

extern "C" {
#include <mupdf/fitz.h>
}

/**
 * PdfRenderer wraps MuPDF to provide PDF page rendering for QML.
 *
 * It manages a single open document and renders pages to QImage bitmaps.
 * All MuPDF state (context, document) is owned by this class.
 */
class PdfRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

public:
    explicit PdfRenderer(QObject *parent = nullptr);
    ~PdfRenderer() override;

    // --- Properties ---
    QString filePath() const;
    void setFilePath(const QString &path);

    int pageCount() const;

    int currentPage() const;
    void setCurrentPage(int page);

    qreal zoom() const;
    void setZoom(qreal zoom);

    // --- Rendering ---
    /**
     * Render the given page at the current zoom level.
     * Returns an 8-bit grayscale QImage.
     * @param pageNum 0-indexed page number
     * @param dpi Target DPI (default 226 for rM2, use 72*zoom for desktop)
     */
    QImage renderPage(int pageNum, qreal dpi = 0) const;

    /**
     * Get the size of a page in points (1 point = 1/72 inch).
     */
    Q_INVOKABLE QSizeF pageSize(int pageNum) const;

    /**
     * Navigate to next/previous page. Returns true if navigation occurred.
     */
    Q_INVOKABLE bool nextPage();
    Q_INVOKABLE bool previousPage();

signals:
    void filePathChanged();
    void pageCountChanged();
    void currentPageChanged();
    void zoomChanged();
    void documentLoaded();
    void documentError(const QString &error);

private:
    void closeDocument();
    bool openDocument(const QString &path);

    fz_context *m_ctx = nullptr;
    fz_document *m_doc = nullptr;

    QString m_filePath;
    int m_pageCount = 0;
    int m_currentPage = 0;
    qreal m_zoom = 1.0;
};

#endif // PDFRENDERER_H

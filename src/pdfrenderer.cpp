#include "pdfrenderer.h"
#include <QDebug>
#include <cstring>

PdfRenderer::PdfRenderer(QObject *parent)
    : QObject(parent)
{
    m_ctx = fz_new_context(nullptr, nullptr, FZ_STORE_DEFAULT);
    if (!m_ctx) {
        qCritical() << "Failed to create MuPDF context";
        return;
    }
    fz_register_document_handlers(m_ctx);
}

PdfRenderer::~PdfRenderer()
{
    closeDocument();
    if (m_ctx) {
        fz_drop_context(m_ctx);
        m_ctx = nullptr;
    }
}

// --- Properties ---

QString PdfRenderer::filePath() const
{
    return m_filePath;
}

void PdfRenderer::setFilePath(const QString &path)
{
    if (m_filePath == path)
        return;

    closeDocument();
    m_filePath = path;
    emit filePathChanged();

    if (!path.isEmpty()) {
        if (openDocument(path)) {
            emit documentLoaded();
        }
    }
}

int PdfRenderer::pageCount() const
{
    return m_pageCount;
}

int PdfRenderer::currentPage() const
{
    return m_currentPage;
}

void PdfRenderer::setCurrentPage(int page)
{
    if (page < 0 || page >= m_pageCount || page == m_currentPage)
        return;
    m_currentPage = page;
    emit currentPageChanged();
}

qreal PdfRenderer::zoom() const
{
    return m_zoom;
}

void PdfRenderer::setZoom(qreal zoom)
{
    if (zoom < 0.1)
        zoom = 0.1;
    if (zoom > 10.0)
        zoom = 10.0;
    if (qFuzzyCompare(m_zoom, zoom))
        return;
    m_zoom = zoom;
    emit zoomChanged();
}

// --- Document management ---

bool PdfRenderer::openDocument(const QString &path)
{
    if (!m_ctx)
        return false;

    fz_try(m_ctx)
    {
        m_doc = fz_open_document(m_ctx, path.toUtf8().constData());
        m_pageCount = fz_count_pages(m_ctx, m_doc);
        m_currentPage = 0;
        emit pageCountChanged();
        emit currentPageChanged();
        qDebug() << "Opened PDF:" << path << "pages:" << m_pageCount;
        return true;
    }
    fz_catch(m_ctx)
    {
        QString error = QString::fromUtf8(fz_caught_message(m_ctx));
        qWarning() << "Failed to open PDF:" << error;
        emit documentError(error);
        m_doc = nullptr;
        m_pageCount = 0;
        emit pageCountChanged();
    }
    return false;
}

void PdfRenderer::closeDocument()
{
    if (m_doc && m_ctx) {
        fz_drop_document(m_ctx, m_doc);
        m_doc = nullptr;
    }
    m_pageCount = 0;
    m_currentPage = 0;
}

// --- Rendering ---

QSizeF PdfRenderer::pageSize(int pageNum) const
{
    if (!m_doc || !m_ctx || pageNum < 0 || pageNum >= m_pageCount)
        return QSizeF();

    QSizeF size;
    fz_try(m_ctx)
    {
        fz_page *page = fz_load_page(m_ctx, m_doc, pageNum);
        fz_rect bounds = fz_bound_page(m_ctx, page);
        size = QSizeF(bounds.x1 - bounds.x0, bounds.y1 - bounds.y0);
        fz_drop_page(m_ctx, page);
    }
    fz_catch(m_ctx)
    {
        qWarning() << "Failed to get page size for page" << pageNum;
    }
    return size;
}

QImage PdfRenderer::renderPage(int pageNum, qreal dpi) const
{
    if (!m_doc || !m_ctx || pageNum < 0 || pageNum >= m_pageCount)
        return QImage();

    // Default DPI: 72 * zoom (MuPDF uses 72 DPI as base)
    if (dpi <= 0)
        dpi = 72.0 * m_zoom;

    qreal scale = dpi / 72.0;

    QImage result;

    fz_try(m_ctx)
    {
        fz_page *page = fz_load_page(m_ctx, m_doc, pageNum);
        fz_rect bounds = fz_bound_page(m_ctx, page);

        fz_matrix ctm = fz_scale(scale, scale);

        // Render to an 8-bit grayscale pixmap
        fz_colorspace *cs = fz_device_gray(m_ctx);
        fz_pixmap *pix = fz_new_pixmap_from_page(m_ctx, page, ctm, cs, 0);

        int w = fz_pixmap_width(m_ctx, pix);
        int h = fz_pixmap_height(m_ctx, pix);
        int stride = fz_pixmap_stride(m_ctx, pix);
        unsigned char *samples = fz_pixmap_samples(m_ctx, pix);

        // Create QImage from the grayscale pixmap data
        // MuPDF grayscale pixmap has 1 byte per pixel (no alpha since alpha=0 above)
        result = QImage(w, h, QImage::Format_Grayscale8);
        for (int y = 0; y < h; ++y) {
            memcpy(result.scanLine(y), samples + y * stride, w);
        }

        fz_drop_pixmap(m_ctx, pix);
        fz_drop_page(m_ctx, page);
    }
    fz_catch(m_ctx)
    {
        qWarning() << "Failed to render page" << pageNum
                    << ":" << fz_caught_message(m_ctx);
    }

    return result;
}

// --- Navigation ---

bool PdfRenderer::nextPage()
{
    if (m_currentPage + 1 < m_pageCount) {
        setCurrentPage(m_currentPage + 1);
        return true;
    }
    return false;
}

bool PdfRenderer::previousPage()
{
    if (m_currentPage > 0) {
        setCurrentPage(m_currentPage - 1);
        return true;
    }
    return false;
}

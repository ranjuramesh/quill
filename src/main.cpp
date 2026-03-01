#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "pdfrenderer.h"
#include "pageimageproviderplugin.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("Quill");
    app.setApplicationVersion("0.1.0");
    app.setOrganizationName("quill");

    PdfRenderer renderer;

    // If a PDF path is passed as argument, open it
    QStringList args = app.arguments();
    if (args.size() > 1) {
        renderer.setFilePath(args[1]);
    }

    QQmlApplicationEngine engine;

    // Register the image provider so QML can request rendered pages
    // via "image://page/<pageNum>/<zoom>"
    engine.addImageProvider("page", new PageImageProvider(&renderer));

    // Expose the renderer to QML
    engine.rootContext()->setContextProperty("pdfRenderer", &renderer);

    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML";
        return -1;
    }

    return app.exec();
}

import QtQuick 2.15

Item {
    id: readerView

    // Cache-busting counter to force image reload on page/zoom change
    property int refreshCounter: 0

    Connections {
        target: pdfRenderer
        function onCurrentPageChanged() { refreshCounter++ }
        function onZoomChanged() { refreshCounter++ }
        function onDocumentLoaded() { refreshCounter++ }
    }

    // --- Page image ---
    Image {
        id: pageImage
        anchors.centerIn: parent
        cache: false
        fillMode: Image.PreserveAspectFit

        // Request the current page from the image provider
        // The refreshCounter forces QML to re-request when page/zoom changes
        source: pdfRenderer.pageCount > 0
                ? "image://page/" + pdfRenderer.currentPage + "/" + pdfRenderer.zoom
                  + "?" + refreshCounter
                : ""

        // Fit within the view
        width: Math.min(sourceSize.width, readerView.width)
        height: Math.min(sourceSize.height, readerView.height - 60) // leave room for toolbar

        // Smooth scaling for desktop; on e-ink this won't matter
        smooth: true
        mipmap: true
    }

    // --- Touch / click navigation zones ---
    // Left 25% = previous page, Right 25% = next page, Center = toggle toolbar
    MouseArea {
        anchors.fill: parent
        onClicked: {
            var relX = mouse.x / width;
            if (relX < 0.25) {
                pdfRenderer.previousPage();
            } else if (relX > 0.75) {
                pdfRenderer.nextPage();
            }
            // Center tap: could toggle toolbar visibility (future)
        }
    }

    // --- Swipe gesture detection ---
    MouseArea {
        id: swipeArea
        anchors.fill: parent
        propagateComposedEvents: true

        property real startX: 0
        property real startY: 0

        onPressed: {
            startX = mouse.x;
            startY = mouse.y;
            mouse.accepted = false; // let clicks through
        }

        onReleased: {
            var dx = mouse.x - startX;
            var dy = mouse.y - startY;
            var absDx = Math.abs(dx);
            var absDy = Math.abs(dy);

            // Require minimum 50px horizontal swipe, mostly horizontal
            if (absDx > 50 && absDx > absDy * 1.5) {
                if (dx < 0) {
                    pdfRenderer.nextPage();
                } else {
                    pdfRenderer.previousPage();
                }
            }
        }
    }
}

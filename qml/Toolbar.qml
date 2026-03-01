import QtQuick 2.15

Rectangle {
    id: toolbar
    height: 48
    color: "#e8e8e8"
    border.color: "#ccc"
    border.width: 1

    Row {
        anchors.centerIn: parent
        spacing: 24

        // Previous page button
        Text {
            text: "◀"
            font.pixelSize: 20
            color: pdfRenderer.currentPage > 0 ? "#333" : "#bbb"
            MouseArea {
                anchors.fill: parent
                onClicked: pdfRenderer.previousPage()
            }
        }

        // Page indicator
        Text {
            text: (pdfRenderer.currentPage + 1) + " / " + pdfRenderer.pageCount
            font.pixelSize: 16
            color: "#333"
            verticalAlignment: Text.AlignVCenter
        }

        // Next page button
        Text {
            text: "▶"
            font.pixelSize: 20
            color: pdfRenderer.currentPage < pdfRenderer.pageCount - 1 ? "#333" : "#bbb"
            MouseArea {
                anchors.fill: parent
                onClicked: pdfRenderer.nextPage()
            }
        }

        // Separator
        Rectangle { width: 1; height: 24; color: "#ccc" }

        // Zoom out
        Text {
            text: "−"
            font.pixelSize: 24
            font.bold: true
            color: "#333"
            MouseArea {
                anchors.fill: parent
                onClicked: pdfRenderer.zoom = Math.max(pdfRenderer.zoom - 0.25, 0.25)
            }
        }

        // Zoom level
        Text {
            text: Math.round(pdfRenderer.zoom * 100) + "%"
            font.pixelSize: 14
            color: "#555"
            verticalAlignment: Text.AlignVCenter
        }

        // Zoom in
        Text {
            text: "+"
            font.pixelSize: 24
            font.bold: true
            color: "#333"
            MouseArea {
                anchors.fill: parent
                onClicked: pdfRenderer.zoom = Math.min(pdfRenderer.zoom + 0.25, 5.0)
            }
        }
    }
}

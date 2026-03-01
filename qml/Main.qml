import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: root
    visible: true
    width: 800
    height: 1000
    title: "Quill" + (pdfRenderer.filePath ? " — " + pdfRenderer.filePath : "")
    color: "#f0f0f0"

    // --- Welcome screen (no PDF loaded) ---
    Rectangle {
        id: welcomeScreen
        anchors.fill: parent
        visible: pdfRenderer.pageCount === 0
        color: "#f5f5f5"

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "Quill"
                font.pixelSize: 48
                font.bold: true
                color: "#333"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Lightweight PDF Reader"
                font.pixelSize: 18
                color: "#666"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "Open a PDF: quill <file.pdf>"
                font.pixelSize: 14
                color: "#999"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    // --- Reader view (PDF loaded) ---
    ReaderView {
        id: readerView
        anchors.fill: parent
        visible: pdfRenderer.pageCount > 0
    }

    // --- Bottom toolbar ---
    Toolbar {
        id: toolbar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: pdfRenderer.pageCount > 0
    }

    // --- Keyboard shortcuts ---
    Shortcut {
        sequence: "Right"
        onActivated: pdfRenderer.nextPage()
    }
    Shortcut {
        sequence: "Left"
        onActivated: pdfRenderer.previousPage()
    }
    Shortcut {
        sequence: "Space"
        onActivated: pdfRenderer.nextPage()
    }
    Shortcut {
        sequence: "+"
        onActivated: pdfRenderer.zoom = Math.min(pdfRenderer.zoom + 0.25, 5.0)
    }
    Shortcut {
        sequence: "-"
        onActivated: pdfRenderer.zoom = Math.max(pdfRenderer.zoom - 0.25, 0.25)
    }
    Shortcut {
        sequence: "Q"
        onActivated: Qt.quit()
    }
}

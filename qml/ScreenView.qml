import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int appState: (typeof emulator !== "undefined") ? emulator.appState : 0
    property bool scanlinesEnabled: true

    signal gameSelected(int index)

    // Screen Bezel Outer Frame
    Rectangle {
        anchors.fill: parent
        radius: 14
        color: "#0a0c10"
        border.color: "#222530"
        border.width: 4

        // Inner Screen Bevel
        Rectangle {
            anchors.fill: parent
            anchors.margins: 4
            radius: 10
            color: "#05070a"
            border.color: "#151820"
            border.width: 2
            clip: true

            // --- Top Screen Status Bar ---
            Item {
                id: topBar
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 22
                z: 10

                Rectangle {
                    anchors.fill: parent
                    color: "#0f131a"
                    border.color: "#1c222d"
                    border.width: 1
                }

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6

                    Rectangle {
                        width: 6
                        height: 6
                        radius: 3
                        color: "#10b981" // Power LED indicator
                    }

                    Text {
                        text: "AHMED-64 ADVANCE"
                        color: "#9ca3af"
                        font.bold: true
                        font.pixelSize: 10
                        font.family: "Consolas, monospace"
                    }
                }

                Row {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10

                    Text {
                        text: "60 FPS"
                        color: "#00f0ff"
                        font.bold: true
                        font.pixelSize: 9
                        font.family: "Consolas, monospace"
                    }

                    Text {
                        text: "BAT: 98% 🔋"
                        color: "#10b981"
                        font.pixelSize: 9
                        font.family: "Consolas, monospace"
                    }
                }
            }

            // --- Main Content Views ---
            Item {
                anchors.top: topBar.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                // Game Library View (Menu)
                GameLibrary {
                    id: libraryView
                    anchors.fill: parent
                    visible: root.appState === 0
                    onGameSelected: function(idx) {
                        root.gameSelected(idx);
                    }
                }

                // In-Game Canvas Display View
                GameDisplay {
                    id: displayView
                    anchors.fill: parent
                    visible: root.appState !== 0
                }
            }

            // --- CRT Scanline Simulation Overlay ---
            Canvas {
                id: scanlineCanvas
                anchors.fill: parent
                visible: root.scanlinesEnabled
                opacity: 0.18
                z: 20

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.clearRect(0, 0, width, height);
                    ctx.fillStyle = "#000000";
                    for (var y = 0; y < height; y += 3) {
                        ctx.fillRect(0, y, width, 1);
                    }
                }
            }

            // Glass Reflection Glare on Corner
            Rectangle {
                anchors.top: parent.top
                anchors.right: parent.right
                width: 160
                height: 160
                rotation: 45
                transformOrigin: Item.TopRight
                z: 25
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#25ffffff" }
                    GradientStop { position: 0.3; color: "#08ffffff" }
                    GradientStop { position: 1.0; color: "#00ffffff" }
                }
            }
        }
    }

    function selectNextGame() {
        if (root.appState === 0) {
            libraryView.selectNext();
        }
    }

    function selectPrevGame() {
        if (root.appState === 0) {
            libraryView.selectPrev();
        }
    }

    function launchSelectedGame() {
        if (root.appState === 0) {
            libraryView.launchCurrent();
        }
    }
}

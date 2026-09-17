import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string rawFrameText: (typeof emulator !== "undefined") ? emulator.frameText : ""
    property int score: (typeof emulator !== "undefined") ? emulator.currentScore : 0
    property bool isPaused: (typeof emulator !== "undefined") ? emulator.isPaused : false
    property bool isGameOver: (typeof emulator !== "undefined") ? emulator.isGameOver : false
    property string gameName: (typeof emulator !== "undefined") ? emulator.activeGameName : ""

    Rectangle {
        anchors.fill: parent
        color: "#080b10"
    }

    // Top In-Game Status HUD
    Item {
        id: inGameHud
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 28

        Rectangle {
            anchors.fill: parent
            color: "#121721"
            border.color: "#1f2937"
            border.width: 1
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            spacing: 8

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: root.isGameOver ? "#ef4444" : (root.isPaused ? "#f59e0b" : "#10b981")
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: root.gameName.toUpperCase()
                color: "#f3f4f6"
                font.bold: true
                font.pixelSize: 11
                font.family: "Consolas, monospace"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            spacing: 16

            Text {
                text: "SCORE: " + root.score
                color: "#eab308"
                font.bold: true
                font.pixelSize: 12
                font.family: "Consolas, monospace"
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: "ESC: Vault | P: Pause | R: Reset"
                color: "#6b7280"
                font.pixelSize: 10
                font.family: "Consolas, monospace"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    // Main Game Canvas Display Area
    Item {
        anchors.top: inGameHud.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 4

        // Retro Matrix Monospace Text Display
        Text {
            id: screenText
            anchors.fill: parent
            anchors.margins: 4
            text: root.rawFrameText
            color: "#4ade80" // Phosphor green / neon retro default
            font.family: "Consolas, 'Courier New', monospace"
            font.pixelSize: 12
            font.bold: true
            lineHeight: 1.05
            wrapMode: Text.NoWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // Pause Overlay Modal
    Rectangle {
        anchors.centerIn: parent
        width: 260
        height: 100
        radius: 10
        color: "#181d28"
        border.color: "#f59e0b"
        border.width: 2
        visible: root.isPaused && !root.isGameOver

        Column {
            anchors.centerIn: parent
            spacing: 8

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "⏸️ GAME PAUSED"
                color: "#f59e0b"
                font.bold: true
                font.pixelSize: 16
                font.family: "Segoe UI, sans-serif"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Press [ P ] or PAUSE to resume"
                color: "#9ca3af"
                font.pixelSize: 11
                font.family: "Segoe UI, sans-serif"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Press [ MENU ] or ESC to quit to Vault"
                color: "#6b7280"
                font.pixelSize: 10
                font.family: "Segoe UI, sans-serif"
            }
        }
    }

    // Game Over Overlay Modal
    Rectangle {
        anchors.centerIn: parent
        width: 280
        height: 120
        radius: 10
        color: "#1f1618"
        border.color: "#ef4444"
        border.width: 2
        visible: root.isGameOver

        Column {
            anchors.centerIn: parent
            spacing: 8

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "💀 GAME OVER"
                color: "#ef4444"
                font.bold: true
                font.pixelSize: 18
                font.family: "Segoe UI, sans-serif"
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Final Score: " + root.score
                color: "#ffffff"
                font.bold: true
                font.pixelSize: 13
                font.family: "Consolas, monospace"
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12

                Rectangle {
                    width: 90
                    height: 28
                    radius: 6
                    color: "#ef4444"

                    Text {
                        anchors.centerIn: parent
                        text: "RETRY [ R ]"
                        color: "#ffffff"
                        font.bold: true
                        font.pixelSize: 11
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (typeof emulator !== "undefined") emulator.resetGame();
                        }
                    }
                }

                Rectangle {
                    width: 90
                    height: 28
                    radius: 6
                    color: "#374151"

                    Text {
                        anchors.centerIn: parent
                        text: "VAULT [ ESC ]"
                        color: "#e5e7eb"
                        font.bold: true
                        font.pixelSize: 11
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (typeof emulator !== "undefined") emulator.returnToMenu();
                        }
                    }
                }
            }
        }
    }
}

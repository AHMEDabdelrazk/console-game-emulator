import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 140
    height: 140

    signal pressed(string direction)

    // Base background recessed bevel
    Rectangle {
        anchors.fill: parent
        radius: width / 2
        color: "#18191f"
        border.color: "#2c2e3a"
        border.width: 3

        Rectangle {
            anchors.centerIn: parent
            width: parent.width - 12
            height: parent.height - 12
            radius: width / 2
            color: "#121318"
        }
    }

    // D-Pad Cross Body
    Item {
        anchors.centerIn: parent
        width: 120
        height: 120

        // Vertical Bar
        Rectangle {
            id: vBar
            anchors.centerIn: parent
            width: 38
            height: 114
            radius: 6
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#363844" }
                GradientStop { position: 0.5; color: "#24252e" }
                GradientStop { position: 1.0; color: "#1a1b22" }
            }
            border.color: "#4a4d5e"
            border.width: 1
        }

        // Horizontal Bar
        Rectangle {
            id: hBar
            anchors.centerIn: parent
            width: 114
            height: 38
            radius: 6
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#363844" }
                GradientStop { position: 0.5; color: "#24252e" }
                GradientStop { position: 1.0; color: "#1a1b22" }
            }
            border.color: "#4a4d5e"
            border.width: 1
        }

        // Center Pivot depression
        Rectangle {
            anchors.centerIn: parent
            width: 28
            height: 28
            radius: 14
            color: "#1c1d24"
            border.color: "#31333e"
            border.width: 1

            Rectangle {
                anchors.centerIn: parent
                width: 12
                height: 12
                radius: 6
                color: "#14151a"
            }
        }

        // --- Interactive Buttons ---

        // UP
        Item {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 38
            height: 38

            Text {
                anchors.centerIn: parent
                text: "▲"
                color: upArea.pressed ? "#00f0ff" : "#888c9d"
                font.pixelSize: 12
            }

            MouseArea {
                id: upArea
                anchors.fill: parent
                onPressed: root.pressed("UP")
            }
        }

        // DOWN
        Item {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 38
            height: 38

            Text {
                anchors.centerIn: parent
                text: "▼"
                color: downArea.pressed ? "#00f0ff" : "#888c9d"
                font.pixelSize: 12
            }

            MouseArea {
                id: downArea
                anchors.fill: parent
                onPressed: root.pressed("DOWN")
            }
        }

        // LEFT
        Item {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 38
            height: 38

            Text {
                anchors.centerIn: parent
                text: "◀"
                color: leftArea.pressed ? "#00f0ff" : "#888c9d"
                font.pixelSize: 12
            }

            MouseArea {
                id: leftArea
                anchors.fill: parent
                onPressed: root.pressed("LEFT")
            }
        }

        // RIGHT
        Item {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: 38
            height: 38

            Text {
                anchors.centerIn: parent
                text: "▶"
                color: rightArea.pressed ? "#00f0ff" : "#888c9d"
                font.pixelSize: 12
            }

            MouseArea {
                id: rightArea
                anchors.fill: parent
                onPressed: root.pressed("RIGHT")
            }
        }
    }
}

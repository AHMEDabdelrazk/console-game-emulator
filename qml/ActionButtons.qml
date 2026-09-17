import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 140
    height: 140

    signal pressed(string button)

    // Base recessed housing
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

    component ArcadeBtn: Rectangle {
        id: btn
        property string label: "A"
        property color btnColor: "#22c55e"
        property color textColor: "#ffffff"
        property string actionName: "A_BTN"

        width: 36
        height: 36
        radius: 18

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: ma.pressed ? Qt.darker(btnColor, 1.4) : Qt.lighter(btnColor, 1.3)
            }
            GradientStop {
                position: 0.6
                color: btnColor
            }
            GradientStop {
                position: 1.0
                color: Qt.darker(btnColor, 1.6)
            }
        }

        border.color: Qt.darker(btnColor, 2.0)
        border.width: 1.5

        scale: ma.pressed ? 0.92 : 1.0
        Behavior on scale { NumberAnimation { duration: 60 } }

        Text {
            anchors.centerIn: parent
            text: btn.label
            font.bold: true
            font.pixelSize: 14
            font.family: "Segoe UI, sans-serif"
            color: btn.textColor
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onPressed: root.pressed(btn.actionName)
        }
    }

    // Y Button (Top)
    ArcadeBtn {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        label: "Y"
        btnColor: "#eab308" // Yellow
        actionName: "Y_BTN"
    }

    // X Button (Left)
    ArcadeBtn {
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        label: "X"
        btnColor: "#3b82f6" // Blue
        actionName: "X_BTN"
    }

    // A Button (Right)
    ArcadeBtn {
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        label: "A"
        btnColor: "#22c55e" // Green
        actionName: "A_BTN"
    }

    // B Button (Bottom)
    ArcadeBtn {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        label: "B"
        btnColor: "#ef4444" // Red
        actionName: "B_BTN"
    }
}

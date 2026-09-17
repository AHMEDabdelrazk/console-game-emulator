import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 320
    height: 48

    signal pressed(string button)

    component PillButton: Item {
        id: pill
        property string label: "START"
        property string actionName: "START"
        property color pillColor: "#474959"

        width: 56
        height: 44

        Column {
            anchors.centerIn: parent
            spacing: 4

            Rectangle {
                width: 44
                height: 14
                radius: 7
                rotation: -25
                anchors.horizontalCenter: parent.horizontalCenter

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: ma.pressed ? Qt.darker(pillColor, 1.4) : Qt.lighter(pillColor, 1.2)
                    }
                    GradientStop {
                        position: 1.0
                        color: Qt.darker(pillColor, 1.4)
                    }
                }
                border.color: "#181920"
                border.width: 1

                scale: ma.pressed ? 0.90 : 1.0
                Behavior on scale { NumberAnimation { duration: 60 } }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: pill.label
                font.pixelSize: 9
                font.bold: true
                font.family: "Segoe UI, monospace"
                color: "#9ca3af"
            }
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            onPressed: root.pressed(pill.actionName)
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 12

        PillButton {
            label: "SELECT"
            actionName: "SELECT"
        }

        PillButton {
            label: "START"
            actionName: "START"
        }

        PillButton {
            label: "MENU"
            actionName: "MENU"
            pillColor: "#6366f1"
        }

        PillButton {
            label: "PAUSE"
            actionName: "PAUSE"
            pillColor: "#f59e0b"
        }

        PillButton {
            label: "RESET"
            actionName: "RESET"
            pillColor: "#ef4444"
        }
    }
}

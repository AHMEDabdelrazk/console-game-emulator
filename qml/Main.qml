import QtQuick
import QtQuick.Controls
import QtQuick.Window

ApplicationWindow {
    id: window
    width: 1040
    height: 640
    minimumWidth: 900
    minimumHeight: 560
    visible: true
    title: "Ahmed Abdelrazik — Retro Game Emulator"
    color: "#0a0b0e"

    // --- Global Keyboard Handling ---
    Item {
        id: keyHandler
        focus: true
        anchors.fill: parent

        Keys.onPressed: function(event) {
            if (typeof emulator === "undefined") return;

            if (event.key === Qt.Key_Up || event.key === Qt.Key_W) {
                emulator.sendKeyByName("UP");
                if (screenView.appState === 0) screenView.selectPrevGame();
                event.accepted = true;
            } else if (event.key === Qt.Key_Down || event.key === Qt.Key_S) {
                emulator.sendKeyByName("DOWN");
                if (screenView.appState === 0) screenView.selectNextGame();
                event.accepted = true;
            } else if (event.key === Qt.Key_Left || event.key === Qt.Key_A) {
                emulator.sendKeyByName("LEFT");
                if (screenView.appState === 0) screenView.selectPrevGame();
                event.accepted = true;
            } else if (event.key === Qt.Key_Right || event.key === Qt.Key_D) {
                emulator.sendKeyByName("RIGHT");
                if (screenView.appState === 0) screenView.selectNextGame();
                event.accepted = true;
            } else if (event.key === Qt.Key_Space) {
                emulator.sendKeyByName("SPACE");
                if (screenView.appState === 0) screenView.launchSelectedGame();
                event.accepted = true;
            } else if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                emulator.sendKeyByName("ENTER");
                if (screenView.appState === 0) screenView.launchSelectedGame();
                event.accepted = true;
            } else if (event.key === Qt.Key_Escape) {
                emulator.sendKeyByName("MENU");
                event.accepted = true;
            } else if (event.key === Qt.Key_P) {
                emulator.sendKeyByName("PAUSE");
                event.accepted = true;
            } else if (event.key === Qt.Key_R) {
                emulator.sendKeyByName("RESET");
                event.accepted = true;
            } else if (event.key === Qt.Key_1) {
                if (screenView.appState === 0) emulator.launchGame(0);
                else emulator.sendKeyByName("1");
                event.accepted = true;
            } else if (event.key === Qt.Key_2) {
                if (screenView.appState === 0) emulator.launchGame(1);
                else emulator.sendKeyByName("2");
                event.accepted = true;
            } else if (event.key === Qt.Key_3) {
                if (screenView.appState === 0) emulator.launchGame(2);
                else emulator.sendKeyByName("3");
                event.accepted = true;
            } else if (event.key === Qt.Key_4) {
                if (screenView.appState === 0) emulator.launchGame(3);
                else emulator.sendKeyByName("4");
                event.accepted = true;
            } else if (event.key === Qt.Key_5) {
                if (screenView.appState === 0) emulator.launchGame(4);
                event.accepted = true;
            }
        }
    }

    // --- Main Handheld Chassis Housing ---
    Rectangle {
        id: consoleChassis
        anchors.centerIn: parent
        width: Math.min(parent.width - 20, 1010)
        height: Math.min(parent.height - 20, 610)
        radius: 36

        // Deep charcoal matte console casing with metallic rim
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#2d303d" }
            GradientStop { position: 0.08; color: "#1f212a" }
            GradientStop { position: 0.92; color: "#17181f" }
            GradientStop { position: 1.0; color: "#111217" }
        }
        border.color: "#3f4354"
        border.width: 3

        // Outer Screw Rivets (Retro Hardware Details)
        Repeater {
            model: [
                { x: 22, y: 22 },
                { x: consoleChassis.width - 34, y: 22 },
                { x: 22, y: consoleChassis.height - 34 },
                { x: consoleChassis.width - 34, y: consoleChassis.height - 34 }
            ]
            Rectangle {
                x: modelData.x
                y: modelData.y
                width: 12
                height: 12
                radius: 6
                color: "#181920"
                border.color: "#333745"
                border.width: 1.5

                Rectangle {
                    anchors.centerIn: parent
                    width: 6
                    height: 1.5
                    color: "#555b70"
                }
            }
        }

        // Top Shoulder Bumper Buttons (L and R Triggers)
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 80
            width: 120
            height: 8
            radius: 4
            color: "#383c4c"
            border.color: "#252733"
            Text {
                anchors.centerIn: parent
                text: "[ L ]"
                color: "#8389a0"
                font.pixelSize: 8
                font.bold: true
            }
        }

        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 80
            width: 120
            height: 8
            radius: 4
            color: "#383c4c"
            border.color: "#252733"
            Text {
                anchors.centerIn: parent
                text: "[ R ]"
                color: "#8389a0"
                font.pixelSize: 8
                font.bold: true
            }
        }

        // Device Brand Header
        Row {
            anchors.top: parent.top
            anchors.topMargin: 16
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 12

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: "#10b981"
                anchors.verticalCenter: parent.verticalCenter
                // Glowing Power LED
                Rectangle {
                    anchors.centerIn: parent
                    width: 16
                    height: 16
                    radius: 8
                    color: "#4010b981"
                }
            }

            Text {
                text: "★  A B D E L R A Z I K   H A N D H E L D   E M U L A T O R  ★"
                color: "#9ca3af"
                font.pixelSize: 11
                font.bold: true
                font.letterSpacing: 2
                font.family: "Consolas, monospace"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        // --- Left Side: D-Pad & Speaker Grille ---
        Item {
            id: leftControls
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            width: 160
            height: 380

            // Speaker Grille Left
            Column {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4
                Repeater {
                    model: 5
                    Row {
                        spacing: 4
                        Repeater {
                            model: 5
                            Rectangle {
                                width: 4
                                height: 4
                                radius: 2
                                color: "#13141a"
                            }
                        }
                    }
                }
            }

            // D-Pad Component
            DPad {
                anchors.centerIn: parent
                onPressed: function(dir) {
                    if (typeof emulator !== "undefined") {
                        emulator.sendKeyByName(dir);
                    }
                    if (screenView.appState === 0) {
                        if (dir === "LEFT" || dir === "UP") screenView.selectPrevGame();
                        if (dir === "RIGHT" || dir === "DOWN") screenView.selectNextGame();
                    }
                }
            }

            // D-PAD Label
            Text {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: "DIRECTION"
                color: "#6b7280"
                font.pixelSize: 10
                font.bold: true
                font.family: "Segoe UI, sans-serif"
            }
        }

        // --- Center: LCD Display Screen ---
        ScreenView {
            id: screenView
            anchors.centerIn: parent
            width: consoleChassis.width - 380
            height: consoleChassis.height - 120
            onGameSelected: function(idx) {
                if (typeof emulator !== "undefined") {
                    emulator.launchGame(idx);
                }
            }
        }

        // --- Right Side: Action Buttons & Speaker Grille ---
        Item {
            id: rightControls
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            width: 160
            height: 380

            // Speaker Grille Right
            Column {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4
                Repeater {
                    model: 5
                    Row {
                        spacing: 4
                        Repeater {
                            model: 5
                            Rectangle {
                                width: 4
                                height: 4
                                radius: 2
                                color: "#13141a"
                            }
                        }
                    }
                }
            }

            // Action Buttons Component
            ActionButtons {
                anchors.centerIn: parent
                onPressed: function(btnName) {
                    if (typeof emulator !== "undefined") {
                        emulator.sendKeyByName(btnName);
                    }
                    if (screenView.appState === 0 && (btnName === "A_BTN" || btnName === "B_BTN")) {
                        screenView.launchSelectedGame();
                    }
                }
            }

            // ACTION Label
            Text {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: "ACTION"
                color: "#6b7280"
                font.pixelSize: 10
                font.bold: true
                font.family: "Segoe UI, sans-serif"
            }
        }

        // --- Bottom Center: System Buttons ---
        SystemButtons {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 14
            anchors.horizontalCenter: parent.horizontalCenter
            onPressed: function(btn) {
                if (typeof emulator !== "undefined") {
                    emulator.sendKeyByName(btn);
                }
                if (btn === "START" && screenView.appState === 0) {
                    screenView.launchSelectedGame();
                }
            }
        }
    }
}

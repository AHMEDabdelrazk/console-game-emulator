import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int selectedIndex: 0
    signal gameSelected(int index)

    Rectangle {
        anchors.fill: parent
        color: "#0d1117"
    }

    // Top Header Banner
    Item {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50

        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#161b22" }
                GradientStop { position: 1.0; color: "#0d1117" }
            }
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10

            Text {
                text: "🕹️"
                font.pixelSize: 20
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: "RETRO ARCADE CARTRIDGE VAULT"
                color: "#f3f4f6"
                font.bold: true
                font.pixelSize: 15
                font.letterSpacing: 2
                font.family: "Consolas, monospace"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Text {
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            text: "SYSTEM READY [5 ROMs]"
            color: "#10b981"
            font.bold: true
            font.pixelSize: 12
            font.family: "Consolas, monospace"
        }

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: "#30363d"
        }
    }

    // Cartridges Carousel / List
    ListView {
        id: gameListView
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
        anchors.margins: 16
        orientation: ListView.Horizontal
        spacing: 16
        clip: true
        currentIndex: root.selectedIndex

        model: [
            {
                name: "Snake Classic",
                badge: "🐍",
                accent: "#10b981",
                genre: "ARCADE / SURVIVAL",
                desc: "Eat fruit, grow longer, avoid walls and your own tail! Dynamic speed curve.",
                controls: "Arrows / WASD"
            },
            {
                name: "Highway Racer",
                badge: "🏎️",
                accent: "#ef4444",
                genre: "RACING / SPEED",
                desc: "Dodge oncoming traffic across 4 lanes. Accelerate, brake, survive the highway!",
                controls: "A/D: Lanes | W/S: Speed"
            },
            {
                name: "Cubes (Tetris)",
                badge: "🧱",
                accent: "#8b5cf6",
                genre: "PUZZLE / BLOCKS",
                desc: "Rotate and stack tetromino blocks to clear lines and rack up mega bonuses!",
                controls: "Arrows: Move | Space: Drop"
            },
            {
                name: "Retro Pong",
                badge: "🏓",
                accent: "#06b6d4",
                genre: "SPORTS / ARCADE",
                desc: "Classic 1-Player vs AI Pong! Angle deflections and high-speed rally points.",
                controls: "W/S or Up/Down"
            },
            {
                name: "Sudoku 9x9",
                badge: "🔢",
                accent: "#f59e0b",
                genre: "LOGIC / BRAIN",
                desc: "Navigate 9x9 grid with arrows, place numbers 1-9 with full rule checking.",
                controls: "Arrows: Cursor | 1-4: Input"
            }
        ]

        delegate: Rectangle {
            id: card
            width: 175
            height: gameListView.height - 10
            radius: 12
            color: "#161b22"
            border.color: (gameListView.currentIndex === index) ? modelData.accent : "#30363d"
            border.width: (gameListView.currentIndex === index) ? 3 : 1

            scale: (gameListView.currentIndex === index) ? 1.02 : 0.98
            Behavior on scale { NumberAnimation { duration: 120 } }
            Behavior on border.color { ColorAnimation { duration: 150 } }

            // Cartridge Top Grip lines
            Column {
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 3
                Repeater {
                    model: 4
                    Rectangle {
                        width: 90
                        height: 2
                        radius: 1
                        color: "#21262d"
                    }
                }
            }

            // Cartridge Sticker Box Art
            Rectangle {
                id: sticker
                anchors.top: parent.top
                anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 24
                height: 90
                radius: 8
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.darker(modelData.accent, 2.5) }
                    GradientStop { position: 1.0; color: "#0d1117" }
                }
                border.color: Qt.darker(modelData.accent, 1.5)
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: modelData.badge
                    font.pixelSize: 42
                }
            }

            // Game Title
            Text {
                id: titleText
                anchors.top: sticker.bottom
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                text: modelData.name
                color: "#ffffff"
                font.bold: true
                font.pixelSize: 14
                font.family: "Segoe UI, sans-serif"
                elide: Text.ElideRight
            }

            // Genre
            Text {
                id: genreText
                anchors.top: titleText.bottom
                anchors.topMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: modelData.genre
                color: modelData.accent
                font.bold: true
                font.pixelSize: 9
                font.letterSpacing: 1
                font.family: "Consolas, monospace"
            }

            // Description
            Text {
                anchors.top: genreText.bottom
                anchors.topMargin: 6
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                text: modelData.desc
                color: "#8b949e"
                font.pixelSize: 10
                wrapMode: Text.WordWrap
                maximumLineCount: 3
                elide: Text.ElideRight
            }

            // Play Button on Card
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 24
                height: 28
                radius: 6
                color: (gameListView.currentIndex === index) ? modelData.accent : "#21262d"

                Text {
                    anchors.centerIn: parent
                    text: (gameListView.currentIndex === index) ? "▶ INSERT & PLAY" : "SELECT [ " + (index + 1) + " ]"
                    color: (gameListView.currentIndex === index) ? "#ffffff" : "#c9d1d9"
                    font.bold: true
                    font.pixelSize: 10
                    font.family: "Segoe UI, sans-serif"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.selectedIndex = index;
                        root.gameSelected(index);
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.selectedIndex = index;
                    gameListView.currentIndex = index;
                }
                onDoubleClicked: {
                    root.gameSelected(index);
                }
            }
        }
    }

    // Bottom Navigation Help Footer
    Item {
        id: footer
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 38

        Rectangle {
            anchors.fill: parent
            color: "#161b22"
        }

        Row {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "◄ / ► or 1-5: Choose Game"
                color: "#8b949e"
                font.pixelSize: 11
                font.family: "Consolas, monospace"
            }

            Text {
                text: "•"
                color: "#484f58"
                font.pixelSize: 12
            }

            Text {
                text: "START / A: Launch Game"
                color: "#00f0ff"
                font.bold: true
                font.pixelSize: 11
                font.family: "Consolas, monospace"
            }
        }
    }

    function selectNext() {
        if (root.selectedIndex < 4) {
            root.selectedIndex++;
            gameListView.currentIndex = root.selectedIndex;
        }
    }

    function selectPrev() {
        if (root.selectedIndex > 0) {
            root.selectedIndex--;
            gameListView.currentIndex = root.selectedIndex;
        }
    }

    function launchCurrent() {
        root.gameSelected(root.selectedIndex);
    }
}

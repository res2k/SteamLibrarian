import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: applicationWindow1
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    Grid {
        id: grid1
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        antialiasing: false
        spacing: 6
        columns: 2
        anchors.rightMargin: 6
        anchors.leftMargin: 6
        anchors.topMargin: 6

        Label {
            id: label1
            text: qsTr("Steam location:")
        }

        Label {
            id: steamLabel
            text: piping.steamInstallPath
        }

        Label {
            id: label2
            text: qsTr("Steam running:")
        }

        Label {
            text: piping.steamRunning
        }

    }
    Flow {
        anchors.left: grid1.left
        anchors.leftMargin: 0
        anchors.top: grid1.bottom
        anchors.topMargin: 6
        spacing: 3
        enabled: (piping.steamInstallPath != "")

        Button {
            text: "Start"
            enabled: (!piping.steamRunning && !piping.actionPending)
            onClicked: piping.steamStart()
        }
        Button {
            text: "Stop"
            enabled: (piping.steamRunning && !piping.actionPending)
            onClicked: piping.steamShutdown()
        }
    }



}

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
        spacing: 6
        rows: 2
        columns: 2
        anchors.rightMargin: 6
        anchors.leftMargin: 6
        anchors.bottomMargin: 6
        anchors.topMargin: 6
        anchors.fill: parent

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

}

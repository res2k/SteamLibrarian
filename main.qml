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

    Label {
        id: label1
        text: qsTr("Steam:")
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 6
    }

    Label {
        id: steamLabel
        text: piping.steamInstallPath
        anchors.top: label1.top
        anchors.topMargin: 0
        anchors.left: label1.right
        anchors.leftMargin: 6
    }

}

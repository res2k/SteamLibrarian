import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    id: moveForm
    width: 400
    height: columnLayout.implicitHeight

    property bool isUndo: false
    property alias columnLayout: columnLayout
    property alias busy: busy

    ColumnLayout {
        id: columnLayout
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 8
        anchors.left: busy.right
        anchors.right: parent.right
        anchors.top: parent.top

        Label {
            id: label1
            Layout.fillWidth: true
            text: isUndo ? qsTr("Undoing application move, please wait...") : qsTr("Moving application, please wait...")
            anchors.left: parent.left
            anchors.leftMargin: 0
            wrapMode: Text.WordWrap
        }
    }

    BusyIndicator { id: busy; width: 16; height: 16; anchors.verticalCenter: parent.verticalCenter; running: true }
}


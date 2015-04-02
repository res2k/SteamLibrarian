import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 400
    height: columnLayout.implicitHeight

    property bool isUndo: false

    ColumnLayout {
        id: columnLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Label {
            id: label1
            Layout.fillWidth: true
            text: isUndo ? qsTr("Undoing application move, please wait...") : qsTr("Moving application, please wait...")
            wrapMode: Text.WordWrap
        }
    }
}


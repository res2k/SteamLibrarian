import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    width: 400
    height: columnLayout.implicitHeight

    property alias exitButton: exitButton
    property alias cancelButton: cancelButton

    ColumnLayout {
        id: columnLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Label {
            id: label1
            Layout.fillWidth: true
            text: qsTr("Steam is currently running. To move an aplication between libraries, Steam must exit first.\nShut down Steam now?")
            wrapMode: Text.WordWrap
        }

        RowLayout {
            id: rowLayout1
            Layout.alignment: Qt.AlignRight
            width: implicitWidth
            height: implicitHeight

            Button {
                id: exitButton
                text: qsTr("Shut down Steam")
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel")
            }
        }
    }
}


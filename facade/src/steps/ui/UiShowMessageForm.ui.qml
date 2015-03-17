import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 400
    height: columnLayout.implicitHeight

    property string message
    property alias okButton: okButton

    ColumnLayout {
        id: columnLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Label {
            Layout.fillWidth: true
            text: qsTr("An error occured:")
            wrapMode: Text.WordWrap
        }

        Label {
            id: label1
            Layout.fillWidth: true
            text: message
            wrapMode: Text.WordWrap
        }

        Button {
            id: okButton
            Layout.alignment: Qt.AlignRight
            text: qsTr("OK")
        }

    }
}


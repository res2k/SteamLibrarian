import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    width: 400
    height: columnLayout.implicitHeight

    property string moveError
    property alias undoButton: undoButton
    property alias cancelButton: cancelButton

    ColumnLayout {
        id: columnLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Label {
            id: label1
            Layout.fillWidth: true
            text: qsTr("The application couldn't be moved:")
            wrapMode: Text.WordWrap
        }

        Label {
            id: moveErrorLabel
            text: moveError
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        Label {
            id: label2
            Layout.fillWidth: true
            text: qsTr("Do you want to undo the partial application move?")
            wrapMode: Text.WordWrap
        }

        RowLayout {
            id: rowLayout1
            Layout.alignment: Qt.AlignRight
            width: implicitWidth
            height: implicitHeight

            Button {
                id: undoButton
                text: qsTr("Undo")
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel")
            }
        }
    }
}


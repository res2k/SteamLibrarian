/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

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
        anchors.leftMargin: 8
        anchors.left: image1.right
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

    Image {
        id: image1
        width: 32
        height: 32
        sourceSize.height: 32
        sourceSize.width: 32
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        fillMode: Image.Stretch
        source: "image://standard/MessageBoxWarning"
    }
}


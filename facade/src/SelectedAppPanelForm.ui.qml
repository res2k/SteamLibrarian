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
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0

Item {
    id: selectedAppPanel
    width: 400
    height: grid.implicitHeight + grid.anchors.topMargin + grid.anchors.bottomMargin

    property var app: null
    property int appLibIndex: -1
    property var libsModel
    property alias gameLabel: gameLabel
    property alias libCombo: libCombo
    property alias moveButton: moveButton
    property alias sizeLabel: sizeLabelActual

    Rectangle {
        id: rectangle1
        color: palette.button
        border.width: 0
        anchors.fill: parent

        GridLayout {
            id: grid

            anchors.top: parent.top
            anchors.topMargin: 6
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 6
            anchors.right: parent.right
            anchors.rightMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 6
            columns: 3
            rows: 2

            Label {
                id: gameLabel
                Layout.columnSpan: 3
                Layout.fillWidth: true
                text: qsTr("Select a game.")
                elide: Text.ElideRight

                property real largeFontSize: font.pointSize*1.25
            }

            Label {
                id: sizeLabel
                visible: false
                text: qsTr("Size:")
            }

            Label {
                id: sizeLabelActual
                Layout.columnSpan: 2
                visible: false
                text: qsTr("")
            }

            Label {
                id: libLabel
                visible: false
                text: qsTr("Library:")
            }

            LibrariesCombo {
                id: libCombo
                visible: false
                Layout.fillWidth: true
                libsModel: selectedAppPanel.libsModel
                textRole: "name"
            }

            Button {
                id: moveButton
                visible: false
                text: qsTr("&Move")
                enabled: libCombo.currentIndex != selectedAppPanel.appLibIndex
            }
        }
    }

    states: [
        State {
            name: "GAME_SELECTED"
            when: selectedAppPanel.app != null
            PropertyChanges { target: gameLabel; text: selectedAppPanel.app.name; font.pointSize: gameLabel.largeFontSize }
            PropertyChanges { target: sizeLabel; visible: true }
            PropertyChanges { target: sizeLabelActual; visible: true }
            PropertyChanges { target: libLabel; visible: true }
            PropertyChanges { target: libCombo; visible: true }
            PropertyChanges { target: moveButton; visible: true }
        }
    ]
}

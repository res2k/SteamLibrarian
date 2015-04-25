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


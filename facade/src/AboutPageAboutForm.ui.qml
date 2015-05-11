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

Item {
    width: 600
    height: 400

    property alias appNameLabel: appNameLabel
    property alias versionLabel: versionLabel
    property alias licenseLabel: licenseLabel

    Image {
        id: logoImage
        width: 192
        height: 192
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        fillMode: Image.Stretch
        source: "data/icon.svg"
        sourceSize.width: width
        sourceSize.height: height
    }

    Label {
        id: appNameLabel
        text: qsTr("SteamLibrarian")
        anchors.top: logoImage.top
        anchors.topMargin: 0
        anchors.left: logoImage.right
        anchors.leftMargin: 10
    }

    Label {
        id: versionLabel
        text: qsTr("Version %1")
        anchors.left: appNameLabel.left
        anchors.leftMargin: 0
        anchors.top: appNameLabel.bottom
        anchors.topMargin: 6
    }

    Label {
        id: licenseLabel
        text: qsTr("Licensed under the terms of the <a href=\"tab://license\">GNU General Public License version 3</a> (or later).")
        anchors.right: parent.right
        anchors.rightMargin: 10
        wrapMode: Text.WordWrap
        anchors.top: versionLabel.bottom
        anchors.topMargin: 10
        anchors.left: versionLabel.left
    }
}


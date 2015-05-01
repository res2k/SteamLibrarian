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
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls.Private 1.0

Item {
    id: aboutPanel
    width: 600
    height: 400

    property alias appNameLabel: appNameLabel
    property alias versionLabel: versionLabel
    property alias backButton: backButton
    property string uiFontFace

    Item {
        id: topBar

        anchors.left: parent.left
        anchors.leftMargin: 6
        anchors.top: parent.top
        anchors.topMargin: 6
        height: backButton.height



        ToolButton {
            id: backButton
            text: "<"
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            style: ToolButtonStyle
            {
                property Component panel: Item {
                    id: item
                    anchors.fill: parent
                    implicitWidth: 32
                    implicitHeight: 32

                    StyleItem {
                        id: styleitem

                        anchors.fill: parent
                        elementType: "toolbutton"
                        on: control.checkable && control.checked
                        sunken: control.pressed
                        raised: !(control.checkable && control.checked) && control.hovered
                        hover: control.hovered
                        hasFocus: control.activeFocus
                        hints: control.styleHints

                        properties: {
                            "icon": control.__iconAction.__icon,
                            "position": control.__position,
                            "menu" : control.menu !== null
                        }
                    }
                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: control.text
                        font.pixelSize: 24
                        font.family: aboutPanel.uiFontFace
                    }
                }
            }
        }

    }

    Item {
        id: content
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: topBar.bottom
        anchors.topMargin: 0

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
            text: qsTr("Version 1.0")
            anchors.left: appNameLabel.left
            anchors.leftMargin: 0
            anchors.top: appNameLabel.bottom
            anchors.topMargin: 6
        }

        Label {
            id: text1
            text: qsTr("Licensed under the terms of the GNU General Public License version 3 (or later).")
            anchors.right: parent.right
            anchors.rightMargin: 10
            wrapMode: Text.WordWrap
            anchors.top: versionLabel.bottom
            anchors.topMargin: 10
            anchors.left: versionLabel.left
        }
    }
}


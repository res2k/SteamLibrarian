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
import QtQuick.Controls.Styles 1.3
//import QtQuick.Controls.Styles.Desktop 1.0
import QtQuick.Controls.Private 1.0

Button {
    // Tweak style to get appearance of a tool button
    style: ButtonStyle {
        background: Item {
            id: backgroundItem
            implicitWidth: Math.round(TextSingleton.implicitHeight * 4.5)
            implicitHeight: Math.max(25, Math.round(TextSingleton.implicitHeight * 1.2))
            StyleItem {
                id: styleitem
                anchors.fill: parent
                elementType: "toolbutton"
                sunken: control.pressed || (control.checkable && control.checked)
                raised: !(control.pressed || (control.checkable && control.checked))
                hover: control.hovered
                hasFocus: control.activeFocus
                hints: control.styleHints
                // If no icon, let the style do the drawing
                activeControl: control.isDefault ? "default" : "f"

                properties: {
                    "icon": control.__iconAction.__icon,
                    "menu": control.menu
                }
            }
            Image {
                id: imageItem
                visible: control.menu !== null
                source: "image://standard/IndicatorArrowDown?" + (!control.enabled ? "disabled," : "")
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 1
                anchors.right: parent.right
                anchors.rightMargin: padding.right
            }
        }
    }
}

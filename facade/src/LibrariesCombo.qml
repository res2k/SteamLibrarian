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

import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls.Private 1.0
import SteamLibrarian 1.0

ComboBox {
    id: libCombo
    textRole: "name"

    property var libsModel

    function selectLibrary(lib)
    {
        libCombo.currentIndex = libsModelProxy.find(lib);
    }

    function selectedLib()
    {
        return libsModel.get(libsModelProxy.sourceIndex(libCombo.currentIndex));
    }

    model: SortFilterProxyModel {
        id: libsModelProxy
        source: libsModel.count > 0 ? libsModel : null

        sortOrder: Qt.AscendingOrder
        sortCaseSensitivity: Qt.CaseInsensitive
        sortRole: libsModel.count > 0 ? "name" : ""

        function find(obj) {
            return source != null ? proxyIndex(source.find(obj)) : -1;
        }
    }

    /* FIXME: Lots of copy and paste from the desktop style as I didn't
       find a more elegant way */
    style: ComboBoxStyle {
        property Component panel: Item {
            property bool popup: !!styleItem.styleHint("comboboxpopup")
            property color textColor: SystemPaletteSingleton.text(control.enabled)
            property color selectionColor: SystemPaletteSingleton.highlight(control.enabled)
            property color selectedTextColor: SystemPaletteSingleton.highlightedText(control.enabled)
            property int dropDownButtonWidth: 24

            implicitWidth: 125
            implicitHeight: styleItem.implicitHeight
            baselineOffset: styleItem.baselineOffset
            anchors.fill: parent
            StyleItem {
                id: styleItem

                height: parent.height
                width: parent.width
                elementType: "combobox"
                sunken: control.pressed
                raised: !sunken
                hover: control.hovered
                enabled: control.enabled
                // The style makes sure the text rendering won't overlap the decoration.
                // In that case, 35 pixels margin in this case looks good enough. Worst
                // case, the ellipsis will be truncated (2nd worst, not visible at all).
                //text: elidedText(control.currentText, Text.ElideRight, parent.width - 35)
                hasFocus: control.activeFocus
                // contentHeight as in QComboBox
                contentHeight: Math.max(Math.ceil(textHeight("")), 14) + 2

                hints: control.styleHints
                properties: {
                    "popup": control.__popup,
                    "editable" : control.editable
                }

                StyleItem {
                    id: panelTextItem
                    anchors.verticalCenter: parent.verticalCenter
                    elementType: "item"
                    contentWidth: textWidth(text)
                    contentHeight: textHeight(text)
                    text: elidedText(control.currentText, Text.ElideRight, parent.width - 35)
                    selected: parent ? parent.selected : false
                }
                StyleItem {
                    id: panelAvailableItem
                    visible: parent.width-35 > panelTextItem.contentWidth + contentWidth
                    anchors.right: parent.right
                    anchors.rightMargin: dropDownButtonWidth
                    anchors.verticalCenter: parent.verticalCenter
                    elementType: "item"
                    contentWidth: textWidth(text)
                    contentHeight: textHeight(text)
                    text: qsTr("%1 free").arg(libsModelProxy.get(control.currentIndex).available)
                    selected: parent ? parent.selected : false
                    opacity: 0.5
                }
            }
        }

        property Component __dropDownStyle: Style {
            id: dropDownStyleRoot
            property int __maxPopupHeight: 600
            property int submenuOverlap: 0
            property int submenuPopupDelay: 0

            property Component frame: StyleItem {
                elementType: "frame"
                Component.onCompleted: {
                    var defaultFrameWidth = pixelMetric("defaultframewidth")
                    dropDownStyleRoot.padding.left = defaultFrameWidth
                    dropDownStyleRoot.padding.right = defaultFrameWidth
                    dropDownStyleRoot.padding.top = defaultFrameWidth
                    dropDownStyleRoot.padding.bottom = defaultFrameWidth
                }
            }

            property Component menuItemPanel: StyleItem {
                elementType: "itemrow"
                selected: styleData.selected

                implicitWidth: textItem.implicitWidth + 4 + availableItem.implicitWidth + dropDownButtonWidth
                implicitHeight: textItem.implicitHeight

                StyleItem {
                    id: textItem
                    elementType: "item"
                    contentWidth: textWidth(text)
                    contentHeight: textHeight(text)
                    text: styleData.text
                    selected: parent ? parent.selected : false
                }
                StyleItem {
                    id: availableItem
                    anchors.right: parent.right
                    anchors.rightMargin: dropDownButtonWidth
                    anchors.verticalCenter: parent.verticalCenter
                    elementType: "item"
                    contentWidth: textWidth(text)
                    contentHeight: textHeight(text)
                    text: qsTr("%1 free").arg(libsModelProxy.get(__menuItemIndex).available)
                    selected: parent ? parent.selected : false
                    opacity: 0.5
                }
            }

            property Component __scrollerStyle: ScrollViewStyle { }
        }
    }
}


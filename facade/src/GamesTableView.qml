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
import SteamLibrarian 1.0

TableView {
    id: tableView
    highlightOnFocus: false
    alternatingRowColors: true
    backgroundVisible: true

    sortIndicatorVisible: true

    TableViewColumn {
        id: appColumn
        title: "Application"
        role: "app"
        movable: false
        resizable: true
        width: tableView.viewport.width - (libColumn.width + sizeOnDiskColumn.width)
    }

    TableViewColumn {
        id: sizeOnDiskColumn
        title: "Size on disk"
        role: "sizeOnDiskStr"
        movable: false
        resizable: true
        width: 100

        property string sortRole: "sizeOnDisk"
    }

    TableViewColumn {
        id: libColumn
        title: "Library"
        role: "library"
        movable: false
        resizable: true
        width: tableView.viewport.width / 3
    }

    function libraryTint(libStr, baseColor) {
        var n = 0;
        for (var x = 0; x < libStr.length; x++)
        {
            var c = libStr.charCodeAt(x);
            n += c;
        }
        // Avoid reddish colors
        var tintColor = Qt.hsla(((n % 10)+1) / 12.0, 1.0, 0.5, 0.5);
        return Qt.tint(baseColor, tintColor);
    }

    itemDelegate: Item {
        Label {
            anchors.fill: parent
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 8
            color: {
                return (tableView.getColumn(styleData.column).role === "library"  && !styleData.selected)
                   ? tableView.libraryTint(styleData.value, styleData.textColor) : styleData.textColor;
            }
            elide: styleData.elideMode
            text: styleData.value
        }
    }

    model: SortFilterProxyModel {
        id: proxyModel
        source: appsModel.count > 0 ? appsModel : null

        sortOrder: tableView.sortIndicatorOrder
        sortCaseSensitivity: Qt.CaseInsensitive
        sortRole: {
            if (appsModel.count > 0)
            {
                var col = tableView.getColumn(tableView.sortIndicatorColumn);
                if (col.sortRole !== undefined) return col.sortRole;
                return col.role;
            }
            else
            {
                return "";
            }
        }
    }

    AppsModel {
        id: appsModel
        libraries: Piping.libraries
    }

    selection.onSelectionChanged: {
        selectedAppPanel.app = (tableView.currentRow >= 0) ? appsModel.get(proxyModel.sourceIndex(tableView.currentRow)) : null;
    }
}

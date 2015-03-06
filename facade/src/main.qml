import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import SteamLibrarian 1.0
import "js/RedBlackTree.js" as RedBlackTree

ApplicationWindow {
    id: applicationWindow1
    title: qsTr("Steam Librarian")
    width: 640
    height: 480
    visible: true

    property var libs: new RedBlackTree.RedBlackTree()

    function addLibrary(lib) {
        libs.insert(lib, new RedBlackTree.RedBlackTree());
        for (var a = 0; a < lib.count(); a++)
        {
            addApp(lib, lib.get(a));
        }
    }

    function addApp(lib, app) {
        var modelIndex = 0;
        modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        appsTree.insert(app, null);
        modelIndex += appsTree.querySortedIndex(app);
        appsModel.insert(modelIndex, {"app": app.name, "library": lib.displayName});
        libs.setUserCount(lib, appsTree.count());
    }

    Component.onCompleted: {
        for (var l = 0; l < Piping.libraries.count(); l++)
        {
            var lib = Piping.libraries.get(l);
            addLibrary(lib);
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    Grid {
        id: grid1
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        antialiasing: false
        spacing: 6
        columns: 2
        anchors.rightMargin: 6
        anchors.leftMargin: 6
        anchors.topMargin: 6

        Label {
            id: label1
            text: qsTr("Steam location:")
        }

        Label {
            id: steamLabel
            text: Piping.steamInstallPath
        }

        Label {
            id: label2
            text: qsTr("Steam running:")
        }

        Label {
            text: Piping.steamRunning
        }

    }
    Flow {
        id: flow
        anchors.left: grid1.left
        anchors.leftMargin: 0
        anchors.top: grid1.bottom
        anchors.topMargin: 6
        spacing: 3
        enabled: (Piping.steamInstallPath != "")

        Button {
            text: "Start"
            enabled: (!Piping.steamRunning && !Piping.actionPending)
            onClicked: Piping.steamStart()
        }
        Button {
            text: "Stop"
            enabled: (Piping.steamRunning && !Piping.actionPending)
            onClicked: Piping.steamShutdown()
        }
    }

    TableView {
        id: tableView
        highlightOnFocus: false
        alternatingRowColors: true
        backgroundVisible: true
        anchors.rightMargin: 6
        anchors.leftMargin: 6
        anchors.bottomMargin: 6
        anchors.top: flow.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 6

        frameVisible: false
        sortIndicatorVisible: true


        /*Layout.minimumWidth: 400
        Layout.minimumHeight: 240
        Layout.preferredWidth: 600
        Layout.preferredHeight: 400*/

        TableViewColumn {
            id: appColumn
            title: "App"
            role: "app"
            movable: false
            resizable: true
            width: tableView.viewport.width / 3
        }

        TableViewColumn {
            id: libColumn
            title: "Library"
            role: "library"
            movable: false
            resizable: true
            width: tableView.viewport.width - appColumn.width
        }

        model: SortFilterProxyModel {
            id: proxyModel
            source: appsModel.count > 0 ? appsModel : null

            sortOrder: tableView.sortIndicatorOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            sortRole: appsModel.count > 0 ? tableView.getColumn(tableView.sortIndicatorColumn).role : ""
        }

        ListModel {
            id: appsModel
        }
    }
}

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

    // Can't add bindings as a ListModel element (I think), so update manually
    function setAppName(app) {
        var lib = app.library;
        var modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        modelIndex += appsTree.querySortedIndex(app);
        appsModel.set(modelIndex, {"app": app.name, "library": lib.displayName});
    }

    function addOneApp(lib, app) {
        var modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        appsTree.insert(app, null);
        modelIndex += appsTree.querySortedIndex(app);
        appsModel.insert(modelIndex, {"app": app.name, "library": lib.displayName});
        libs.setUserCount(lib, appsTree.count());
        app.nameChanged.connect(function() { setAppName(app); });
    }

    function addApp(app) {
        var lib = app.library;
        addOneApp(lib, app);
    }

    function removeApp(app) {
        var lib = app.library;
        var modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        modelIndex += appsTree.querySortedIndex(app);
        appsTree.remove(app);
        appsModel.remove(modelIndex, 1);
        libs.setUserCount(lib, appsTree.count());
    }

    function addLibrary(lib) {
        libs.insert(lib, new RedBlackTree.RedBlackTree());
        for (var a = 0; a < lib.count(); a++)
        {
            addOneApp(lib, lib.get(a));
        }
        lib.appAdd.connect(addApp);
        lib.appRemove.connect(removeApp);
    }

    function removeLibrary(lib) {
        var modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        var numApps = appsTree.count();
        libs.remove(lib);
        appsModel.remove(modelIndex, numApps);
    }

    Component.onCompleted: {
        for (var l = 0; l < Piping.libraries.count(); l++)
        {
            var lib = Piping.libraries.get(l);
            addLibrary(lib);
        }
        Piping.libraries.libraryAdd.connect(addLibrary);
        Piping.libraries.libraryRemove.connect(removeLibrary);
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Tools")
            MenuItem {
                text: qsTr("St&art Steam")
                enabled: (!Piping.steamRunning && !Piping.actionPending)
                onTriggered: Piping.steamStart()
            }
            MenuItem {
                text: qsTr("St&op Steam")
                enabled: (Piping.steamRunning && !Piping.actionPending)
                onTriggered: Piping.steamShutdown()
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
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
        anchors.top: parent.top
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

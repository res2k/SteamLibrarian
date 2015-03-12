import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import SteamLibrarian 1.0
import "js/RedBlackTree.js" as RedBlackTree

ApplicationWindow {
    SystemPalette { id: palette; }

    id: applicationWindow1
    title: qsTr("Steam Librarian")
    width: 640
    height: 480
    style: ApplicationWindowStyle {
        background: Rectangle { color: palette.base }
    }
    visible: true

    property var libs: new RedBlackTree.RedBlackTree()

    // Can't add bindings as a ListModel element (I think), so update manually
    function setAppName(app) {
        var lib = app.library;
        var modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        modelIndex += appsTree.querySortedIndex(app);
        appsModel.set(modelIndex, {"app": app.name, "library": lib.displayName, "appObj": app});
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

        var libsModelIndex = libs.querySortedIndex(lib);
        libsModel.insert(libsModelIndex, {"lib": lib, "name": lib.displayName});
    }

    function removeLibrary(lib) {
        var modelIndex = libs.queryUserCount(lib);
        var appsTree = libs.queryValue(lib);
        var numApps = appsTree.count();
        libs.remove(lib);
        appsModel.remove(modelIndex, numApps);

        var libsModelIndex = libs.querySortedIndex(lib);
        libsModel.remove(libsModelIndex, 1);
    }

    Component.onCompleted: {
        selectedAppPanel.libsModel = libsModel;

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
        anchors.bottom: selectedAppPanel.top
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

        selection.onSelectionChanged: {
            selectedAppPanel.app = (tableView.currentRow >= 0) ? appsModel.get(proxyModel.sourceIndex(tableView.currentRow)).appObj : null;
        }
    }

    ListModel {
        id: libsModel

        function find(obj) {
            return libs.querySortedIndex(obj);
        }
    }

    Item {
        id: selectedAppPanel
        y: 419
        height: grid.implicitHeight + grid.anchors.topMargin + grid.anchors.bottomMargin
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Behavior on height {
            NumberAnimation {
                easing { type: Easing.InOutQuad }
            }
        }

        property var libsModel: null
        property var app: null
        property int appLibIndex: -1
        onAppChanged: {
            // Select app's current library in combo
            if (app != null)
            {
                libCombo.currentIndex = appLibIndex = libsModelProxy.find(app.library);
            }
        }

        SortFilterProxyModel {
            id: libsModelProxy
            source: libsModel.count > 0 ? libsModel : null

            sortOrder: Qt.AscendingOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            sortRole: libsModel.count > 0 ? "name" : ""

            function find(obj) {
                return source != null ? proxyIndex(source.find(obj)) : -1;
            }
        }

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
                    font.pointSize: 10
                }

                Label {
                    id: libLabel
                    visible: false
                    text: qsTr("Library:")
                }

                ComboBox {
                    id: libCombo
                    visible: false
                    Layout.fillWidth: true
                    model: libsModelProxy
                    textRole: "name"
                }

                Button {
                    id: moveButton
                    visible: false
                    text: qsTr("&Move")
                    enabled: libCombo.currentIndex != selectedAppPanel.appLibIndex
                }
            }

            states: [
                State {
                    name: "GAME_SELECTED"
                    when: selectedAppPanel.app != null
                    PropertyChanges { target: gameLabel; text: selectedAppPanel.app.name }
                    PropertyChanges { target: libLabel; visible: true }
                    PropertyChanges { target: libCombo; visible: true }
                    PropertyChanges { target: moveButton; visible: true }
                }

            ]
        }

    }
}

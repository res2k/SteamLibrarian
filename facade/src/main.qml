import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import SteamLibrarian 1.0

ApplicationWindow {
    id: applicationWindow1
    title: qsTr("Steam Librarian")
    width: 640
    height: 480
    visible: true

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
            id: titleColumn
            title: "Title"
            role: "title"
            movable: false
            resizable: false
            width: tableView.viewport.width - authorColumn.width
        }

        TableViewColumn {
            id: authorColumn
            title: "Author"
            role: "author"
            movable: false
            resizable: false
            width: tableView.viewport.width / 3
        }

        model: SortFilterProxyModel {
            id: proxyModel
            source: sourceModel.count > 0 ? sourceModel : null

            sortOrder: tableView.sortIndicatorOrder
            sortCaseSensitivity: Qt.CaseInsensitive
            sortRole: sourceModel.count > 0 ? tableView.getColumn(tableView.sortIndicatorColumn).role : ""
        }

        ListModel {
            id: sourceModel
            ListElement {
                title: "Moby-Dick"
                author: "Herman Melville"
            }
            ListElement {
                title: "The Adventures of Tom Sawyer"
                author: "Mark Twain"
            }
            ListElement {
                title: "Cat’s Cradle"
                author: "Kurt Vonnegut"
            }
            ListElement {
                title: "Farenheit 451"
                author: "Ray Bradbury"
            }
            ListElement {
                title: "It"
                author: "Stephen King"
            }
            ListElement {
                title: "On the Road"
                author: "Jack Kerouac"
            }
            ListElement {
                title: "Of Mice and Men"
                author: "John Steinbeck"
            }
            ListElement {
                title: "Do Androids Dream of Electric Sheep?"
                author: "Philip K. Dick"
            }
            ListElement {
                title: "Uncle Tom’s Cabin"
                author: "Harriet Beecher Stowe"
            }
            ListElement {
                title: "The Call of the Wild"
                author: "Jack London"
            }
            ListElement {
                title: "The Old Man and the Sea"
                author: "Ernest Hemingway"
            }
            ListElement {
                title: "A Streetcar Named Desire"
                author: "Tennessee Williams"
            }
            ListElement {
                title: "Catch-22"
                author: "Joseph Heller"
            }
            ListElement {
                title: "One Flew Over the Cuckoo’s Nest"
                author: "Ken Kesey"
            }
            ListElement {
                title: "The Murders in the Rue Morgue"
                author: "Edgar Allan Poe"
            }
            ListElement {
                title: "Breakfast at Tiffany’s"
                author: "Truman Capote"
            }
            ListElement {
                title: "Death of a Salesman"
                author: "Arthur Miller"
            }
            ListElement {
                title: "Post Office"
                author: "Charles Bukowski"
            }
            ListElement {
                title: "Herbert West—Reanimator"
                author: "H. P. Lovecraft"
            }
        }
    }
}

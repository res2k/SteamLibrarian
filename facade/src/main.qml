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
import QtQuick.Controls.Styles.Desktop 1.0
import QtQuick.Controls.Private 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import SteamLibrarian 1.0

Window {
    SystemPalette { id: palette; }

    id: applicationWindow1
    title: qsTr("Steam Librarian")
    width: 640
    height: 480
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Item {
            id: appsListPage
            width: parent.width
            height: parent.height

            RowLayout {
                id: rowLayout1
                height: implicitHeight
                anchors.right: parent.right
                anchors.rightMargin: 6
                anchors.left: parent.left
                anchors.leftMargin: 6
                anchors.top: parent.top
                anchors.topMargin: 6

                MenuPopupButton {
                    id: toolButton1
                    text: "Tools"
                    menu: Menu {
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
            }

            GamesTableView {
                id: tableView
                anchors.rightMargin: 6
                anchors.leftMargin: 6
                anchors.bottomMargin: 6
                anchors.top: rowLayout1.bottom
                anchors.right: parent.right
                anchors.bottom: selectedAppPanel.top
                anchors.left: parent.left
                anchors.topMargin: 6

                frameVisible: false
            }

            LibrariesModel {
                id: libsModel
                libraries: Piping.libraries
            }

            SelectedAppPanel {
                id: selectedAppPanel
                y: 419
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0

                libsModel: libsModel

                onMoveClicked: {
                    stackView.push(workPanel);
                    //workPanel.addStep(Qt.createComponent("steps/StepWait.qml"));
                    var stepComp = Qt.createComponent("steps/StepWaitSteamStop.qml");
                    workPanel.addStep(stepComp);
                    var moveComp = Qt.createComponent("steps/StepPerformMove.qml");
                    workPanel.addStep(moveComp,
                                      {"app": selectedAppPanel.app,
                                       "destinationLib": selectedLib()});
                }
            }
        }
    }

    WorkPanel {
        id: workPanel
        visible: false
        width: parent.width
        height: parent.height

    }
}

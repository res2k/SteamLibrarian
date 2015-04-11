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

                Button {
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

                    // TODO: Appearance of a tool button
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
                anchors.top: rowLayout1.bottom
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

    Item {
        id: workPanel
        visible: false
        width: parent.width
        height: parent.height

        Timer {
            id: triggerTimer
            interval: 0
        }

        Component.onCompleted: {
            triggerTimer.triggered.connect(nextStep);
        }

        property var steps: []
        property var currentStep: null
        property var currentItem: null

        function addStep(comp, props) {
            if (comp.status !== Component.Ready)
            {
                console.log(comp.errorString());
                return;
            }
            var step = comp.createObject(workPanel);
            if (props !== undefined)
            {
                for (var key in props)
                {
                    step[key] = props[key];
                }
            }
            steps.push(step);
            if ((steps.length == 1) && (currentStep === null))
            {
                // First step
                triggerTimer.start();
            }
        }

        function setCurrentUI(url, props) {
            var wrapperItemComp = Qt.createComponent("WorkPanelItem.qml");
            if (wrapperItemComp.status !== Component.Ready)
            {
                console.log(wrapperItemComp.errorString());
            }
            var wrapperItem = wrapperItemComp.createObject(workPanel);
            wrapperItem.anchors.fill = workPanel;

            var item = null;
            if (url != undefined)
            {
                var comp = Qt.createComponent(url);
                if (comp.status === Component.Ready)
                {
                    item = comp.createObject(wrapperItem);
                    if (props !== undefined)
                    {
                        for (var key in props)
                        {
                            item[key] = props[key];
                        }
                    }
                }
                else
                {
                    console.log(comp.errorString());
                }
            }

            if (currentItem != null) currentItem.fadingVisibility = false;
            if (item !== null)
            {
                item.anchors.left = wrapperItem.left;
                item.anchors.right = wrapperItem.right;
                item.anchors.verticalCenter = wrapperItem.verticalCenter;
                item.anchors.leftMargin = 6;
                item.anchors.rightMargin = 6;
                item.visible = true;
            }
            currentItem = wrapperItem;
            currentItem.fadingVisibility = true;
            return item;
        }

        function stepCompleted(success, errorMessage) {
            if (success)
            {
                if (steps.length > 0)
                {
                    nextStep();
                    return;
                }
            }
            else
            {
                // Cancel all work.
                steps = [];
                currentStep = null;
                // Display message, if any
                if (errorMessage !== undefined)
                {
                    addStep(Qt.createComponent("steps/StepShowMessage.qml"), {"message": errorMessage});
                    return;
                }
            }
            // We're done.
            currentStep = null;
            stackView.pop();
            setCurrentUI();
        }

        function nextStep() {
            var step = steps.shift();
            currentStep = step;
            step.stepCompleted.connect(stepCompleted);
            step.perform(workPanel);
        }
    }

}

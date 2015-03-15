import QtQuick 2.4

Item {
    id: workPanelItem
    property bool fadingVisibility: false

    opacity: fadingVisibility ? 1 : 0
    Behavior on opacity {
        PropertyAnimation {
            id: opacityAnim
            onRunningChanged: {
                if (!opacityAnim.running && !workPanelItem.fadingVisibility)
                {
                    workPanelItem.destroy();
                }
            }
        }
    }
}

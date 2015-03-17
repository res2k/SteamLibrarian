import QtQuick 2.4

Item {
    id: workPanelItem
    property bool fadingVisibility: false

    opacity: fadingVisibility ? 1 : 0
    Behavior on opacity {
        OpacityAnimator {
            target: workPanelItem
            id: opacityAnim
        }
    }

    property bool _wasVisible: false
    onOpacityChanged: {
        if (fadingVisibility)
        {
            _wasVisible = true;
        }
        else if (_wasVisible && (opacity < 0.001))
        {
            workPanelItem.destroy();
        }
    }
}

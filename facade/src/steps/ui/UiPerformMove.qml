import QtQuick 2.4

UiPerformMoveForm {
    Component.onCompleted: {
        this.height = Qt.binding(function() { return Math.max(columnLayout.implicitHeight, busy.height)})
    }
}


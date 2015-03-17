import QtQuick 2.4

Item {
    signal stepCompleted(bool succeeded)

    property string message

    function perform(steps) {
        var ui = steps.setCurrentUI("steps/ui/UiShowMessage.qml", {"message": message});
        ui.confirmed.connect(function() { stepCompleted(true); });
    }
}

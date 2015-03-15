import QtQuick 2.4

Item {
    id: waitStep
    signal stepCompleted(bool succeeded)

    Timer {
        id: waitTimer
        interval: 5000

        onTriggered: stepCompleted(true);
    }

    function perform(steps) {
        steps.setCurrentUI("steps/ui/UiWait.qml");
        waitTimer.start();
    }
}

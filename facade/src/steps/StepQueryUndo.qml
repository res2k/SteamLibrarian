import QtQuick 2.4
import SteamLibrarian 1.0

QtObject {
    id: queryUndoStep
    signal stepCompleted(bool succeeded)

    property string message
    property var undoMover

    function perform(steps) {
        var prompt = steps.setCurrentUI("steps/ui/UiQueryUndo.qml");
        prompt.moveError = message;
        prompt.undoConfirmed.connect(function() { undoConfirmed(steps); });
        prompt.cancelled.connect(function() { stepCompleted(false); });
    }

    function undoConfirmed(steps) {
        steps.addStep(Qt.createComponent("StepPerformUndo.qml"), {"undoMover": undoMover});
        stepCompleted(true);
    }
}

import QtQuick 2.4
import SteamLibrarian 1.0

Item {
    id: waitStep
    signal stepCompleted(bool succeeded, string errorMessage)

    property QtObject app
    property QtObject destinationLib

    property var mover

    function perform(steps) {
        steps.setCurrentUI("steps/ui/UiPerformMove.qml", {"isUndo": false});
        mover = app.queryMover(destinationLib);
        if (mover === null)
        {
            stepCompleted(false, undefined);
            return;
        }
        mover.finished.connect(function(result) { moverFinished(result, steps); });
        mover.perform();
    }

    function moverFinished(result, steps) {
        console.log(result);
        switch(result)
        {
        case AppMover.Success:
            stepCompleted(true, undefined);
            break;
        case AppMover.UserCancelled:
        case AppMover.Failure:
            {
                var message =
                    (result === AppMover.UserCancelled) ? qsTr("User cancelled.")
                                                        : mover.errorString;
                var undoMover = mover.getUndoMover();
                if (undoMover === null)
                {
                    stepCompleted(false, undefined);
                    return;
                }
                steps.addStep(Qt.createComponent("StepQueryUndo.qml"), {"message": message, "undoMover": undoMover});
                stepCompleted(true, undefined);
            }
            break;
        default:
            stepCompleted(false, qsTr("Unknown result " + result));
            break;
        }
    }
}

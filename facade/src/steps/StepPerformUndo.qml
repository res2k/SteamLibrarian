import QtQuick 2.4
import SteamLibrarian 1.0

Item {
    id: waitStep
    signal stepCompleted(bool succeeded, string errorMessage)

    property var undoMover

    function perform(steps) {
        steps.setCurrentUI("steps/ui/UiPerformMove.qml", {"isUndo": true});
        undoMover.finished.connect(moverFinished);
        undoMover.perform();
    }

    function moverFinished(result) {
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
                                                        : undoMover.errorString;
                stepCompleted(false, message);
            }
            break;
        default:
            stepCompleted(false, qsTr("Unknown result " + result));
            break;
        }
    }
}

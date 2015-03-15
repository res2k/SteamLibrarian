import QtQuick 2.4
import SteamLibrarian 1.0

QtObject {
    id: stopStep
    signal stepCompleted(bool succeeded)

    function perform(steps) {
        console.log(steps);
        // Check if Steam is running
        console.log(Piping.steamRunning);
        if (!Piping.steamRunning)
        {
            //  if not, proceed
            stepCompleted(true);
        }
        else
        {
            //  if running, show UI to query exit
            var exitPrompt = steps.setCurrentUI("steps/ui/UiSteamStopQuery.qml");
            console.log(exitPrompt);
            //   if exit is confirmed, trigger Steam stop + wait
            exitPrompt.exitConfirmed.connect(function() { exitConfirmed(steps); });
            exitPrompt.cancelled.connect(function() { stepCompleted(false); });
        }
    }

    function exitConfirmed(steps) {
        var waitUI = steps.setCurrentUI("steps/ui/UiWaitForSteamStop.qml")
        waitUI.stopped.connect(function() { stepCompleted(true); });
        waitUI.cancelled.connect(function() { stepCompleted(false); });
    }
}

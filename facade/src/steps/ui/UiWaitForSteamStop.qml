import QtQuick 2.4
import SteamLibrarian 1.0

UiWaitForSteamStopForm {
    signal stopped()
    signal cancelled()

    Component.onCompleted: {
        cancelButton.clicked.connect(cancelled);
        Piping.steamRunningChanged.connect(steamRunningChanged);
        Piping.steamShutdown();
    }

    function steamRunningChanged(running) {
        if (!running) stopped();
    }
}


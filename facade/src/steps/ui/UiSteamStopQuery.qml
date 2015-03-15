import QtQuick 2.4

UiSteamStopQueryForm {
    signal exitConfirmed()
    signal cancelled()

    Component.onCompleted: {
        exitButton.clicked.connect(exitConfirmed);
        cancelButton.clicked.connect(cancelled);
    }
}


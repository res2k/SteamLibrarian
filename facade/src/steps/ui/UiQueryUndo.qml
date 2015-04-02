import QtQuick 2.4

UiQueryUndoForm {
    signal undoConfirmed()
    signal cancelled()

    Component.onCompleted: {
        undoButton.clicked.connect(undoConfirmed);
        cancelButton.clicked.connect(cancelled);
    }
}


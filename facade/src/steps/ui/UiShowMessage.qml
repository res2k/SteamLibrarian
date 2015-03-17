import QtQuick 2.4

UiShowMessageForm {
    signal confirmed()

    Component.onCompleted: {
        okButton.clicked.connect(confirmed);
    }
}


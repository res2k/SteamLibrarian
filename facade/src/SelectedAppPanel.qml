import QtQuick 2.4

SelectedAppPanelForm {
    signal moveClicked()

    Behavior on height {
        NumberAnimation {
            easing { type: Easing.InOutQuad }
        }
    }

    Component.onCompleted: {
        // Break binding loop
        gameLabel.largeFontSize += 0;

        moveButton.onClicked.connect(moveClicked);
    }

    onAppChanged: {
        // Select app's current library in combo
        if (app != null)
        {
            libCombo.selectLibrary(app.library);
            appLibIndex = libCombo.currentIndex;
        }
    }

    function selectedLib()
    {
        return libCombo.selectedLib();
    }
}


import QtQuick 2.4
import SteamLibrarian 1.0

SelectedAppPanelForm {
    signal moveClicked()

    Behavior on height {
        NumberAnimation {
            easing { type: Easing.InOutQuad }
        }
    }

    sizeLabel.text: {
        var s = "";
        if (app !== null)
        {
            s = UI.FormatByteAmount(app.sizeOnDisk, UI.roundUp);
            var dlsize = app.downloadingSize;
            if (dlsize > 0)
            {
                s = qsTr("%1 (+ about %2 downloading)").arg(s).arg(UI.FormatByteAmount(dlsize, UI.roundUp));
            }
        }
        return s;
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


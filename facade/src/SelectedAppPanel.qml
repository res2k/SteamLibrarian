/*
  SteamLibrarian
  Copyright (c) 2015 Frank Richter

  This file is part of SteamLibrarian.

  SteamLibrarian is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SteamLibrarian is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SteamLibrarian.  If not, see <http://www.gnu.org/licenses/>.
*/

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


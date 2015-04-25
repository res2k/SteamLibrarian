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
                    stepCompleted(false, message);
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

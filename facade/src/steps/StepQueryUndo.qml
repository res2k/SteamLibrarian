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

QtObject {
    id: queryUndoStep
    signal stepCompleted(bool succeeded)

    property string message
    property var undoMover

    function perform(steps) {
        var prompt = steps.setCurrentUI("steps/ui/UiQueryUndo.qml");
        prompt.moveError = message;
        prompt.undoConfirmed.connect(function() { undoConfirmed(steps); });
        prompt.cancelled.connect(function() { stepCompleted(false); });
    }

    function undoConfirmed(steps) {
        steps.addStep(Qt.createComponent("StepPerformUndo.qml"), {"undoMover": undoMover});
        stepCompleted(true);
    }
}

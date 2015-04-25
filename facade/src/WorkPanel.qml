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

Item {
    id: workPanel

    Timer {
        id: triggerTimer
        interval: 0
    }

    Component.onCompleted: {
        triggerTimer.triggered.connect(nextStep);
    }

    property var steps: []
    property var currentStep: null
    property var currentItem: null

    function addStep(comp, props) {
        if (comp.status !== Component.Ready)
        {
            console.log(comp.errorString());
            return;
        }
        var step = comp.createObject(workPanel);
        if (props !== undefined)
        {
            for (var key in props)
            {
                step[key] = props[key];
            }
        }
        steps.push(step);
        if ((steps.length == 1) && (currentStep === null))
        {
            // First step
            triggerTimer.start();
        }
    }

    function setCurrentUI(url, props) {
        var wrapperItemComp = Qt.createComponent("WorkPanelItem.qml");
        if (wrapperItemComp.status !== Component.Ready)
        {
            console.log(wrapperItemComp.errorString());
        }
        var wrapperItem = wrapperItemComp.createObject(workPanel);
        wrapperItem.anchors.fill = workPanel;

        var item = null;
        if (url != undefined)
        {
            var comp = Qt.createComponent(url);
            if (comp.status === Component.Ready)
            {
                item = comp.createObject(wrapperItem);
                if (props !== undefined)
                {
                    for (var key in props)
                    {
                        item[key] = props[key];
                    }
                }
            }
            else
            {
                console.log(comp.errorString());
            }
        }

        if (currentItem != null) currentItem.fadingVisibility = false;
        if (item !== null)
        {
            item.anchors.left = wrapperItem.left;
            item.anchors.right = wrapperItem.right;
            item.anchors.verticalCenter = wrapperItem.verticalCenter;
            item.anchors.leftMargin = 6;
            item.anchors.rightMargin = 6;
            item.visible = true;
        }
        currentItem = wrapperItem;
        currentItem.fadingVisibility = true;
        return item;
    }

    function stepCompleted(success, errorMessage) {
        if (success)
        {
            if (steps.length > 0)
            {
                nextStep();
                return;
            }
        }
        else
        {
            // Cancel all work.
            steps = [];
            currentStep = null;
            // Display message, if any
            if (errorMessage !== undefined)
            {
                addStep(Qt.createComponent("steps/StepShowMessage.qml"), {"message": errorMessage});
                return;
            }
        }
        // We're done.
        currentStep = null;
        stackView.pop();
        setCurrentUI();
    }

    function nextStep() {
        var step = steps.shift();
        currentStep = step;
        step.stepCompleted.connect(stepCompleted);
        step.perform(workPanel);
    }
}

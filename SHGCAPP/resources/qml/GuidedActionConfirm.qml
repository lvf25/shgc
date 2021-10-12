import QtQuick 2.0
import QtQuick.Controls         1.2
import QtQuick.Dialogs          1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts          1.2

import QGroundControl           1.0



Rectangle {
    id: _root
    color: "#FFFFFF"
    visible:        false

    property alias  slider: slider
    property int    slider_width: _root.width*(2/3)
    property int     slider_height: _root.height/5
    property var    guidedController
    property int    action
    property var    actionData

    property alias  title:              titleText.text
    property alias  message:            messageText.text
    property bool   hideTrigger:        false
    property var    mapIndicator

//    onHideTriggerChanged: {
//        if (hideTrigger) {
//            confirmCancelled()
//        }
//    }


    function confirmCancelled() {
//        altitudeSlider.visible = false
        visible = false
        hideTrigger = false
        visibleTimer.stop()
        if (mapIndicator) {
            mapIndicator.actionCancelled()
            mapIndicator = undefined
        }
    }

    function show(immediate) {
        slider.value = 0
        visible = true
        visibleTimer.start()

//        if (immediate) {
//            visible = true
//        } else {
//            // We delay showing the confirmation for a small amount in order to any other state
//            // changes to propogate through the system. This way only the final state shows up.
//            visibleTimer.restart()
//        }
    }

    Timer {
        id:             visibleTimer
        interval:       5000
        repeat:         false
        onTriggered:    visible = false
    }



    Column {
        id:confirmColumn
        anchors.margins: 2
        anchors.horizontalCenter:   _root.horizontalCenter
        anchors.top: _root.top
        anchors.topMargin: 20
        anchors.bottom: _root.bottom
        anchors.bottomMargin: 20
        spacing: 20
        Label{
            id:titleText
            anchors.left:       slider.left
            anchors.right:      slider.right
            horizontalAlignment:    Text.AlignHCenter
            font.pointSize:  20
        }

        Label{
            id:                     messageText
            anchors.left:           slider.left
            anchors.right:          slider.right
            horizontalAlignment:    Text.AlignHCenter
            wrapMode:               Text.WordWrap
        }

        Slider{
            id:  slider
            property string confirmText                         ///< Text for slider
            property bool _up :true
            implicitHeight:  slider_height
            implicitWidth: slider_width
            anchors.horizontalCenter:   parent.horizontalCenter
            confirmText:    "Slide to confirm"
            stepSize: 0.01
            value: 0
            visible: true
            updateValueWhileDragging: false
            wheelEnabled:false
            onValueChanged:
            {
                if(value < maximumValue)
                {
                    value = 0
                }

                if(value == maximumValue)
                {
                    var altitudeChange = 4.0
                    guidedController.executeAction(_root.action, _root.actionData, altitudeChange)

                }
            }
            style: SliderStyle
            {             
                id:_sliderstyle
                groove:SliderRectangle
                {
                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: slider_width
                    implicitHeight:  slider_height
                    slider_value:slider.value
                    showMsg:slider.confirmText
                    border.color: "#CFD2D4"
                    border.width: 1
                    radius: slider.height /2
                }

                handle: Rectangle
                {
                    id:_handle
                    anchors.horizontalCenter:parent.horizontalCenter
                    anchors.bottom:parent.bottom
                    height:     slider.height
                    width:      slider.height
                    antialiasing:true
                    radius:     (slider.height) / 2
                    color:      "#FFFFFF"
                }
            }

        }
    }


}

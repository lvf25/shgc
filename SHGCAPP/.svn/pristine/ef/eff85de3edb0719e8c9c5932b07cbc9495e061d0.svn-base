import QtQuick 2.0
import QtQuick.Controls         1.2

Rectangle
   {
       property var slider_value
       property string showMsg
       id:sliderR
       color: "#E6E9EB"

       Label {
           id: slideLabel
           anchors.centerIn:   parent
           text: qsTr(showMsg)
           font.pointSize: 10
           color: "#323232"
       }

       Rectangle
       {
           id:line_child
           radius: parent.height /2
           height: parent.height
           anchors.top:parent.top
           anchors.left: parent.left
           width: parent.width*slider_value
           color: "#03A8F7"
       }
   }

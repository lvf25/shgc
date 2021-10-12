import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import QGroundControl.Controllers   1.0
import QGroundControl               1.0


Item
{
    property var tablewidth
    property var model: model

    TableView
    {
        id: tableView
        frameVisible: false
        anchors.fill: parent
        backgroundVisible: false
//        selectionMode: SelectionMode.NoSelection

        alternatingRowColors: false

        TableViewColumn {
            id: filename
            role: "filename"
            width: parent.width*0.6

        }
        TableViewColumn {
            id: opentime
            role: "opentime"
            width: parent.width*0.4

        }
        model: model



        headerDelegate:Rectangle{
            visible: false
        }

        rowDelegate:Rectangle{
            height: tablewidth
            property color rowColor: styleData.selected? Qt.rgba(255/255,255/255,255/255,0.5):Qt.rgba(255/255,255/255,255/255,0)
            color: rowColor
        }

        itemDelegate: Rectangle{
            anchors.fill: parent
            color: Qt.rgba(22/255,22/255,22/255,0)
            Text {
                verticalAlignment: Text.AlignVCenter
                color: Qt.rgba(255/255,255/255,255/255,1)
                text: styleData.value
                font.pixelSize: parent.height*0.3
            }
        }

        onDoubleClicked: {
            mavlinkplayer.logfileforplaybackByid(row)
        }

    }

    LogSqlModel{
        id:model
    }
}

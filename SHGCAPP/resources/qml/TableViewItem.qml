import QtQuick 2.0
import QtQuick.Controls 1.4

Item {

    property var missionController

    property var    _visualItems:                       _missionController.visualItems

    ListModel {
        id: tablemode

        ListElement {
            title: "A"
            author: "Gabriel"
        }
        ListElement {
            title: "Brilliance"
            author: "Jens"
        }
        ListElement {
            title: "Outstanding"
            author: "Frederik"
        }
    }

    TableView{
        id :tableView
        alternatingRowColors : false
        TableViewColumn {
                  role: "title"
                  title: "Title"
                  width: tableView.viewport.width/tableView.columnCount
              }
        TableViewColumn {
              role: "author"
              title: "Author"
              width: tableView.viewport.width/tableView.columnCount
          }
        model: tablemode
    }
}

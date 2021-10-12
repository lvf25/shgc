import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs  1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.Controllers   1.0

Item {
    id:         _fileroot

    property var    qgcView
    property string folder              // Due to Qt bug with file url parsing this must be an absolute path
    property var    nameFilters
    property string fileExtension       // Primary file extension to search for
    property string fileExtension2: ""  // Secondary file extension to search for
    property string title
    property bool   selectExisting
    property bool   selectFolder

    property bool   _openForLoad:   true
    property real   _margins:       ScreenTools.defaultFontPixelHeight / 2
    property var    _rgExtensions


    signal acceptedForLoad(string file)
    signal acceptedForSave(string file)
    signal rejected

    Component.onCompleted: setupFileExtensions()

    onFileExtensionChanged: setupFileExtensions()
    onFileExtension2Changed: setupFileExtensions()

    function setupFileExtensions() {
        if (fileExtension2 == "") {
            _rgExtensions = [ fileExtension ]
        } else {
            _rgExtensions = [ fileExtension, fileExtension2 ]
        }
    }

    function openForLoad() {
        _openForLoad = true
//        if (_mobileDlg && folder.length !== 0) {
//            qgcView.showDialog(mobileFileOpenDialog, title, qgcView.showDialogDefaultWidth, StandardButton.Cancel)
//        } else {
            fullFileDialog.open()
//        }
    }

    function openForSave() {
        _openForLoad = false
//        if (_mobileDlg && folder.length !== 0) {
////            qgcView.showDialog(mobileFileSaveDialog, title, qgcView.showDialogDefaultWidth, StandardButton.Cancel | StandardButton.Ok)
//        } else {
            fullFileDialog.open()
//        }
    }

    function close() {
        console.log("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
        fullFileDialog.close()
    }

    QGCFileDialogController { id: controller }

    FileDialog {
        id:             fullFileDialog
        folder:         "file:///" + _fileroot.folder
        nameFilters:    _fileroot.nameFilters ? _fileroot.nameFilters : []
        title:          _fileroot.title
        selectExisting: _fileroot.selectExisting
        selectMultiple: false
        selectFolder:   _fileroot.selectFolder

        onAccepted: {
            if (_openForLoad) {
                _fileroot.acceptedForLoad(controller.urlToLocalFile(fileUrl))
            } else {
                _fileroot.acceptedForSave(controller.urlToLocalFile(fileUrl))
                console.log(controller.urlToLocalFile(fileUrl))
            }
        }
        onRejected: {
            _fileroot.rejected()
        }

//        Component.onCompleted: visible = true
    }
}

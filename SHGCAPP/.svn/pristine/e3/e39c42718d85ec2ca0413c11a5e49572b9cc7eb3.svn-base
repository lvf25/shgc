import QtQuick 2.0
import QtQml 2.0
import QtQuick.Controls         2.5
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

Rectangle {
    id:straightline
    color: "#10000000"
    visible: false

    property var lineEditWidth
    property var lineEditHeight
    property var linefont

    Rectangle{
        id:editform
        color: "#161616"
        opacity: 0.8
        implicitWidth: lineEditWidth*0.9
        implicitHeight: lineEditHeight*0.9
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        z: 0

        ColumnLayout{
            spacing:linefont
            anchors.left: parent.left
            anchors.leftMargin: linefont*2.5
            anchors.top: parent.top
            anchors.topMargin: linefont*2

            Column{
                spacing:linefont
                Label{
                    id:direction
                    width: linefont*2.5
                    height: linefont
                    Text{
                        anchors.fill: parent
                        horizontalAlignment:    Text.AlignHCenter
                        verticalAlignment:    Text.AlignVCenter
                        font.bold: true
                        font.pixelSize: linefont*1.5
                        text: qsTr("<font color=\"#FFFFFF\">方向</font>")
                    }
                }

                Row{
                    spacing:linefont
                    ScrollView{
                        id: directioninput
                        width: lineEditWidth/3
                        height: lineEditHeight/5
                        background: Rectangle{
                            color: "#C2C2C2"
                            border.color: "#C2C2C2"
                            border.width: directioninput.activeFocus? 2: 1
                        }
                        clip: true
                        ScrollBar.horizontal: ScrollBar{ visible: false }

                        TextArea{
                            id:directionedit
                            color:"black"
                            verticalAlignment:    Text.AlignVCenter
                            horizontalAlignment:    Text.AlignHCenter
                            placeholderText: "Input Direction"
                            placeholderTextColor: "gray"
                            textFormat: TextEdit.RichText
                            selectByKeyboard: true
                            selectByMouse: true
                            //选中文本的颜色
                            selectedTextColor: "white"
                            //选中文本背景色
                            selectionColor: "blue"
                            renderType: Text.NativeRendering
                            //文本换行，默认NoWrap
                            wrapMode: TextEdit.Wrap
                            font.bold: true
                            font.pixelSize: linefont*2


                        }
                    }

                    Button{
                        id:directionnote
                        width: linefont*4
                        height: linefont*4

                        background: Rectangle{
                            id:_directionnote
                            color:"transparent"
                            Image {
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectFit
                                source: "qrc:/qml/resources/qml/description.png"
                                }
                        }
                        onClicked:
                        {

                        }

                    }

                }
            }




            RowLayout{
                spacing: linefont*4
                Column{
                    spacing: linefont
                    Label{
                        id:length
                        width: linefont*2.5
                        height: linefont
                        Text{
                            anchors.fill: parent
                            horizontalAlignment:    Text.AlignHCenter
                            verticalAlignment:    Text.AlignVCenter
                            font.bold: true
                            font.pixelSize: linefont*1.5
                            text: qsTr("<font color=\"#FFFFFF\">长度</font>")
                        }
                    }

                    Row{
                        spacing:linefont*0.2
//                        ScrollView{
//                            id: lengthinput
//                            width: lineEditWidth/3
//                            height: lineEditHeight/5
//                            background: Rectangle{
//                                color: "#C2C2C2"
//                                border.color: "#C2C2C2"
//                                border.width: lengthinput.activeFocus? 2: 1
//                            }
//                            clip: true
//                            ScrollBar.horizontal: ScrollBar{ visible: false }

//                            TextArea{
//                                id:lengthedit
//                                color:"black"
//                                verticalAlignment:    Text.AlignVCenter
//                                horizontalAlignment:    Text.AlignHCenter
//                                placeholderText: "Input Length"
//                                placeholderTextColor: "gray"
//                                textFormat: TextEdit.RichText
//                                selectByKeyboard: true
//                                selectByMouse: true
//                                selectedTextColor: "white"
//                                selectionColor: "blue"
//                                renderType: Text.NativeRendering
//                                wrapMode: TextEdit.Wrap
//                                font.bold: true
//                                font.pixelSize: linefont*2


//                            }
//                        }
                        ScrollView{
                            width: lineEditWidth/3
                            height: lineEditHeight/5
                            SpinBox{
                                from: 0
                                to:500
                                value: 20
                                stepSize: 1
                                editable: true
                                implicitWidth: lineEditWidth/3
                                implicitHeight: lineEditHeight/5
                                font.pixelSize: linefont*2
                            }
                        }




                        Label{
                            id:lengthunit
                            width: linefont*2
                            height: lineEditHeight/5

                            Text{
                                anchors.fill: parent
                                horizontalAlignment:    Text.AlignHCenter
                                verticalAlignment:    Text.AlignVCenter
                                font.bold: true
                                font.pixelSize: linefont*2
                                text: qsTr("<font color=\"#FFFFFF\">m</font>")
                            }


                        }

                    }
                }

                Column{
                    spacing: linefont
                    Label{
                        id:number
                        width: linefont*4
                        height: linefont
                        Text{
                            anchors.fill: parent
                            horizontalAlignment:    Text.AlignHCenter
                            verticalAlignment:    Text.AlignVCenter
                            font.bold: true
                            font.pixelSize: linefont*1.5
                            text: qsTr("<font color=\"#FFFFFF\">航点数</font>")
                        }
                    }

                    ScrollView{
                        width: lineEditWidth/3
                        height: lineEditHeight/5
                        SpinBox{
                            from: 0
                            to:50
                            value: 2
                            stepSize: 1
                            editable: true
                            implicitWidth: lineEditWidth/3
                            implicitHeight: lineEditHeight/5
                            font.pixelSize: linefont*2
                        }
                    }

//                    ScrollView{
//                        id: numberinput
//                        width: lineEditWidth/3
//                        height: lineEditHeight/5
//                        background: Rectangle{
//                            color: "#C2C2C2"
//                            border.color: "#C2C2C2"
//                            border.width: numberinput.activeFocus? 2: 1
//                        }
//                        clip: true
//                        ScrollBar.horizontal: ScrollBar{ visible: false }

//                        TextArea{
//                            id:numberedit
//                            color:"black"
//                            verticalAlignment:    Text.AlignVCenter
//                            horizontalAlignment:    Text.AlignHCenter
//                            placeholderText: "Input Number"
//                            placeholderTextColor: "gray"
//                            textFormat: TextEdit.RichText
//                            selectByKeyboard: true
//                            selectByMouse: true
//                            selectedTextColor: "white"
//                            selectionColor: "blue"
//                            renderType: Text.NativeRendering
//                            wrapMode: TextEdit.Wrap
//                            font.bold: true
//                            font.pixelSize: linefont*2

//                        }
//                    }

                }


            }

        }

    }

    Button{
        id:closeedit
        implicitWidth: lineEditWidth*0.2
        implicitHeight: lineEditHeight*0.2
        anchors.right: editform.right
        anchors.rightMargin: -linefont*5
        anchors.top: editform.top
        anchors.topMargin: -linefont*2
        z: editform.z+1

        background: Rectangle{
            id:_closeedit
            color:"transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/qml/close.png"
                }
        }

        onClicked:
        {
            straightline.visible = false
        }

    }



}

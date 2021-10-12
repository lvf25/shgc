import QtQuick 2.7
import QtQml 2.0
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0
import QtWebEngine 1.4
import QtMultimedia 5.4
import QtWebChannel 1.0
import QtPositioning 5.11

import QGroundControl 1.0
import QGroundControl.ScreenToolsController 1.0
import QGroundControl.Controllers 1.0

Item {

    id: root
    signal logReplay()
    signal showFlightMonitRectange()
    signal showRoutePlanRectange()
    signal showActionAdjust()

    signal showUpload()
    signal showLeftMainWindow()
    signal uploadToUAV()
    signal showMessage(string str)
    signal showFV_WLG()
    signal closeFV_WLG()
    signal connectUAVSerialLink(bool str)
    signal setAircraftCenter_fromVelocimeter(bool str)

    signal point_Edit_WidgetStatue(string seq_no)
    signal addPointItem(double lat,double lng,int row)
    signal removePointItem(int row)
    signal upPointItem(int row)
    signal downPointItem(int row)

    property int button_width:              root.height/13
    property int shihuanfontsize:           button_width/5
    property int  flightData_width:         root.width*(1/5)
    property alias  _guidedController:      guidedActionsController
    property alias flightdatashowrectange:  flightdatashowrectange
    property alias latlondata:              latlondata
    property alias show_flightData:         show_flightData

    property alias maptohtmlrectange:       maptohtmlrectange
    property alias positionshow:            positionshow
    property alias someObject:              someObject

    property alias routplanactionrectange:  routplanactionrectange
    property alias routeplantoprectange:    routeplantoprectange
    property alias playview:                playview
    property alias logplayer:               logplayer
    property alias chooselogfile:           chooselogfile
    property alias _deleteflycontrolwaypointtip:    deleteflycontrolwaypointtip
    property alias powerdisplay:             powerdisplay
    property alias panelloadComponent:       panelloadComponent


    property alias showmessage: showmessage
    function qmlshowFlightMonitRectange()
    {

        latlondata.visible = true
        show_flightData.visible = true
        flightdatashowrectange.visible = true
        flightControl.visible = true

        routplanactionrectange.visible = false
        routeplantoprectange.visible = false
        upload.visible = false

        adjustrectange.visible = false

    }

    function  qmlshowRoutePlanRectange()
    {
        latlondata.visible = false
        show_flightData.visible = false
        flightdatashowrectange.visible = false
        flightControl.visible = false

        routplanactionrectange.visible = true
        routeplantoprectange.visible = true
        upload.visible = true

        adjustrectange.visible = false
    }

    function qmlshowActionAdjust()
    {
        latlondata.visible = false
        show_flightData.visible = false
        flightdatashowrectange.visible = false
        flightControl.visible = false

        routplanactionrectange.visible = false
        routeplantoprectange.visible = false
        upload.visible = false

        panelload.setSource(panelloadComponent)


    }



    function load(str)
    {
        //        console.log(str)
        switch (str)
        {
        case "百度地图": webEngine.url = "qrc:/pages/baidumap.html";
            break
        case "高德地图": webEngine.url = "qrc:/pages/gaodemap.html";
            break
        }
    }

    function uploadToUAVfun()
    {
        playview._planMasterController.upload();
    }
    
    function showMessagefun(str)
    {
        showmessage.messge = str;
        showmessage.visible = true;
    }


    QtObject {
        id: someObject
        // ID，在这个ID下，这个对象在WebEngineView端是已知的
        WebChannel.id: "map"
        signal latSignal(var lat)
        signal lonSignal(var lon)
        signal headingRecvDataToHtml(var headingData)

        signal openLogAnctionToHtml(bool str)

        signal showNormalMap()
        signal showHybridMap()

        signal showPointData()
        signal hidePointData()
        signal showLineData()
        signal hideLineData()

        //地图回中
        signal setAircraftCenter(bool str)
        signal setQGCCenter(bool str)
        signal setStartPoiCenter()
        signal setHomeCenter()

        //画航点
        signal drawRoutePlan()
        signal removePointFun()

        signal drawLine(var num ,var lng,var lat)
        signal drawtakeoff(var lng,var lat)
        signal drawHomePosition(var lng,var lat)

        signal reverseOrder()
        signal rulerMeasuring()
        signal cancleMeasuring()

        signal clearLoglines()

        property int visualItemscount: playview.getvisualItemscount

        readonly property int   _decimalPlaces:             8


        function sendData()
        {
            root.closeallretange()
        }

        function point_Edit_Statue(seq_no)
        {
            root.point_Edit_WidgetStatue(seq_no)
        }

        //创建航点链表
        function sendLonLatData(lng,lat)
        {
            console.log(lat)
            var coordinate = QtPositioning.coordinate(lat, lng, 0)
            coordinate.latitude = coordinate.latitude.toFixed(_decimalPlaces)
            coordinate.longitude = coordinate.longitude.toFixed(_decimalPlaces)
            coordinate.altitude = coordinate.altitude.toFixed(_decimalPlaces)

            playview._insertSimpleMissionItem(coordinate)

        }

    }


    Rectangle{
        id :view1
        parent: root
        visible: true
        width: root.width
        height: root.height
        z:0

        WebEngineView {
            id: webEngine
            z: 1
            anchors.fill: parent
            url: "qrc:/pages/baidumap.html"
            settings.javascriptCanAccessClipboard: true
            settings.javascriptCanOpenWindows : true
            settings.javascriptEnabled: true
            settings.pluginsEnabled: true
            settings.autoLoadImages: true
            onNewViewRequested: request.openIn(webEngine)
            settings.focusOnNavigationEnabled: true

            webChannel: channel
        }
    }

    WebChannel {
        id: channel
        registeredObjects: [someObject]
    }

    TopRectange{
        id:toprectange
        implicitHeight: root.height/15
        implicitWidth: root.width
        topwidth: root.width
        topheight: root.height/15
        topfontsize: shihuanfontsize
        anchors.top: root.top
        anchors.left: root.left
        visible: true
//        powershow: powerdisplay
    }

    PowerDisplay{
        id:powerdisplay
        implicitHeight: toprectange.height*0.3
        anchors.top: toprectange.bottom
        anchors.topMargin: 0.15
        anchors.left: root.left
        anchors.right: root.right
        powerheight: toprectange.height/3
        powerwidth: root.width
        powerfont: shihuanfontsize
    }

    FlightPosition{
        id:latlondata
        z:flightControl.z
        visible: true
        anchors.top: powerdisplay.bottom
        anchors.topMargin: toprectange.height/3
        anchors.horizontalCenter: parent.horizontalCenter
        implicitHeight: button_width*0.6
        implicitWidth: flightData_width
        flightpositionheight:button_width
    }

    Rectangle{
        id:show_flightData
        z:webEngine.z +3
        visible: true
        implicitWidth : flightData_width*5
        implicitHeight: button_width*2
        anchors.left: root.left
        anchors.bottom: root.bottom
        opacity: 0.5
        gradient: Gradient{
            GradientStop{ position: 0.0;color: "#FFFFFF"}
            GradientStop{ position: 1.0;color: "#323232"}
        }
        border { width:0}
    }


    SHGClogplayer
    {
        id:logplayer
        visible: false
        anchors.horizontalCenter:   root.horizontalCenter
        anchors.bottom: flightdatashowrectange.top
        anchors.bottomMargin: button_width*1.3
        fontSize:shihuanfontsize
        labersize: button_width
    }

    SHGCChooseFile{
        id:chooselogfile
        z:flightdatashowrectange.z+1
        visible: false
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        implicitWidth: flightData_width
        fontSize:shihuanfontsize
        labersize: button_width
        mavlinkplayer: logplayer.mavlinkplayer
    }

    FlightDataShowRectange{
        id: flightdatashowrectange
        z: show_flightData.z+3
        visible: true
        implicitWidth : flightData_width*3
        anchors.horizontalCenter:   root.horizontalCenter
        anchors.bottom: root.bottom
        anchors.bottomMargin: button_width*1.3
        fontSize:shihuanfontsize
    }

    MapToHtmlRectange{
        id:maptohtmlrectange
        visible: true
        anchors.top: powerdisplay.bottom
        anchors.topMargin: toprectange.height/3
//        anchors.left: latlon.right
//        anchors.leftMargin: button_width
        anchors.right: parent.right
        anchors.rightMargin: button_width
        buttonSize: button_width
    }

    PositionShow{
        id:positionshow
        implicitHeight: button_width
        implicitWidth: button_width*3.6
        anchors.top: maptohtmlrectange.bottom
        anchors.topMargin: button_width*1.1
        anchors.right: maptohtmlrectange.right
        anchors.rightMargin:button_width*2.8
        size:button_width
    }

    SwitchRectange{
        id:switchrectange
        implicitHeight: button_width
        implicitWidth: button_width*4.5
        anchors.top: maptohtmlrectange.bottom
        anchors.topMargin: button_width*1.1
        anchors.right: maptohtmlrectange.right
        anchors.rightMargin:button_width*1.5
        switchsize:button_width
    }

    MapChoose{
        id:mapchoose
        implicitHeight: button_width*2
        implicitWidth: button_width*3.6
        mapsize: button_width
        anchors.top: maptohtmlrectange.bottom
        anchors.topMargin: button_width*1.1
        anchors.right: maptohtmlrectange.right
//        anchors.rightMargin:button_width*0.5
    }

    function closeallretange()
    {
        positionshow.visible = false
        switchrectange.visible = false
        mapchoose.visible = false
        flycontroloperating.visible = false
        chooselogfile.visible = false
    }



    Rectangle{
        id: flightControl
        z: flightdatashowrectange.z+ 3

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: button_width*6.5
        visible: true

        ColumnLayout {

            spacing: button_width/3

            Button{
                id: startmission
                text:""
                implicitWidth: button_width
                implicitHeight: button_width
                enabled: guidedActionsController.showStartMission
                style: ButtonStyle{
                    background: BorderImage{
                        source:enabled ? "qrc:/qml/resources/qml/checkstartmission.png":"qrc:/qml/resources/qml/uncheckstartmission.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    guidedActionsController.closeAll()
                    guidedActionsController.confirmAction(_guidedController.actionStartMission)
                }

            }

            Button{
                id: takeoff
                text:""
                implicitWidth: button_width
                implicitHeight: button_width
                enabled: guidedActionsController.showTakeoff
                style: ButtonStyle{
                    background: BorderImage{
                        source:enabled ? "qrc:/qml/resources/qml/takeoff_is.png":"qrc:/qml/resources/qml/takeoff.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    guidedActionsController.closeAll()
                    guidedActionsController.confirmAction(_guidedController.actionTakeoff)
                }

            }

            Button{
                id: pause
                text:""
                implicitWidth: button_width
                implicitHeight: button_width
                enabled: guidedActionsController.showPause
                visible: true
                style: ButtonStyle{
                    background: BorderImage{
                        source:enabled ? "qrc:/qml/resources/qml/hover_is.png":"qrc:/qml/resources/qml/hover2.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    if(guidedActionsController.showPause)
                    {
                        _guidedController.confirmAction( _guidedController.actionPause)
                        pause.visible = false
                        flycontine.visible = true
                    }
                }
            }

            Button{
                id: flycontine
                text:""
                implicitWidth: button_width
                implicitHeight: button_width
                enabled: true
                visible: false
                style: ButtonStyle{
                    background: BorderImage{
                        source:visible ? "qrc:/qml/resources/qml/takeoffContinue.png":"qrc:/qml/resources/qml/hover2.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {

                    flycontine.visible = false
                    pause.visible = true
                }
            }

            Button{
                id: land
                text:""
                implicitWidth: button_width
                implicitHeight: button_width
                enabled: guidedActionsController.showLand
                style: ButtonStyle{
                    background: BorderImage{
                        source:enabled ? "qrc:/qml/resources/qml/landing.png":"qrc:/qml/resources/qml/unland.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    _guidedController.confirmAction( _guidedController.actionLand)

                }
            }

            Button{
                id: rtl
                text:""
                implicitWidth: button_width
                implicitHeight: button_width
                enabled: guidedActionsController.showRTL
                style: ButtonStyle{
                    background: BorderImage{
                        source:enabled ? "qrc:/qml/resources/qml/turnback_is.png":"qrc:/qml/resources/qml/turnback.png"
                        border.left: 0;border.right: 0;border.top: 0;border.bottom: 0
                    }
                }

                onClicked:
                {
                    _guidedController.confirmAction( _guidedController.actionRTL)

                }
            }
        }
    }


    GuidedActionConfirm {
        id:                         guidedActionConfirm
        z:flightdatashowrectange.z+3
        width: button_width*4
        height: button_width*3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/10
        anchors.horizontalCenter:   parent.horizontalCenter
        guidedController:           _guidedController
//        altitudeSlider:             _altitudeSlider

    }


    GuidedActionsController {
        id:                 guidedActionsController
        confirmDialog:      guidedActionConfirm
        missionController:  playview._missionController

        /// Close all dialogs
        function closeAll() {
//            mainWindow.enableToolbar()
//            rootLoader.sourceComponent  = null
            guidedActionConfirm.visible = false
//            guidedActionList.visible    = false
//            altitudeSlider.visible      = false
        }

    }


    //航线规划界面
    RoutePlanActionRectange{
        id:routplanactionrectange
        visible: false
        implicitWidth: button_width
        implicitHeight: button_width*7.5+8
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        widthsize:button_width


    }


    PlayView{
        id:playview
    }


    RoutePlanTopRectange{
        id:routeplantoprectange
        visible: false
        buttonSize: button_width
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: powerdisplay.bottom
        anchors.topMargin: toprectange.height/3
        playController:playview._planMasterController
    }

    FlyControlOperating{
        id:flycontroloperating
        visible: false
        implicitHeight: button_width*3.3
        implicitWidth: button_width*2.4
        fcsize:button_width
        anchors.top: routeplantoprectange.bottom
        anchors.topMargin: button_width*1.1
        anchors.left: routeplantoprectange.left
        anchors.leftMargin:button_width*1.2
        masterController:playview._planMasterController

    }

    DeleteConfirm{
        id:deletdConfirm
        waypointtip: _deleteflycontrolwaypointtip
        deleteTipWidth:button_width*6.6
        deleteTipHeight:button_width*4
        implicitWidth:button_width*6.6
        implicitHeight: button_width*4
        anchors.top:routeplantoprectange.bottom
        anchors.topMargin: button_width*1.5
        anchors.left: routeplantoprectange.left
        anchors.leftMargin: button_width *4
        fontsize:shihuanfontsize

    }

    DeleteFlyControlWaypointTip{
        id:deleteflycontrolwaypointtip
        tipWidth:button_width*5.5
        tipHeight:button_width
        implicitWidth:button_width*5.5
        implicitHeight:button_width
        anchors.top: routeplantoprectange.bottom
        anchors.topMargin: button_width*1.5
        anchors.left: routeplantoprectange.left
        anchors.leftMargin: button_width*11
        deletefontsize:shihuanfontsize

    }

    //lineEditWidth  lineEditHeight  linefont
    StraightLineEdit{
        id:straightlineedit
        lineEditWidth: button_width*10
        lineEditHeight: button_width*4
        linefont: shihuanfontsize
        anchors.bottom: root.bottom
        anchors.bottomMargin: shihuanfontsize
        anchors.right: root.right
        anchors.rightMargin: root.width*0.67
        z:upload.z+3

    }

    UploadRectange{
        id:upload
        visible: false
        implicitWidth:button_width*3
        implicitHeight: button_width*0.8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: button_width*0.5
        z:webEngine.z+2
    }

    UploadShow{
        id:uploadshow
        anchors.fill: parent
        visible: false
    }


    ShowMessage{
        id:showmessage
        visible:false
        implicitWidth: flightData_width*2.5
        implicitHeight: flightData_width*0.2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: latlondata.bottom
        anchors.topMargin: button_width
        fontsize:shihuanfontsize
    }

    //校准

    Component{
        id:panelloadComponent
        ShowAdjustRectange{
            id:adjustrectange
            anchors.fill: parent
        }
    }

    Loader{
        id:panelload
        anchors.fill: parent

        function setSource(source){
            panelload.sourceComponent = source
        }
    }






    Component.onCompleted:
    {
        root.showFlightMonitRectange.connect(root.qmlshowFlightMonitRectange)
        root.showRoutePlanRectange.connect(root.qmlshowRoutePlanRectange)
        root.showActionAdjust.connect(root.qmlshowActionAdjust)
        root.uploadToUAV.connect(root.uploadToUAVfun)
        root.showMessage.connect(root.showMessagefun)
        root.setAircraftCenter_fromVelocimeter.connect(someObject.setAircraftCenter)
        root.addPointItem.connect(playview.addSimpleMissionItem)
        root.removePointItem.connect(playview.deleteSimpleMissionItem)
        root.upPointItem.connect(playview.upSimpleMissionItem)
        root.downPointItem.connect(playview.downSimpleMissionItem)
    }

//    Component.onDestruction:  root.receFromWidget.disconnect(root.load)
}

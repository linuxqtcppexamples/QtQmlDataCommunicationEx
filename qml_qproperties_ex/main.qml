import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

import IUserData 1.0
import IRevisionEx 1.1
import IRevisionEx 1.2
//import IInvokeEx 1.0

import "./controls" //import another qml file

ApplicationWindow {


    id:mainwindow
    visible: true
    width: 640
    height: 480
    title: ""

//    CInvokeEx
//    {
//        id:data1invoke
//    }

    Component.onCompleted:
    {
        mainwindow.title= UserData.strUserName + regUserData.strUserName
      //  popupwin = Qt.createComponent("PopupDilaog.qml");

    }

    PopupDialog{

        id:popupwin
    }

    CUserData
    {
        id:regUserData


    }



    CRevisionEx
    {
        id:reviosnexId
        Component.onCompleted:
        {
            console.log("normalProperty value is:",normalProperty)
            console.log("newProperty value is: ",newProperty)
            console.log("newerProperty value is: ",newerProperty)

        }
    }

    signal qmlSignal(string msg)



//    Connections
//    {
//            target: data1invoke
//    }

    function myQmlFunction(msg)
    {
        console.log("Got message:", msg)
        return "Data received at qml"
    }


    Rectangle
    {
        id: tec1
        border.color: "#009c2b2b"
        gradient: Gradient {
            GradientStop {
                position: 0.23;
                color: "#ffffff";
            }
            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }
        anchors {
            fill: parent
            margins: 10
        }

        ColumnLayout
        {
            id: col1
            width:mainwindow.width
            TextField
            {
                id : textedi1
                Layout.fillWidth: true
                width:mainwindow.width
                height:50
                placeholderText: qsTr("Enter Username")

            }

            TextField
            {
                id : textedi2
                Layout.fillWidth: true
                width:mainwindow.width
                height:50
                placeholderText: qsTr("Enter Password")
            }


            CustomButton
            {
                id: bt1
                text: "Submit"
                Layout.fillWidth: true
                width:mainwindow.width

                onClicked:
                {
                    UserData.strUserName =textedi1.text + textedi2.text
                    regUserData.strUserName =textedi1.text + textedi2.text
                    mainwindow.qmlSignal(textedi1.text + textedi2.text)
                    popupwin.open()

                }

            }

            PropertyAlias
            {
                buttonText: "Please enter username and password."
                Layout.fillWidth: true
                width:mainwindow.width
            }

            Item {
                id: qtComponentBinding
                Layout.fillWidth: true
                width:mainwindow.width
                height: 50
                property string dynamicText: "Root text"

                Component.onCompleted: {
                    var c = Qt.createComponent("qrc:/controls/QtComponentBinding.qml")

                    var obj1 = c.createObject(qtComponentBinding, { 'text': Qt.binding(function() { return dynamicText + ' extra text' }) })
                    qtComponentBinding.dynamicText = "Modified qtComponentBinding text"

                    var obj2 = c.createObject(qtComponentBinding, { 'text': Qt.binding(function() { return this.dynamicText + ' extra text' }) })
                    qtComponentBinding.dynamicText = "Modified dynamic text"
                }
            }


            Item {
                id: loadSource
                property string dynamicText: "Root text"
                Layout.fillWidth: true
                width:mainwindow.width
                height: 50
                Loader {
                    id: loaderOne
                    onLoaded: loadSource.dynamicText = "Modified loadSource text"
                }

                Loader {
                    id: loaderTwo
                    onLoaded: item.dynamicText = "Modified dynamic text"
                }

                Component.onCompleted: {
                    loaderOne.setSource("qrc:/controls/QtComponentBinding.qml", { 'text': Qt.binding(function() { return dynamicText + ' extra text' }) })
                    loaderTwo.setSource("qrc:/controls/QtComponentBinding.qml", { 'text': Qt.binding(function() { return this.dynamicText + ' extra text' }) })
                }
            }
        }

    }

}


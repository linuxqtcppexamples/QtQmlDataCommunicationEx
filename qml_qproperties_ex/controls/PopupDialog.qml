import QtQuick 2.0
import QtQuick.Controls 2.4

Popup {

    id: popupWindow
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    width: 100
    height: 100
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    Text {
        id: submit
        text: qsTr("Submitted")
    }

}


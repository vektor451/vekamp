import QtQuick
import QtQuick.Controls
import VekAmp

Window {
    width: 960
    height: 600
    visible: true
    title: qsTr("VekAmp")

    BASSUIBackend{
        id: bassUI
    }

    Pane{
        id: pane
        anchors.fill: parent

        Label{
            id: label
            anchors.centerIn: parent
            text: qsTr("Hello World!")

            Button{
                text: qsTr("Click Me!")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -32
                flat: false
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: bassUI.qClickMe();
            }
        }
    }
}

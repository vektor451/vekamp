import QtQuick
import VekAmp
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Studio.DesignEffects

Window {
    width: 960
    height: 600

    visible: true

    title: qsTr("VekAmp")
    color: "#00000000"

    MainPlayer {
        id: mainScreen
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0

        data: [
            DesignEffect {
                effects: [
                    DesignDropShadow {
                        visible: true
                        color: "#56000000"
                        showBehind: false
                        offsetY: 0
                        blur: 8
                    }
                ]
            }
        ]
        wheelEnabled: true

        anchors.centerIn: parent
    }
}




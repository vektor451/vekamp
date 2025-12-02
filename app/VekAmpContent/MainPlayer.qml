

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import VekAmp
import QtQuick.Window
import QtQuick.Studio.DesignEffects

Pane {
    id: bg

    width: 960
    height: 600

    anchors.fill: parent
    padding: 2
    rightPadding: 2
    leftPadding: 2
    bottomPadding: 2
    topPadding: 2

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        MenuBar {
            id: menuBar
            anchors.fill: parent
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: false
            Layout.fillWidth: true

            Menu {
                title: qsTr("&File")
                Action { text: qsTr("&Open File")}
            }
        }

        ToolBar {
            id: toolBar
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            Layout.fillWidth: true

            RowLayout {
                id: playerControls
                anchors.fill: parent
                spacing: 4
                uniformCellSizes: false

                ToolButton {
                    id: toolButton
                    text: qsTr("<")
                    Layout.minimumHeight: 32
                    Layout.minimumWidth: 32
                    flat: true
                }

                ToolButton {
                    id: toolButton1
                    text: qsTr("P")
                    Layout.minimumHeight: 32
                    Layout.minimumWidth: 32
                    flat: true
                }

                ToolButton {
                    id: toolButton2
                    text: qsTr(">")
                    Layout.minimumHeight: 32
                    Layout.minimumWidth: 32
                    flat: true
                }

                ColumnLayout {
                    id: columnLayout1
                    width: 100
                    height: 100
                    Layout.topMargin: 4
                    Layout.rowSpan: 0


                    Label {
                        id: label
                        text: qsTr("Hello World")
                        Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                    }
                    Slider {
                        id: slider
                        value: 0.5
                        Layout.bottomMargin: 4
                        Layout.fillWidth: true
                    }
                }

                Slider {
                    id: slider1
                    value: 0.5
                    Layout.preferredWidth: 128
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.minimumWidth: 16
                    Layout.fillWidth: false
                    Layout.bottomMargin: 4
                }
            }

        }
    }
}

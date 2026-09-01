import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

Window {
    id: aboutDialog
    title: qsTr("Welcome to VekAmp")

    Pane{
        anchors.fill: parent;
        font: interFont.font;

        ColumnLayout{
            anchors.fill: parent

            Label {
                text: qsTr("**Welcome to VekAmp**")
                textFormat: Text.MarkdownText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                font.pointSize: 16
            }

            Label {
                text: qsTr("Initial configuration:")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            }

            Frame {
                id: frame1
                Layout.fillHeight: true
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    uniformCellSizes: false
                    layoutDirection: Qt.LeftToRight

                    Label {
                        text: qsTr("Library Directory:")
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    }

                    Frame {
                        id: frame
                        width: 200
                        height: 200
                        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                        Layout.fillWidth: true

                        RowLayout {
                            anchors.fill: parent
                            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            TextField {
                                id: textField
                                text: ""
                                Layout.fillWidth: true
                                placeholderText: qsTr("Library Directory")
                            }

                            Button {
                                id: button
                                text: qsTr("Browse")
                            }
                        }
                    }

                }
            }

            Button {
                text: qsTr("Confirm")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            }
        }
    }
}

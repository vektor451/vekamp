import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs

Item{
    property var aboutDialog: aboutDialog
    property var bassERROR: bassERROR
    property var bassERRORText: bassERRORText

    // ERROR WINDOW
    Window{
        id: bassERROR
        title: qsTr("BASS Error")
        maximumWidth: 256
        maximumHeight: 128
        minimumWidth: 256
        minimumHeight: 128
        flags: Qt.Dialog

        Pane{
            anchors.fill: parent;
            font: interFont.font;

            ColumnLayout{
                anchors.fill: parent;

                Label{
                    id: bassERRORText
                    text: "";
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    textFormat: Text.MarkdownText
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Button{
                    text: qsTr("OK")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    onClicked: bassERROR.hide()
                }
            }
        }
    }

    // ABOUT WINDOW
    Window{
        //TODO: get this an icon
        id: aboutDialog
        title: qsTr("About VekAmp")
        maximumWidth: 384
        maximumHeight: 200
        minimumWidth: 384
        minimumHeight: 200
        flags: Qt.Dialog

        Pane{
            anchors.fill: parent
            padding: 8
            font: interFont.font;

            ColumnLayout{
                anchors.fill: parent

                VectorImage{
                    source: "/Resources/wordmark.svg"
                    assumeTrustedSource: true
                    fillMode: VectorImage.Stretch
                    width: 360
                    height: 112
                    preferredRendererType: VectorImage.CurveRenderer
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                }

                Label{
                    text: miscUI.qGetAboutPromptString();
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                }

                Button{
                    text: qsTr("Close")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    onClicked: aboutDialog.hide()
                }
            }
        }
    }


}

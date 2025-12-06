import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

ApplicationWindow {
    width: 960
    height: 600
    minimumWidth: 640
    minimumHeight: 400
    visible: true
    title: qsTr("VekAmp")

    palette.highlight: "#cc3366"

    DropArea{
        id: fileDrop
        anchors.fill: parent
        onEntered: {
            drag.accept(Qt.LinkAction)
        }
        onDropped: {
            bassUI.qFileSelect(drop.urls[0])
        }
    }

    menuBar: MenuBar{
        font.pointSize: 8
        Menu{
            title: qsTr("&File")

            Action{
                text: qsTr("Open &File")
                onTriggered: fileDialog.open();
            }
        }
        Menu{
            title: qsTr("&Edit")

            Action{
                text: qsTr("&Preferences")

            }
        }
        Menu{
            title: qsTr("&About")

            Action{
                text: qsTr("&About VekAmp")
                onTriggered: aboutDialog.show()
            }

            Action{
                text: qsTr("Visit &GitHub Repository")
                onTriggered: Qt.openUrlExternally("https://github.com/vektor451/vekamp")
            }

            Action{
                text: qsTr("Report &Bug(s)")
                onTriggered: Qt.openUrlExternally("https://github.com/vektor451/vekamp/issues/new/choose")
            }
        }
    }

    BASSUIBackend{
        id: bassUI
        onTrackChanged: {
            trackName.text = tagUI.qGetCurTrackName();
        }
    }

    MiscUIBackend{
        id: miscUI
    }

    TagUIBackend{
        id: tagUI
    }

    FileDialog{
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        title: qsTr("Open audio file...")
        onAccepted: bassUI.qFileSelect(selectedFile)
    }

    Window{
        //TODO: get this an icon
        id: aboutDialog
        title: qsTr("About VekAmp")
        maximumWidth: 384
        maximumHeight: 240
        minimumWidth: 384
        minimumHeight: 200
        flags: Qt.Dialog


        Pane{
            anchors.fill: parent
            padding: 8

            ColumnLayout{
                anchors.fill: parent

                VectorImage{
                    source: "/images/Resources/wordmark.svg"
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

    Pane{
        anchors.fill: parent
        padding: 2

        ColumnLayout{
            anchors.fill: parent
            spacing: 2



            Frame {
                id: frame
                width: 200
                height: 200
                bottomPadding: 0
                padding: 0
                leftPadding: 0
                topPadding: 0
                contentWidth: 5
                Layout.fillHeight: true
                Layout.fillWidth: true

                RowLayout{
                    anchors.fill: parent
                    Layout.fillHeight: true
                    Layout.fillWidth: true



                    Frame {
                        id: frame1
                        width: 192
                        height: 200
                        verticalPadding: 2
                        topPadding: 2
                        rightPadding: 2
                        padding: 2
                        leftPadding: 2
                        Layout.preferredWidth: 192
                        Layout.fillHeight: true

                        ScrollView {
                            id: scrollView
                            anchors.fill: parent
                            Layout.preferredWidth: 192
                            Layout.fillHeight: true
                        }
                    }

                    Frame {
                        id: frame2
                        width: 200
                        height: 200
                        padding: 2
                        bottomPadding: 2
                        horizontalPadding: 2
                        Layout.preferredWidth: 192
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        ColumnLayout {
                            id: columnLayout
                            anchors.fill: parent
                            Layout.preferredHeight: 65535
                            Layout.fillHeight: true
                            Layout.preferredWidth: 192
                            Layout.alignment: Qt.AlignRight | Qt.AlignTop
                            spacing: 4
                            layoutDirection: Qt.LeftToRight
                            Layout.fillWidth: false

                            Image {
                                id: image
                                width: 100
                                height: 100
                                source: "Resources/wordmark.svg"
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                                mipmap: true
                                sourceSize.height: 192
                                sourceSize.width: 192
                                fillMode: Image.PreserveAspectFit
                            }
                        }
                    }
                }
            }

            ToolBar{
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                Layout.fillWidth: true
                RowLayout{
                    anchors.fill: parent
                    spacing: 4
                    Button{
                        text: qsTr("<")
                        Layout.bottomMargin: 4
                        Layout.topMargin: 4
                        Layout.margins: 0
                        Layout.preferredHeight: 32
                        Layout.preferredWidth: 2
                        Layout.minimumHeight: 32
                        Layout.minimumWidth: 32
                    }

                    Button{
                        text: qsTr("P")
                        Layout.bottomMargin: 4
                        Layout.topMargin: 4
                        Layout.margins: 0
                        Layout.preferredHeight: 32
                        Layout.preferredWidth: 2
                        Layout.minimumHeight: 32
                        Layout.minimumWidth: 32

                        onClicked: bassUI.qPlayPause();
                    }

                    Button{
                        text: qsTr(">")
                        Layout.bottomMargin: 4
                        Layout.topMargin: 4
                        Layout.margins: 0
                        Layout.preferredHeight: 32
                        Layout.preferredWidth: 2
                        Layout.minimumHeight: 32
                        Layout.minimumWidth: 32
                    }

                    ColumnLayout{
                        Layout.margins: 4
                        Layout.leftMargin: 16
                        Layout.rightMargin: 16
                        Layout.fillWidth: true
                        RowLayout{
                            Label{
                                id: trackName
                                text: qsTr("Welcome to VekAmp!")
                                Layout.fillWidth: true
                                textFormat: Text.MarkdownText
                                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            }

                            Label{
                                id: trackProgress
                                text: qsTr("00:00 / 00:00")
                                horizontalAlignment: Text.AlignRight
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                        }
                        Slider{
                            id: progressSlider
                            Layout.fillWidth: true
                            from: 0
                            value: 0
                            to: 1
                            live: true

                            onMoved: {
                                bassUI.qSetTrackProgress(value);
                            }

                            MouseArea{
                                id: progressArea
                                anchors.fill: parent
                                hoverEnabled: true

                                // TODO: Figure out way to pause playback while seeking
                                onPressed:{
                                    mouse.accepted = false;
                                    //bassUI.qSliderAdjustPause(true)
                                }

                                TapHandler{
                                    id: progressAreaTapHandler
                                    onGrabChanged: {
                                        "console.log: boo"
                                    }
                                }

                                DragHandler{
                                    id: progressAreaDragHandler
                                    onGrabChanged: {
                                        "console.log: boo"
                                    }
                                }
                            }

                            Timer{
                                id: progressTimer
                                interval: 10; running: true; repeat: true
                                onTriggered: {
                                    if(!progressAreaTapHandler.pressed)
                                    {
                                        progressSlider.value = bassUI.qGetTrackLen();
                                        trackProgress.text = bassUI.qGetTrackLenStr();
                                        bassUI.qSliderAdjustPause(false)
                                    }
                                    else
                                    {
                                        //console.log(progressAreaTapHandler.pressed);
                                    }
                                }
                            }
                        }
                    }

                    Slider{
                        id: volumeSlider
                        width: 128
                        Layout.maximumWidth: 128
                        Layout.minimumWidth: 64
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        from: 0
                        value: 0.4;
                        to: 0.8

                        onValueChanged: bassUI.qSetVolume(value)
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:3;invisible:true}D{i:25}D{i:27}
}
##^##*/

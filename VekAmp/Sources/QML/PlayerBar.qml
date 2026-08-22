import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs

ToolBar{
    RowLayout{
        anchors.fill: parent
        spacing: 4
        Button{
            icon.source: "/Resources/controlIcons/prev.svg"
            icon.height: 16
            icon.width: 16
            Layout.bottomMargin: 4
            Layout.topMargin: 4
            Layout.margins: 0
            Layout.preferredHeight: 32
            Layout.preferredWidth: 2
            Layout.minimumHeight: 32
            Layout.minimumWidth: 32
            flat: true

            onClicked: {
                bassUI.qPrevHomeTrack();
            }
        }

        Button{
            id: playButton;
            icon.source: "/Resources/controlIcons/play.svg"
            icon.height: 16
            icon.width: 16
            Layout.bottomMargin: 4
            Layout.topMargin: 4
            Layout.margins: 0
            Layout.preferredHeight: 32
            Layout.preferredWidth: 2
            Layout.minimumHeight: 32
            Layout.minimumWidth: 32
            flat: true

            function updatePlayingIcon(){
                if(bassUI.qIsPlaying())
                {
                    icon.source = "/Resources/controlIcons/pause.svg"
                }
                else
                {
                    icon.source = "/Resources/controlIcons/play.svg"
                }
            }

            onClicked: {
                bassUI.qPlayPause();
                updatePlayingIcon();
            }
        }

        Button{
            icon.source: "/Resources/controlIcons/next.svg"
            icon.height: 16
            icon.width: 16
            Layout.bottomMargin: 4
            Layout.topMargin: 4
            Layout.margins: 0
            Layout.preferredHeight: 32
            Layout.preferredWidth: 2
            Layout.minimumHeight: 32
            Layout.minimumWidth: 32
            flat: true

            onClicked: {
                bassUI.qNextTrack();
            }
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
                to: 0.9995
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

        Button{
            icon.source: "/Resources/controlIcons/speaker.svg"
            icon.width: 16
            icon.height: 16
            height: 32
            Layout.preferredHeight: 24
            width: 32
            Layout.preferredWidth: 24
            flat: true

            //TODO: Implement muting functionality. I'm making this a button cause it themes easily.
        }

        Slider{
            id: volumeSlider
            width: 96
            Layout.maximumWidth: 96
            Layout.minimumWidth: 64
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            from: 0
            value: 0.4;
            to: 0.8

            onValueChanged: bassUI.qSetVolume(value)
        }

        Button{
            icon.source: "/Resources/controlIcons/noRepeat.svg"
            icon.height: 20
            icon.width: 20
            Layout.leftMargin: 12
            Layout.bottomMargin: 4
            Layout.topMargin: 4
            Layout.margins: 0
            Layout.preferredHeight: 32
            Layout.preferredWidth: 2
            Layout.minimumHeight: 32
            Layout.minimumWidth: 32
            flat: true

            onClicked: {
                let repeatNum = bassUI.qToggleRepeatMode();
                switch (repeatNum) {
                case 0:
                    icon.source = "/Resources/controlIcons/noRepeat.svg"
                    break;
                case 1:
                    icon.source = "/Resources/controlIcons/repeat.svg"
                    break;
                case 2:
                    icon.source = "/Resources/controlIcons/repeatOne.svg"
                    break;
                }
            }
        }

        Button{
            icon.source: "/Resources/controlIcons/noShuffle.svg"
            icon.height: 20
            icon.width: 20
            Layout.bottomMargin: 4
            Layout.topMargin: 4
            Layout.margins: 0
            Layout.preferredHeight: 32
            Layout.preferredWidth: 2
            Layout.minimumHeight: 32
            Layout.minimumWidth: 32
            flat: true

            onClicked: {
                if(bassUI.qToggleShuffleMode())
                {
                    icon.source = "/Resources/controlIcons/shuffle.svg"
                }
                else
                {
                    icon.source = "/Resources/controlIcons/noShuffle.svg"
                }
            }
        }
    }
}

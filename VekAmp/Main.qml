import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

Window {
    width: 960
    height: 600
    minimumWidth: 640
    minimumHeight: 400
    visible: true
    title: qsTr("VekAmp")

    //palette.highlight: "#cc3366" palette only works with ApplicationWindow, which none of the editors support as a root element.

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

    BASSUIBackend{
        id: bassUI
        onTrackChanged: {
            trackName.text = tagUI.qGetCurTrackName();
            tagUI.qUpdateAlbumCover();
            playButton.updatePlayingIcon();
        }
        onBassError: {
            bassERROR.show();
            bassERRORText.text = message;
        }
        onPlayStateChanged: {
            playButton.updatePlayingIcon();
        }
    }

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

    MiscUIBackend{
        id: miscUI
    }

    TagUIBackend{
        id: tagUI
        onUpdateImage: {
            console.log("triggered image update")
            coverImage.source = "/Resources/emptycover.png"
            coverImage.update()
            coverImage.source = "image://coverImage"
            coverImage.update()
        }
    }

    FileDialog{
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        title: qsTr("Open audio file(s)...")
        onAccepted: {
            bassUI.qMultifileSelect(selectedFiles)
            bassUI.qFileSelect(selectedFile);
        }
        fileMode: FileDialog.OpenFiles
    }

    FileDialog{
        id: playlistDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0]
        title: qsTr("Open playlist...")
        nameFilters: ["MP3 URL Playlist (*.m3u *.m3u8)"]
        //onAccepted: bassUI.qFileSelect(selectedFile)
        fileMode: FileDialog.OpenFile
    }

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

    Pane{
        anchors.fill: parent
        padding: 0

        ColumnLayout{
            anchors.fill: parent
            spacing: 0

            MenuBar{
                Layout.fillWidth: true
                font.pointSize: 8
                Menu{
                    title: qsTr("&File")

                    Action{
                        text: qsTr("Open &File(s)")
                        onTriggered: fileDialog.open();
                    }
                    Action{
                        // Right now M3U is too non-standard for me to really care for adding support.
                        text: qsTr("Open &Playlist")
                        onTriggered: playlistDialog.open();
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
                        verticalPadding: 2
                        bottomPadding: 3
                        horizontalPadding: 2
                        Layout.preferredWidth: 262
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        ColumnLayout {
                            id: columnLayout
                            anchors.fill: parent
                            Layout.preferredHeight: 65535
                            Layout.fillHeight: true
                            width: 256
                            Layout.preferredWidth: 256
                            Layout.alignment: Qt.AlignRight | Qt.AlignTop
                            spacing: 4
                            layoutDirection: Qt.LeftToRight
                            Layout.fillWidth: false

                            Image {
                                id: coverImage
                                verticalAlignment: Image.AlignVCenter
                                source: "image://coverImage"
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                                mipmap: true
                                sourceSize.height: 256
                                sourceSize.width: 256
                                height: 256
                                width: 256
                                fillMode: Image.PreserveAspectFit
                                cache: false;
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

                    VectorImage{
                        source: "/Resources/controlIcons/speaker.svg"
                        height: 16
                        width: 16
                        preferredRendererType: VectorImage.CurveRenderer
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
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:3;invisible:true}
}
##^##*/

import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

Window {
    id: window;
    width: 960
    height: 600
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("VekAmp")

    //palette.highlight: "#cc3366" palette only works with ApplicationWindow, which none of the editors support as a root element.

    onActiveFocusItemChanged: {
        print("focus changed: " + activeFocusItem);
    }

    FontLoader{
        id: interFont;
        source: "/Resources/interFont/Inter_18pt-Regular.ttf"
    }

    FontLoader{
        id: interFontBold;
        source: "/Resources/interFont/Inter_18pt-Medium.ttf"
    }

    FontLoader{
        id: interFontItalic;
        source: "/Resources/interFont/Inter_18pt-Italic.ttf"
    }

    FontLoader{
        id: interFontBoldItalic;
        source: "/Resources/interFont/Inter_18pt-MediumItalic.ttf"
    }

    MiscPrompts{
        id: miscPrompts
    }

    DropArea{
        id: fileDrop
        anchors.fill: parent
        onEntered: {
            drag.accept(Qt.LinkAction)
        }
        onDropped: {
            bassUI.qMultifileSelect(drop.urls)
            bassUI.qFileSelect(drop.urls[0])
        }
    }

    BASSUIBackend{
        id: bassUI
    }

    TagUIBackend{
        id: tagUI
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

    Pane{
        anchors.fill: parent
        padding: 0
        font.family: interFont.font.family;

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
                        onTriggered: miscPrompts.aboutDialog.show()
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
                padding: 0
                Layout.fillHeight: true
                Layout.fillWidth: true

                RowLayout{
                    anchors.fill: parent

                    Frame {
                        verticalPadding: 2
                        horizontalPadding: 2
                        Layout.preferredWidth: 192
                        Layout.fillHeight: true
                    }

                    Frame {
                        verticalPadding: 2
                        bottomPadding: 3
                        horizontalPadding: 2
                        Layout.preferredWidth: 262
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        ColumnLayout {
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.fillWidth: false
                            spacing: 2

                            Label{
                                text: "**Track Queue**"
                                textFormat: Text.MarkdownText
                                topPadding: 1
                                leftPadding: 2
                                bottomPadding: 1
                            }

                            TrackQueue{
                                id: trackQueue
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            TrackMetaView{
                                Layout.fillWidth: true
                                Layout.fillHeight: false
                            }

                        }
                    }
                }
            }

            PlayerBar{
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                Layout.fillWidth: true
            }
        }
    }
}

/*##^##
Designer {
    D{i:0}D{i:3;invisible:true}
}
##^##*/

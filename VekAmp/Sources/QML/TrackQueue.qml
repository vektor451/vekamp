import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

Item {
    BASSUIBackend{
        id: bassUI
        onNewTrackQueue: {
            trackQueue.listModel.clear();

            for (var i = 0; i < bassUI.qGetTrackQueueLength(); i++)
            {
                var fPath = bassUI.qGetTrackFileNameAtIndex(i);
                var title = tagUI.qGetTrackTitle(fPath);
                var artist = tagUI.qGetTrackArtist(fPath);
                var length = tagUI.qGetTrackLength(fPath);

                trackQueue.listModel.append({trackTitle: title, trackArtist: artist, trackLength: length})
            }
        }
    }

    property var listModel: queueModel

    ScrollView {
        id: scrollView1
        anchors.fill: parent
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        clip: true

        ListView {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 2
            clip: true

            model: ListModel{
                id: queueModel
            }

            delegate: Frame {
                required property string trackTitle
                required property string trackArtist
                required property string trackLength

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 0
                anchors.rightMargin: 12
                transformOrigin: Item.Center
                padding: 2

                contentWidth: trackQueueLayout.implicitWidth
                contentHeight: trackQueueLayout.implicitHeight

                RowLayout {
                    id: trackQueueLayout
                    anchors.fill: parent
                    Layout.fillWidth: true
                    spacing: 4

                    Image {
                        //id: queueImage
                        source: "/Resources/emptycover.png"
                        verticalAlignment: Image.AlignVCenter
                        sourceSize.width: 32
                        sourceSize.height: 32
                        fillMode: Image.PreserveAspectFit
                        cache: false;
                        Layout.fillHeight: true
                        Layout.fillWidth: false
                    }
                    ColumnLayout{
                        Layout.fillHeight: false
                        Layout.alignment: Qt.AlignVCenter
                        spacing: 2

                        Label {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignVCenter
                            text: trackTitle
                            wrapMode: Text.NoWrap
                            elide: Text.ElideRight
                        }
                        Label {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignVCenter
                            text: trackArtist
                            wrapMode: Text.NoWrap
                            elide: Text.ElideRight
                            color: "grey"
                        }
                    }
                    Label {
                        Layout.fillWidth: false
                        textFormat: Text.RichText
                        text: trackLength
                        width: parent.width;
                        wrapMode: Text.NoWrap
                        Layout.alignment: Qt.AlignRight
                        rightPadding: 4
                    }
                }

                MouseArea {
                    id: trackQueueDrag
                    acceptedButtons: Qt.LeftButton
                    anchors.fill: parent

                    onClicked: (mouse) => {
                        if (mouse.button === Qt.RightButton)
                        {
                            //trackQueueContextMenu.
                        }
                    }
                }

                Drag.active: trackQueueDrag.drag.active

                ContextMenu.menu: Menu{
                    id: trackQueueContextMenu
                    MenuItem {
                        text: qsTr("&Play Now")
                    }
                }
            }
        }
    }
}

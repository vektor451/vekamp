import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
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
                    //parent.width: implicitWidth
                    //parent.height: implicitHeight

                    Image {
                        //id: queueImage
                        source: "/Resources/emptycover.png"
                        verticalAlignment: Image.AlignVCenter
                        //width: 32
                        //height: 32
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
                            //id: queueTrackDetailLabel
                            //textFormat: Text.RichText
                            text: trackTitle
                            lineHeight: 1
                            font.pointSize: 8
                            wrapMode: Text.NoWrap
                            elide: Text.ElideRight
                            //leftPadding: 2
                        }
                        Label {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignVCenter
                            //id: queueTrackDetailLabel
                            //textFormat: Text.RichText
                            text: trackArtist
                            lineHeight: 1
                            font.pointSize: 8
                            wrapMode: Text.NoWrap
                            elide: Text.ElideRight
                            color: "grey"
                            //leftPadding: 2
                        }
                    }
                    Label {
                        Layout.fillWidth: false
                        textFormat: Text.RichText
                        text: trackLength
                        lineHeight: 1
                        font.pointSize: 8
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

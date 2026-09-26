import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

SplitView{

    LibraryUIBackend{
        onRefreshLibrary: {
            // Categories
            categoryModel.clear();
            for (var i = 0; i < qGetRecordCategoryCount(); i++)
            {
                var entry = qGetRecordCategoryEntry(i);
                categoryModel.append({categoryName: entry.categoryName, extraInfo: entry.extraInfo})
            }

            print("library refreshed!");
        }
    }

    Frame { // Categories
        verticalPadding: 2
        horizontalPadding: 2
        SplitView.preferredWidth: 192
        SplitView.minimumWidth: 128
        SplitView.fillHeight: true

        ScrollView {
            anchors.fill: parent
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            clip: true

            ListView {
                id: categoryView
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 2
                clip: true

                acceptedButtons: Qt.NoButton

                boundsBehavior: Flickable.StopAtBounds

                model: ListModel{
                    id: categoryModel
                }

                delegate: Frame {
                    required property string categoryName;
                    required property string extraInfo;
                    required property int index;

                    id: categoryFrame
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 0
                    anchors.rightMargin: 12
                    transformOrigin: Item.Center
                    padding: 2

                    contentWidth: libraryCategoryLayout.implicitWidth
                    contentHeight: libraryCategoryLayout.implicitHeight

                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: -1
                        anchors.rightMargin: -1
                        anchors.topMargin: -1
                        anchors.bottomMargin: -1

                        color: categoryFrame.activeFocus ?
                                   mainPalette.highlight : categoryView.currentIndex === index ?
                                       "#10FFFFFF" : "#00000000"
                    }

                    RowLayout{
                        id: libraryCategoryLayout
                        anchors.fill: parent
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
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter || Qt.AlignLeft
                            spacing: 2

                            Label{
                                text: categoryName
                                Layout.alignment: Qt.AlignLeft
                                Layout.fillWidth: true
                                //leftPadding: 4
                                wrapMode: Text.NoWrap
                                elide: Text.ElideRight
                            }

                            Label{
                                text: extraInfo
                                Layout.alignment: Qt.AlignLeft
                                Layout.fillWidth: true
                                //leftPadding: 4
                                wrapMode: Text.NoWrap
                                elide: Text.ElideRight
                                color: "grey"
                            }
                        }

                        focus: true
                        focusPolicy: Qt.ClickFocus
                    }

                    MouseArea{
                        anchors.fill: parent
                        anchors.leftMargin : -3
                        anchors.rightMargin : -3
                        anchors.topMargin : -3
                        anchors.bottomMargin : -3

                        onPressed: {
                            categoryView.currentIndex = index
                            categoryFrame.forceActiveFocus()

                        }

                        //onDoubleClicked: {
                        //
                        //}
                    }
                }
            }
        }
    }

    Frame { // Records
        verticalPadding: 2
        horizontalPadding: 2
        SplitView.minimumWidth: 512
        SplitView.fillWidth: true
        SplitView.fillHeight: true

        ScrollView{
            anchors.fill: parent
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            clip: true

            ListView {
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 2
                clip: true

                boundsBehavior: Flickable.StopAtBounds

                //interactive: false

                model: ListModel {
                    id: recordModel
                    ListElement{albumName: "Cool Album"}
                    ListElement{albumName: "Cool Album"}
                    ListElement{albumName: "Cool Album"}
                    ListElement{albumName: "Cool Album"}
                }

                delegate: Pane {
                    required property string albumName;

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 0
                    anchors.rightMargin: 12
                    transformOrigin: Item.Center
                    leftPadding: 8
                    topPadding: 8
                    rightPadding: 0
                    bottomPadding: 8

                    RowLayout{
                        id: recordsLayout
                        anchors.fill: parent
                        spacing: 12

                        Image {
                            //id: queueImage
                            source: "/Resources/emptycover.png"
                            verticalAlignment: Image.AlignVCenter
                            sourceSize.width: 128
                            sourceSize.height: 128
                            fillMode: Image.PreserveAspectFit
                            Layout.alignment: Qt.AlignTop
                            cache: false;
                        }

                        Pane {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignTop
                            padding: 0
                            //height: tracksListView.contentHeight

                            //background: Rectangle {
                            //    color: "blue"
                            //}

                            ColumnLayout {
                                //Layout.fillWidth: true
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.leftMargin: 0
                                anchors.rightMargin: 12

                                Label {
                                    text: "Album Name"
                                    font.pointSize: 14
                                    font.bold: true

                                    Layout.fillWidth: true
                                }

                                RowLayout {
                                    Label {
                                        text: "2069 | Epic Tunes"
                                        color: "grey"

                                        wrapMode: Text.NoWrap
                                        elide: Text.ElideRight

                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: "420 mins"
                                        color: "grey"
                                    }
                                }

                                ListView {
                                    id: tracksListView
                                    height: contentHeight
                                    Layout.fillWidth: true
                                    spacing: 0
                                    interactive: false
                                    //clip: true

                                    model: ListModel {
                                        id: trackModel

                                        ListElement{
                                            trackNum: "01"
                                            trackTitle: "The quick brown fox jumps over the lazy dog super crazy style he really shouldn't be doing that."
                                            trackArtist: "James"
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "02"
                                            trackTitle: "Track Title"
                                            trackArtist: "Some silly long name goober"
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "03"
                                            trackTitle: "Track Title"
                                            trackArtist: "James"
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "01"
                                            trackTitle: "Track Title"
                                            trackArtist: "James"
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "02"
                                            trackTitle: "Track Title"
                                            trackArtist: "James"
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "03"
                                            trackTitle: "Track Title"
                                            trackArtist: "James"
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "01"
                                            trackTitle: "Track Title"
                                            trackArtist: ""
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "02"
                                            trackTitle: "Track Title"
                                            trackArtist: ""
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "99"
                                            trackTitle: "Track Title"
                                            trackArtist: ""
                                            trackLength: "4:20"
                                        }

                                        ListElement{
                                            trackNum: "1"
                                            trackTitle: "Track Title"
                                            trackArtist: ""
                                            trackLength: "4:20"
                                        }
                                    }

                                    delegate: Pane{
                                        id: trackPane

                                        required property string trackNum
                                        required property string trackTitle
                                        required property string trackArtist
                                        required property string trackLength
                                        required property int index

                                        anchors.left: parent.left
                                        anchors.right: parent.right
                                        anchors.leftMargin: 0
                                        anchors.rightMargin: 0
                                        transformOrigin: Item.Center

                                        Layout.fillWidth: true
                                        bottomPadding: 4
                                        topPadding: 4
                                        rightPadding: 8
                                        leftPadding: 0

                                        contentWidth: trackLayout.implicitWidth
                                        contentHeight: trackLayout.implicitHeight

                                        background: Rectangle {
                                            color: index % 2 == 0 ? "#04FFFFFF" : "#00FFFFFF"
                                        }

                                        RowLayout {
                                            id: trackLayout
                                            anchors.fill: parent

                                            Label { // Track Num
                                                text: trackNum
                                                horizontalAlignment: Text.AlignRight
                                                Layout.preferredWidth: 24
                                                rightPadding: 0
                                                color: "grey"
                                            }

                                            Label {
                                                text: trackTitle
                                                leftPadding: 4
                                                rightPadding: 4
                                                Layout.fillWidth: true
                                                wrapMode: Text.NoWrap
                                                elide: Text.ElideRight
                                            }

                                            Label {
                                                text: trackArtist
                                                color: "grey"
                                                Layout.fillWidth: true
                                                //wrapMode: Text.NoWrap
                                                //elide: Text.ElideRight
                                                horizontalAlignment: Text.AlignRight
                                                rightPadding: 12
                                                leftPadding: 4
                                            }

                                            Label {
                                                Layout.alignment: Qt.AlignRight
                                                text: trackLength
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

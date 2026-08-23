import QtCore
import QtQuick
import QtQuick.VectorImage
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs
import VekAmp

ColumnLayout {
    spacing: 2

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

    BASSUIBackend{
        id: bassUI
        onTrackChanged: {
            tagUI.qUpdateAlbumCover();
            trackDetailLabel.text = tagUI.qGetTrackDetailStr();
        }
    }

    Frame{
        padding: 4
        horizontalPadding: 4
        Layout.fillWidth: true
        Layout.fillHeight: false

        Label {
            id: trackDetailLabel
            textFormat: Text.RichText
            text: qsTr("Track details will appear here.")
            lineHeight: 1
            font.pointSize: 8
            width: parent.width;
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }
    }

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

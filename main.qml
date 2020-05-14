import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    width: 460
    height: 780
    title: qsTr("Bluetooth Pairing Example")

    Connections {
        target: Bluetooth
        onDevicesChanged: {
            if (Bluetooth.devices && Bluetooth.devices[devicesList.currentIndex]) {
                devicesList.itemAtIndex(devicesList.currentIndex).isDevicePaired =
                            Bluetooth.devices[devicesList.currentIndex].isPaired;
            }
        }
    }

    Item {
        anchors.fill: parent
        anchors.margins: 25
        ToolButton {
            id: refreshButton
            implicitWidth: 35
            implicitHeight: 35
            anchors.right: parent.right
            scale: pressed ? 1.1 : 1.0
            Behavior on scale { NumberAnimation {} }
            background: Image {
                anchors.fill: parent
                source: "qrc:/images/refresh.png"
            }
            onClicked: {
                Bluetooth.refresh();
            }
        }
        ScrollView {
            implicitWidth: parent.width
            anchors.top: refreshButton.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            ListView {
                id: devicesList
                model: Bluetooth.devices
                delegate: ItemDelegate {
                    id: deviceDelegate
                    property bool isDevicePaired: false
                    implicitWidth: parent.width
                    implicitHeight: 50
                    scale: pressed ? 1.1 : 1.0
                    Behavior on scale { NumberAnimation {} }
                    contentItem: RowLayout {
                        anchors.fill: parent
                        spacing: 20
                        Image {
                            Layout.preferredWidth: 20
                            Layout.preferredHeight: 35
                            fillMode: Image.PreserveAspectFit
                            source: "qrc:/images/bluetooth.png"
                        }
                        Label {
                            text: modelData.deviceName
                        }
                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                        Switch {
                            id: switchButton
                            checkable: false
                            checked: deviceDelegate.isDevicePaired
                        }
                    }
                    background: Rectangle {
                        width: parent.width
                        height: 1
                        anchors.bottom: parent.bottom
                        color: "lightgrey"
                    }
                    onClicked: {
                        devicesList.currentIndex = index;
                        Bluetooth.pairDevice(modelData.deviceAddress);
                    }
                }
            }
        }
    }

    Label {
        id: noDevicesLabel
        anchors.centerIn: parent
        font.pixelSize: 36
        opacity: (devicesList.count > 0) ? 0.0 : 1.0
        Behavior on opacity { NumberAnimation {}}
        visible: (opacity > 0.0)
        horizontalAlignment: Text.AlignHCenter
        text: Bluetooth.scanning ? "Scanning..." : "No devices found \n Press refresh to try again"
        SequentialAnimation {
            running: Bluetooth.scanning
            loops: Animation.Infinite
            ScaleAnimator {
                target: noDevicesLabel
                from: 0.9
                to: 1
                duration: 1000
            }
            ScaleAnimator {
                target: noDevicesLabel
                from: 1
                to: 0.9
                duration: 1000
            }
        }
    }
}

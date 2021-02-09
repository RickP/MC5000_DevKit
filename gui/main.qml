import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

ApplicationWindow {
    id: window
    width: num_editors > 0 ? 300 * num_editors : 300;
    height: 400
    minimumWidth: num_editors > 0 ? 300 * num_editors : 300;
    minimumHeight: 400
    visible: true
    title: qsTr("MC4000E V0.1")

    property int num_editors: serial.mcuConnections

    header: ToolBar {

        visible: serial.mcuConnections > 0

        RowLayout {
            anchors.fill: parent

            Row {
                Layout.leftMargin: 10
                spacing: 10

                ToolButton {
                    text: qsTr("Save")
                    onClicked: serial.connect()
                }

                ToolButton {
                    text: qsTr("Open")
                }
            }

            ToolButton {
                Layout.rightMargin: 10
                text: qsTr("Upload")
                Layout.alignment: Qt.AlignRight
            }
        }
    }


    Column {
        id: connection
        visible: serial.mcuConnections == 0
        anchors.centerIn: parent
        width: 200

        ComboBox {
            id: serial_ports
            width: parent.width
            model: serial.serialports
        }

        Button {
            width: parent.width
            text: "Connect"
            onClicked: serial.connect(serial_ports.currentText)
        }
    }



    Item {
        id: editors
        visible: serial.mcuConnections > 0
        anchors.fill: parent

        Row {
            anchors.fill: parent

            Repeater {
                model: Array(num_editors)
                delegate: Rectangle {
                    height: parent.height
                    width: parent.width / num_editors
                    color: "lightgrey"
                    Rectangle {
                        anchors.margins: 2
                        anchors.fill: parent
                        border.color: "grey"
                        color: "white"

                        Column {
                            spacing: 2
                            anchors.fill: parent

                            Rectangle {
                                width: parent.width
                                height: 25
                                color: "black"
                                Text {
                                    anchors.fill: parent
                                    font.pointSize: 18
                                    text: "MCU " + (index + 1)
                                    color: "white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                            TextEdit {
                                selectByMouse: true
                                textMargin: 10
                                width: parent.width
                                height: parent.height - 62
                                font.pointSize: 16
                                wrapMode: TextEdit.NoWrap
                                clip: true
                            }

                            Rectangle {
                                width: parent.width
                                height: 30
                                color: "black"
                                Row {
                                    anchors.margins: 4
                                    spacing: 4
                                    anchors.fill: parent
                                    Rectangle {
                                        height: parent.height
                                        width: parent.width / 2 - 2
                                        color: "white"
                                        RowLayout {
                                            spacing: 0
                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                font.pointSize: 18
                                                text: " ACC "
                                                color: "black"
                                            }

                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignRight
                                                font.pointSize: 18
                                                text: "-999"
                                                color: "black"
                                            }
                                        }
                                    }
                                    Rectangle {
                                        height: parent.height
                                        width: parent.width / 2 - 2
                                        color: "white"
                                        RowLayout {
                                            spacing: 0
                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                font.pointSize: 18
                                                text: " DAT "
                                                color: "black"
                                            }

                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignRight
                                                font.pointSize: 18
                                                text: "100"
                                                color: "black"
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

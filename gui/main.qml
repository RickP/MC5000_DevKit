import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.1
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0



ApplicationWindow {
    id: window
    width: serial.mcuConnections > 0 ? 300 * serial.mcuConnections : 300;
    height: 400
    minimumWidth: serial.mcuConnections > 0 ? 300 * serial.mcuConnections : 300;
    minimumHeight: 400
    visible: true
    title: qsTr("MC4000E V0.1")

    property bool upload: false

    Timer {
        interval: 200 + (100 * serial.mcuConnections)
        running: serial.mcuConnections > 0 && !upload
        repeat: true
        onTriggered: serial.updateRegisters()
    }

    header: ToolBar {

        visible: serial.mcuConnections > 0

        RowLayout {
            anchors.fill: parent

            Row {
                Layout.leftMargin: 10
                spacing: 10

                ToolButton {
                    text: qsTr("Save")
                    onClicked: saveDialog.open()
                }

                ToolButton {
                    text: qsTr("Open")
                    onClicked: loadDialog.open()
                }
            }

            ToolButton {
                Layout.rightMargin: 10
                text: qsTr("Upload")
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    upload = true;
                    var code_strings = [];
                    for (var i=0; i < editors.count; i++) {
                        code_strings.push(editors.itemAt(i).children[0].children[0].children[1].text);
                    }
                    console.log(code_strings);
                    serial.upload(code_strings);
                    upload = false;
                }
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
        visible: serial.mcuConnections > 0
        anchors.fill: parent

        Row {
            anchors.fill: parent

            Repeater {
                id: editors
                model: Array(serial.mcuConnections)
                delegate: Rectangle {
                    height: parent.height
                    width: parent.width / serial.mcuConnections
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
                                id: editor
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
                                                text: serial.accRegisters[index]
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
                                                text: serial.datRegisters[index]
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

    FileDialog {
        id: loadDialog
        title: "Load file"
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
        }
        onRejected: {
            console.log("Canceled")
        }
        fileMode: FileDialog.OpenFile
    }

    FileDialog {
        id: saveDialog
        title: "Save File"
        fileMode: FileDialog.SaveFile

    }

}



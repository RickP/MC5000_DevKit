import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import Qt.labs.platform 1.1

ApplicationWindow {
    id: window
    width: serial.mcuConnections > 0 ? 300 * serial.mcuConnections : 300;
    height: 426
    minimumWidth: serial.mcuConnections > 0 ? 300 * serial.mcuConnections : 300;
    minimumHeight: 400
    visible: true
    title: qsTr("MC5000 V1.0")
    property int maxLines: 16

    Timer {
        interval: 100 + (50 * serial.mcuConnections)
        running: serial.mcuConnections > 0
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

            Row {
                Layout.rightMargin: 10
                spacing: 10
                Layout.alignment: Qt.AlignRight

                ToolButton {
                    text: qsTr("Manual")
                    onClicked: Qt.openUrlExternally(manualUrl)
                }

                ToolButton {
                    text: qsTr("Upload")
                    Layout.alignment: Qt.AlignRight
                    onClicked: {
                        var code_strings = [];
                        for (var i=0; i < editors.count; i++) {
                            code_strings.push(editors.itemAt(i).children[0].children[1].children[1].text);
                        }
                        serial.startUpload(code_strings);
                        messageDialog.visible = (serial.errorMessage != "");
                    }
                }
            }
        }
    }


    Column {
        id: connection
        visible: serial.mcuConnections == 0
        anchors.centerIn: parent
        width: 200

        Text {
            visible: serial.serialports.length === 0
            height: 15
            width: parent.width
            anchors.horizontalCenterOffset: 0
            font.pixelSize: 18
            font.bold: true
            text: "No serial devices found"
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ComboBox {
            id: serial_ports
            width: parent.width
            model: serial.serialports
            visible: serial.serialports.length > 0
        }

        Button {
            width: parent.width
            text: "Connect"
            onClicked: serial.connectPort(serial_ports.currentText)
            visible: serial.serialports.length > 0

            Component.onCompleted: {
                if (serial.serialports.length === 1) {
                    clicked();
                }
            }
        }

        Button {
            width: 200
            text: "Reload"
            onClicked: serial.loadPorts()
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
                        anchors.margins: 5
                        anchors.fill: parent
                        border.color: "lightgrey"
                        color: "white"

                        Rectangle {
                            color: "lightyellow"
                            visible: editor.focus
                            height: editor.cursorRectangle.height
                            width: editor.width
                            y: editor.cursorRectangle.y + editor.y
                        }

                        Column {
                            spacing: 2
                            anchors.fill: parent

                            Rectangle {
                                width: parent.width
                                height: 25
                                color: "black"
                                Text {
                                    anchors.fill: parent
                                    font.pixelSize: 18
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
                                height: parent.height - 55
                                font.pixelSize: 16
                                wrapMode: TextEdit.NoWrap
                                clip: true
                                onTextChanged: {
                                    if (lineCount > maxLines) undo();
                                }
                            }

                            Rectangle {
                                width: parent.width
                                height: 28
                                color: "black"
                                Row {
                                    anchors.margins: 2
                                    spacing: 2
                                    anchors.fill: parent
                                    Rectangle {
                                        height: parent.height
                                        width: ((parent.width - parent.height) / 2) - 2
                                        color: "white"
                                        RowLayout {
                                            spacing: 0
                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                font.pixelSize: 18
                                                text: " ACC "
                                                color: "black"
                                            }

                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignRight
                                                font.pixelSize: 18
                                                text: serial.accRegisters[index]
                                                color: "black"
                                            }
                                        }
                                    }
                                    Rectangle {
                                        height: parent.height
                                        width: ((parent.width - parent.height) / 2) - 2
                                        color: "white"
                                        RowLayout {
                                            spacing: 0
                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                font.pixelSize: 18
                                                text: " DAT "
                                                color: "black"
                                            }

                                            Text {
                                                height: parent.height
                                                verticalAlignment: Text.AlignVCenter
                                                horizontalAlignment: Text.AlignRight
                                                font.pixelSize: 18
                                                text: serial.datRegisters[index]
                                                color: "black"
                                            }
                                        }
                                    }
                                    Rectangle {
                                        height: parent.height
                                        width: parent.height
                                        color: serial.isProgrammed[index] ? "green" : "red"
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
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            var request = new XMLHttpRequest();
            request.open("GET", file);
            request.onreadystatechange = function() {
                if (request.readyState === XMLHttpRequest.DONE) {
                    var response = request.responseText;
                    response.split(',').forEach(function(editortext, index) {
                        if (editors.count > index) {
                            editors.itemAt(index).children[0].children[1].children[1].text = editortext;
                        }
                    })
                }
            };
            request.send();
        }
    }

    FileDialog {
        id: saveDialog
        title: "Save File"
        fileMode: FileDialog.SaveFile
        defaultSuffix: "txt"
        onAccepted: {
            var request = new XMLHttpRequest();
            request.open("PUT", file, false);
            var code_strings = [];
            for (var i=0; i < editors.count; i++) {
                code_strings.push(editors.itemAt(i).children[0].children[1].children[1].text);
            }
            request.send(code_strings);
        }
    }

    function openFile(fileUrl) {
        var request = new XMLHttpRequest();
        request.open("GET", fileUrl, false);
        request.send(null);
        return request.responseText;
    }

    function saveFile(fileUrl, text) {
        var request = new XMLHttpRequest();
        request.open("PUT", fileUrl, false);
        request.send(text);
        return request.status;
    }

    footer: Item {
            id: messageDialog
            visible: false
            width: parent.width
            height: 40

            Rectangle {
                width: parent.width - 80
                height: parent.height
                color: "yellow"
            }

            Row {
                width: parent.width
                height: parent.height
                anchors.margins: 10

                Text {
                    leftPadding: 5
                    topPadding: 2
                    wrapMode: Text.WordWrap
                    height: parent.height
                    width: parent.width - 80
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: 14
                    font.bold: true
                    text: serial.errorMessage
                    color: "black"
                }
                DialogButtonBox {
                    height: parent.height
                    standardButtons: DialogButtonBox.Ok
                    alignment: Qt.AlignRight
                    onClicked: messageDialog.visible = false
                }
            }
        }
}



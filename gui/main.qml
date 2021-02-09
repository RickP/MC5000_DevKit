import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.0

ApplicationWindow {
    id: window
    width: 250 * num_editors
    height: 450
    minimumWidth: 220 * num_editors
    minimumHeight: 450
    visible: true
    title: qsTr("MC4xxx V0.1")

    property int num_editors: 3

    header: ToolBar {

        RowLayout {
            anchors.fill: parent

            Row {
                Layout.alignment: Qt.AlignLeft
                spacing: 10

                ToolButton {
                    text: qsTr("Save")

                }

                ToolButton {
                    text: qsTr("Open")
                }
            }

            ToolButton {
                text: qsTr("Upload")
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    ListModel {
          id: list
          ListElement { number: 1 }
          ListElement { number: 2 }
      }

    Row {
          anchors.fill: parent
          property int editor_num: 1

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

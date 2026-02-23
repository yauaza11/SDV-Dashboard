import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

Item {
    id : root
    width : 320
    height : 320

    property real rpmValue: 0
    property real maxRpm: 170

    // 중앙 좌표와 동적 반지름 설정 (사이즈 변경 시 자동 대응)
    readonly property real centerX: width / 2
    readonly property real centerY: height / 2
    readonly property real baseRadius: width * 0.42 // 약 135 정도


    Rectangle{
        anchors.fill: parent
        radius: width / 2
        color: "transparent"
        border.color: "#333333"
        border.width: 1
    }

    Repeater{
        model: 21
        delegate: Rectangle{
            width: index%5 === 0 ? 3:1
            height: index%5 === 0 ?15 : 8
            color: index > 16 ? "#ff4444" : "#555555"
            x: root.centerX - width/2
            y: root.centerY - (root.baseRadius + 15)
            antialiasing: true
            transformOrigin: Item.Bottom
            transform: Rotation{
                origin.x: width/2
                origin.y: root.baseRadius + 15 // 반지름 만큼 아래를 축으로 회전?
                angle: 140 + (index*(260/20))
            }
        }
    }

    Shape{
        anchors.fill : parent
        layer.enabled: true
        layer.samples: 8

        ShapePath{
            fillColor: "Transparent"
            strokeColor: "#222222"
            strokeWidth: 12
            capStyle: ShapePath.RoundCap
            PathAngleArc{
                centerX: root.centerX; centerY: root.centerY
                radiusX: root.baseRadius; radiusY: root.baseRadius
                startAngle: 140; sweepAngle: 260
            }
        }

        ShapePath{
            id: progressPath
            fillColor: "transparent"
            strokeWidth: 14
            capStyle: ShapePath.RoundCap

            strokeColor:{
                if(rpmValue<maxRpm*0.6) return "#00d2ff"
                if(rpmValue<maxRpm*0.8) return "#ff9500"
                return "#ff4444"
            }
            PathAngleArc{
                centerX:root.centerX; centerY: root.centerY
                radiusX:root.baseRadius; radiusY: root.baseRadius
                startAngle: 140
                sweepAngle: (rpmValue/maxRpm)*260
            }
        }
    }

    Column{
        anchors.centerIn: parent
        Text{
            text: Math.round(rpmValue)
            font.pixelSize: 90
            font.family: "Arial"
            font.bold: true
            color:"white"
            style: Text.Outline; styleColor: "#222222"
            anchors.horizontalCenter: parent.horizontalCenter

        }
        Text{
            text: "RPM"
            font.pixelSize: 22
            color: "#888888"
            font.letterSpacing: 2
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Behavior on rpmValue{
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutCubic
        }
    }
}

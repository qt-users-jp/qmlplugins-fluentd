/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 2.0
import QtFluentd 0.1

Rectangle {
    width: 360
    height: 360

    Fluentd {
        id: fluentd
    }

    Component.onCompleted: {
        if (fluentd.send('debug.QtFluentd 0.1', {'event': 'Component.onCompleted'}))
            timer.start()
    }

    Text {
        id: state
        text: 'Fluentd.%1'.arg(state.state)
        states: [
            State {
                name: "UnconnectedState"
                when: fluentd.connectionState === Fluentd.UnconnectedState
            },
            State {
                name: "HostLookupState"
                when: fluentd.connectionState === Fluentd.HostLookupState
            },
            State {
                name: "ConnectingState"
                when: fluentd.connectionState === Fluentd.ConnectingState
            },
            State {
                name: "ConnectedState"
                when: fluentd.connectionState === Fluentd.ConnectedState
            },
            State {
                name: "ClosingState"
                when: fluentd.connectionState === Fluentd.ClosingState
            }
        ]
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: qsTr('Click to send a message')

        states: State {
            when: timer.running
            PropertyChanges {
                target: label
                text: qsTr('Sent a message')
            }
        }
    }

    Timer {
        id: timer
        repeat: false
        interval: 1000
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (fluentd.send('debug.QtFluentd 0.1', {'event': 'MouseArea.onClicked'}))
                    timer.start()
        }
    }
}

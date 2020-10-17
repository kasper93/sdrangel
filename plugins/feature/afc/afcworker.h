///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2020 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_FEATURE_AFCWORKER_H_
#define INCLUDE_FEATURE_AFCWORKER_H_

#include <QObject>
#include <QTimer>

#include "util/message.h"
#include "util/messagequeue.h"

#include "afcsettings.h"

class WebAPIAdapterInterface;

class AFCWorker : public QObject
{
    Q_OBJECT
public:
    class MsgConfigureAFCWorker : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        const AFCSettings& getSettings() const { return m_settings; }
        bool getForce() const { return m_force; }

        static MsgConfigureAFCWorker* create(const AFCSettings& settings, bool force)
        {
            return new MsgConfigureAFCWorker(settings, force);
        }

    private:
        AFCSettings m_settings;
        bool m_force;

        MsgConfigureAFCWorker(const AFCSettings& settings, bool force) :
            Message(),
            m_settings(settings),
            m_force(force)
        { }
    };

    class MsgPTT : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        bool getTx() const { return m_tx; }

        static MsgPTT* create(bool tx) {
            return new MsgPTT(tx);
        }

    private:
        bool m_tx;

        MsgPTT(bool tx) :
            Message(),
            m_tx(tx)
        { }
    };

    AFCWorker(WebAPIAdapterInterface *webAPIAdapterInterface);
    ~AFCWorker();
    void reset();
    bool startWork();
    void stopWork();
    bool isRunning() const { return m_running; }
    MessageQueue *getInputMessageQueue() { return &m_inputMessageQueue; }
    void setMessageQueueToGUI(MessageQueue *messageQueue) { m_msgQueueToGUI = messageQueue; }

private:
    WebAPIAdapterInterface *m_webAPIAdapterInterface;
	MessageQueue m_inputMessageQueue; //!< Queue for asynchronous inbound communication
    MessageQueue *m_msgQueueToGUI; //!< Queue to report state to GUI
    AFCSettings m_settings;
    bool m_running;
    bool m_tx;
	QTimer m_updateTimer;
    QMutex m_mutex;

    bool handleMessage(const Message& cmd);
    void applySettings(const AFCSettings& settings, bool force = false);

private slots:
    void handleInputMessages();
};

#endif // INCLUDE_FEATURE_SIMPLEPTTWORKER_H_

#pragma once

#include <QObject>

#include <atomic>
#include <thread>

class StoppableTask : public QObject {
    Q_OBJECT
signals:
    void afterRunSignal();
private:
    std::atomic<bool> stopFlag;
    std::thread taskThread;
public:
    StoppableTask() : stopFlag(false) {
        QObject::connect(this, &StoppableTask::afterRunSignal, this, &StoppableTask::afterRun, Qt::QueuedConnection);
    }

    virtual void beforeRun() {}
    virtual void run() = 0;

    void operator()() {
        beforeRun();
        std::thread taskThread([&]() {
            try {
                run();
            }  catch (...) {}
        });
        taskThread.detach();
        emit afterRunSignal();
    }

    bool isStopped() {
        return stopFlag.load();
    }

    void stopTask() {
        stopFlag.store(true);
    }
public slots:
    void stopTaskSlot() {
        stopTask();
    }

    virtual void afterRun() {}
};

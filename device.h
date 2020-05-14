#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QObject>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>

class Device: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ getName NOTIFY deviceChanged)
    Q_PROPERTY(bool isPaired READ isPaired WRITE setIsPaired NOTIFY deviceChanged)
    Q_PROPERTY(QBluetoothAddress deviceAddress READ getAddress NOTIFY deviceChanged)

public:
    Device(const QBluetoothDeviceInfo &device);

    bool isPaired() const;
    void setIsPaired(const bool &paired);

    QString getName() const;
    QBluetoothAddress getAddress() const;

signals:
    void deviceChanged();

private:
    QBluetoothDeviceInfo mDevice;
    bool mIsDevicePaired;
};

#endif // DEVICE_H

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>

class Device;

class Bluetooth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool scanning READ scanning NOTIFY scanningChanged)
    Q_PROPERTY(QVariant devices READ devices NOTIFY devicesChanged)

public:
    explicit Bluetooth(QObject *parent = nullptr);
    ~Bluetooth();

    bool scanning() const;
    QVariant devices();

    void scanForDevices(const QBluetoothUuid &uuid);

public Q_SLOTS:
    void refresh();
    void startScanning();
    void pairDevice(const QBluetoothAddress &deviceAddress);

private slots:
    void addScannedDevice(const QBluetoothDeviceInfo &deviceInfo);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();
    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status);
    void pairingError(QBluetoothLocalDevice::Error error);

signals:
    void devicesChanged();
    void scanningChanged();

private:
    QList<QObject*> mDevices;
    QBluetoothLocalDevice *mbtLocalDevice;
    QBluetoothDeviceDiscoveryAgent *mbtDeviceAgent;
    Device *mCurrentDevice = nullptr;
};

#endif // BLUETOOTH_H

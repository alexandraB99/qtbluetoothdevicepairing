#include "bluetooth.h"
#include "device.h"

Bluetooth::Bluetooth(QObject *parent)
    : QObject(parent), mbtLocalDevice(new QBluetoothLocalDevice)
{
    mbtDeviceAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(mbtDeviceAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Bluetooth::addScannedDevice);
    connect(mbtDeviceAgent, static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
            this, &Bluetooth::scanError);

    connect(mbtDeviceAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Bluetooth::scanFinished);
    connect(mbtDeviceAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &Bluetooth::scanFinished);

    connect(mbtLocalDevice, &QBluetoothLocalDevice::pairingFinished, this, &Bluetooth::pairingFinished);
    connect(mbtLocalDevice, &QBluetoothLocalDevice::error, this, &Bluetooth::pairingError);
}

Bluetooth::~Bluetooth()
{
    qDeleteAll(mDevices);
    delete mbtDeviceAgent;
    delete mbtLocalDevice;
}

void Bluetooth::startScanning()
{
    qDeleteAll(mDevices);
    mDevices.clear();

    emit devicesChanged();
    mbtDeviceAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
    emit scanningChanged();
    qDebug() << "Scanning for devices...";
}

void Bluetooth::addScannedDevice(const QBluetoothDeviceInfo &device)
{
    if (device.coreConfigurations()) {
        mDevices.append(new Device(device));
        emit devicesChanged();
    }
}

void Bluetooth::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qDebug() << "Please turn on the Bluetooth adaptor.";
    else
        qDebug() << "An unknown error has occurred.";
}

void Bluetooth::scanFinished()
{
    if (mDevices.isEmpty())
        qDebug() << "No devices found.";
    else
        qDebug() << "Scanning done.";

    emit scanningChanged();
    emit devicesChanged();
}


bool Bluetooth::scanning() const
{
    return mbtDeviceAgent->isActive();
}

void Bluetooth::refresh()
{
    startScanning();
}

void Bluetooth::pairDevice(const QBluetoothAddress &address)
{
    for (QObject *entry : qAsConst(mDevices)) {
        auto device = qobject_cast<Device *>(entry);
        if (device && device->getAddress() == address ) {
            mCurrentDevice = device;
            if (mCurrentDevice->isPaired()) {
                mbtLocalDevice->requestPairing(mCurrentDevice->getAddress(), QBluetoothLocalDevice::Unpaired);
            } else {
                mbtLocalDevice->requestPairing(mCurrentDevice->getAddress(), QBluetoothLocalDevice::Paired);
            }
            break;
        }
    }
}

void Bluetooth::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status)
{
    if (status == QBluetoothLocalDevice::Unpaired)
    {
        mCurrentDevice->setIsPaired(false);
    } else {
        mCurrentDevice->setIsPaired(true);
    }
    emit devicesChanged();
}

void Bluetooth::pairingError(QBluetoothLocalDevice::Error error)
{
    if (error != QBluetoothLocalDevice::PairingError)
        return;

    qDebug() << "Error while trying to pair device: " << error;

    pairingFinished(mCurrentDevice->getAddress(), QBluetoothLocalDevice::Unpaired);
}

QVariant Bluetooth::devices()
{
    return QVariant::fromValue(mDevices);
}

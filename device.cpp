#include "device.h"
#include <QDebug>

Device::Device(const QBluetoothDeviceInfo &info):
    mDevice(info)
{
    mIsDevicePaired = false;
}

QString Device::getName() const
{
    return mDevice.name();
}

QBluetoothAddress Device::getAddress() const
{
    return mDevice.address();
}

bool Device::isPaired() const
{
    return mIsDevicePaired;

}
void Device::setIsPaired(const bool &paired)
{
    if (paired != mIsDevicePaired) {
        mIsDevicePaired = paired;
    }
}


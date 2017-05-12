#ifndef EZGRAVER_H
#define EZGRAVER_H

#include "ezgravercore_global.h"

#include <QStringList>
#include <QImage>
#include <QSerialPort>
#include <QSize>

#include <memory>

namespace Ez {
/*!
 * Allows accessing a NEJE engraver using the serial port it was instantiated with.
 * The connection is closed as soon as the object is destroyed.
 */
struct EZGRAVERCORESHARED_EXPORT EzGraver {

    /*! The time required to erase the EEPROM in milliseconds. */
    static int const EraseTimeMs{6000};

    /*! The image width */
    static int const ImageWidth{512};

    /*! The image height */
    static int const ImageHeight{512};

    /*!
     * Gets a list of all available ports.
     *
     * \return A list with all ports.
     */
    static QStringList availablePorts();

    /*!
     * Starts the engraving process with the given \a burnTime.
     *
     * \param burnTime The burn time to use in milliseconds.
     */
    void start(unsigned char const& burnTime);

    /*!
     * Pauses the engraving process at the given location. The process
     * can be continued by invoking start.
     */
    void pause();

    /*! Resets the engraver. */
    void reset();

    /*! Moves the engraver to the home position. */
    void home();

    /*! Moves the engraver to the center. */
    void center();

    /*! Draws a preview of the currently loaded image. */
    void preview();

    /*! Moves the engraver up. */
    virtual void up() = 0;

    /*! Moves the engraver down. */
    virtual void down() = 0;

    /*! Moves the engraver left. */
    virtual void left() = 0;

    /*! Moves the engraver right. */
    virtual void right() = 0;

    /*!
     * Erases the EEPROM of the engraver. This is necessary before uploading
     * any new image to it.
     * Erasing the EEPROM takes a while. Sending image data to early causes
     * that some of the leading pixels are lost. Waiting for about 5 seconds
     * seems to be sufficient.
     */
    void erase();

    /*!
     * Uploads the given \a image to the EEPROM. It is mandatory to use \a erase()
     * it prior uploading an image. The image will automatically be scaled, inverted,
     * mirrored and converted to a monochrome bitmap.
     *
     * \param image The image to upload to the EEPROM for engraving.
     * \return The number of bytes being sent to the device.
     */
    int uploadImage(QImage const& image);

    /*!
     * Uploads any given \a image byte array to the EEPROM. It has to be a monochrome
     * bitmap of the dimensions 512x512. Every white pixel is being engraved.
     *
     * \param image The image byte array to upload to the EEPROM.
     * \return The number of bytes being sent to the device.
     */
    int uploadImage(QByteArray const& image);

    /*!
     * Waits until the current serial port buffer is fully written to the device.
     *
     * \param msecs The time in milliseconds to await the transmission to complete.
     */
    void awaitTransmission(int msecs=-1);

    /*!
     * Gets the serialport used by the EzGraver instance.
     *
     * \return The serial port used.
     */
    std::shared_ptr<QSerialPort> serialPort();

    EzGraver() = delete;
    virtual ~EzGraver();

protected:
    explicit EzGraver(std::shared_ptr<QSerialPort> serial);

    void _transmit(unsigned char const& data);
    void _transmit(QByteArray const& data);
    void _transmit(QByteArray const& data, int chunkSize);

private:
    std::shared_ptr<QSerialPort> _serial;

    void _setBurnTime(unsigned char const& burnTime);
};

}

#endif // EZGRAVER_H

/*
 * Copylight (C) 2012, Shunichi Yamamoto, tkrworks.net
 *
 * This file is part of PIC32UBL_Max.
 *
 * PIC32UBL_Max is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option ) any later version.
 *
 * PICrouter is distributed in the hope that it will be useful,
 * but WITHIOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PICrouter. if not, see <http:/www.gnu.org/licenses/>.
 *
 */

import java.io.IOException;

/**
 * HIDManager.java 
 * High-level interface to enumerate, find , open HID devices and 
 * get connect/disconnect notifications.
 *
 * @version 1.0 
 * @author lord
 * 
 */
public abstract class HIDManager
{
    protected long peer;

    /**
     * Abstract callback method which will be called when HID device is connected.
     *
     * @param dev Reference to the hid device info object.
     */
    //public abstract void deviceAdded( HIDDeviceInfo dev);
    
    /**
     * Abstract callback method which will be called when new HID device is disconnected.
     *
     * @param dev Reference to the hid device info object.
    */
    //public abstract void deviceRemoved( HIDDeviceInfo dev);
     
    /**
     * Get list of all the HID devices attached to the system.
     *
     * @return list of devices
     * @throws IOException
     */
    public static native HIDDeviceInfo[] listDevices() throws IOException;

    /**
     * Initializing the underlying HID layer.
     *
     * @throws IOException
     */
    private native void init() throws IOException;

    /**
     * Release underlying HID layer. This method must be called when
     * <code>HIDManager<code> object is no longer needed. Failure to
     * do so could cause memory leaks or unterminated threads. It is
     * safe to call this method multiple times.
     *
     */
    public native void release();
    
    /**
     * Constructor to create HID object manager. It must be invoked
     * from subclass constructor to ensure proper initialization.
     *
     * @throws IOException
     */
    protected HIDManager() throws IOException
    {
        init();
    }

    /**
     * Release HID manager. Will call release().
     *
     * @throws Throwable
     */
    protected void finalize() throws Throwable
    {
        // It is important to call release() if user forgot to do so,
        // since it frees pointer internal data structures and stops
        // thread under MacOS
        try
        {
           release();
        } finally
        {
           super.finalize();
        }
    }

    /**
     * Convenience method to find and open device by path
     * 
     * @param path USB device path
     * @return open device reference <code>HIDDevice<code> object
     * @throws IOException in case of internal error
     * @throws HIDDeviceNotFoundException if devive was not found
     */
    public static HIDDevice openByPath(String path) throws IOException, HIDDeviceNotFoundException
    {
        HIDDeviceInfo[] devs = listDevices();
        for(HIDDeviceInfo d : devs)
        {
            if(d.getPath().equals(path))
                return d.open();
        }
        throw new HIDDeviceNotFoundException(); 
    }

    /**
     * Convenience method to open a HID device using a Vendor ID
     * (VID), Product ID (PID) and optionally a serial number.
     * 
     * @param vendor_id USB vendor ID
     * @param product_id USB product ID
     * @param serial_number USB device serial number (could be <code>null<code>)
     * @return open device
     * @throws IOException in case of internal error
     * @throws HIDDeviceNotFoundException if devive was not found
     */
    public static HIDDevice openById(int vendor_id, int product_id, String serial_number) throws IOException, HIDDeviceNotFoundException
    {
        HIDDeviceInfo[] devs = listDevices();
        for(HIDDeviceInfo d : devs)
        {
            if(d.getVendor_id() == vendor_id && d.getProduct_id() == product_id
                    && (serial_number == null || d.getSerial_number().equals(serial_number)))
                return d.open();
        }
        throw new HIDDeviceNotFoundException(); 
    }

}

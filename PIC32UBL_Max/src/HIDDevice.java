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
 * Instance of this class represents an open HID device.
 *
 * @author lord
 */
public class HIDDevice
{
    protected long peer;
   
    /**
     * Set peer to object.
     * @param peer a peer value 
     */
    protected HIDDevice(long peer)
    {
        this.peer = peer;
    }
    
    /**
     * Destructor to destroy the <code>HIDDevice</code> object.
     * Calls the close() native method.
     * @throws Throwable
     */
    protected void finalize() throws Throwable
    {
        // It is important to call close() if user forgot to do so,
        // since it frees pointer to internal data structure.
        try
        {
           close();
        } finally
        {
           super.finalize();
        }
    }
    
    /**
     * Method to compare <code>HIDDevice</code> object instances.
     * 
     * @param  obj <code>HIDDevice<code> object reference 
     * @return <code>true</code> if the <code>HIDDevice</code> object represent the same value; <code>false</code> otherwise
     */
    public boolean equals(Object obj) 
    {
        if(!(obj instanceof HIDDevice))
            return false;
        return ((HIDDevice)obj).peer == peer;
    }
   
    /**
     * Returns a hash code for this <code>HIDDevice</code> object.
     * @return a hash code value for this object
     */
    public int hashCode()
    {
       // Same hash code calculation as in Long
        return  (int)(peer^(peer>>>32));
    }

    /**
     * Close open device. Multiple calls allowed - id device was
     * already closed no exception will be thrown.
     * 
     * @throws IOException if error occured opening this device
     */
    public native void close() throws IOException;
   
    /**
     * Write an Output Report to a HID device.
     *
     * @param data the data to send, including the report number as the first byte
     * @return the actual number of bytes written
     * @throws IOException if write error occured
     */
    public native int write(byte[] data) throws IOException;
    
    /**
     * Read an Input Report to a HID device.
     *
     * @param buf a buffer to put the read data into
     * @return the actual number of bytes read 
     * @throws IOException if read error occured
     */
    public native int read(byte[] buf) throws IOException;
    
    /** 
     * Enable blocking reads for this <code>HIDDevice</code> object.
     */
    public native void enableBlocking() throws IOException;
    
    /**
     * Disable blocking reads for this <code>HIDDevice</code> object.
     */
    public native void disableBlocking() throws IOException;

    /**
     * Send a Feature Report to the HID device.
     * @param data The data to send, including the report number as the first byte
     * @return the actual number of bytes written
     * @throws IOException
     */
    public native int sendFeatureReport(byte[] data) throws IOException;
    
    /** 
     * Get a Feature Report from a HID device.
     * @param buf a buffer to put the read data into
     * @return the actual number of bytes read and  -1 on error
     * @throws IOException
     */
    public native int getFeatureReport(byte[] buf) throws IOException;
    
    /**
     * Get The Manufacturer String from a HID device.
     * @return the string buffer to put the data into
     * @throws IOException
     */
    public native String getManufacturerString() throws IOException;

    /**
     * Get The Product String from a HID device.
     * @return the string buffer to put the data into
     * @throws IOException
     */
    public native String getProductString() throws IOException;

    /** 
     * Get The Serial Number String from a HID device.
     * @return the string buffer to put the data into
     * @throws IOException
     */
    public native String getSerialNumberString() throws IOException;

    /**
     * Get a string from a HID device, based on its string index.
     * @return the string buffer to put the data into
     * @throws IOException
     */
    public native String getIndexedString(int string_index) throws IOException;
    
}

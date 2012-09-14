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
 * Sample implementation of HID Manager which just prints
 * notifications about device connection/disconnection on stdout.
 *
 * @author lord
 */
public class HIDManagerTest extends HIDManager
{
    /**
     * Creates a new <code>HIDManagerTest</code> object.
     */
    public HIDManagerTest() throws IOException
    {
       super();
    }
    
    /**
     * Callback method which will be called when HID device is
     * connected.
     *
     * @param dev Reference to the <code>HIDDeviceInfo</code> object.
     * @throws IOException
     */
/*
    public void deviceAdded( HIDDeviceInfo dev )
    {
       System.out.print("Added:" + "\n" + dev + "\n");
    }
*/    
    /**
     * Callback method which will be called when HID device is
     * disconnected.
     *
     * @param dev Reference to the <code>HIDDeviceInfo</code> object.
     * @throws IOException
     */
/*
    public void deviceRemoved( HIDDeviceInfo dev)
    {
        System.out.print("Removal:" + "\n" + dev + "\n");
    }
*/
}
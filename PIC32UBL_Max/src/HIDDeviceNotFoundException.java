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
 * Thrown if HID Device with given criteria could not be found
 *
 * @author lord
 */
public class HIDDeviceNotFoundException extends IOException
{
    /**
     * Constructs a <code>HIDDeviceNotFoundException</code> with no detailed error message.
     */
    public HIDDeviceNotFoundException()
    {
    }
    
    /**
     * Constructs a <code>HIDDeviceNotFoundException</code> with the specified error message.
     */
    public HIDDeviceNotFoundException(String message)
    {
        super(message);
    }
}
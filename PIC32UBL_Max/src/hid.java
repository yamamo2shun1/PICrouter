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
 * hid.java,v.0.9 2012/09/14
 */

import java.io.*;
import com.cycling74.max.*;

public class hid extends MaxObject implements Runnable
{
    private static final long READ_UPDATE_DELAY_MS = 50L;

    static
    {
        System.loadLibrary("hidapi-jni");
    }

    // Default VID/PID is PICrouter and Bootloader ID.
    private static final int MICROCHIP_VID      = 0x04D8;
    private static final int HID_BOOTLOADER_PID = 0x003C;
    private static final int PICROUTER_PID      = 0xF81C;
    private static final int BUFSIZE = 2048;
    private static final int MAXOBJECTS = 100;

    Thread thread;

    int vendorId, productId;
    
    HIDDevice dev;
    HIDManager hid_mgr;

    boolean threadFlag = false;
    long interval = 10;

    byte[] inBuffer = new byte[BUFSIZE];
    byte[] prevBuffer = new byte[BUFSIZE];

    FileReader fr;
    char[] crc_table = new char[16];
    int[] hexFile;
    int hexLength, hexLength0, percentage0, percentage1;
    boolean pgm_completed_flag = false;
    boolean close_flag = false;

    public hid(int inlets, int outlets)
    {
        crc_table[0] = 0x0000;
        crc_table[1] = 0x1021;
        crc_table[2] = 0x2042;
        crc_table[3] = 0x3063;
        crc_table[4] = 0x4084;
        crc_table[5] = 0x50a5;
        crc_table[6] = 0x60c6;
        crc_table[7] = 0x70e7;
        crc_table[8] = 0x8108;
        crc_table[9] = 0x9129;
        crc_table[10] = 0xa14a;
        crc_table[11] = 0xb16b;
        crc_table[12] = 0xc18c;
        crc_table[13] = 0xd1ad;
        crc_table[14] = 0xe1ce;
        crc_table[15] = 0xf1ef;

        thread = new Thread(this);
        thread.start();

        declareIO(inlets, outlets);
        declareAttribute("id", null, "setIDs");
        declareAttribute("vid", null, "setVendorID");
        declareAttribute("pid", null, "setProductID");
        declareAttribute("interval", null, "setPollingInterval");
        declareAttribute("openBL", null, "openBootloader");
        declareAttribute("open", null, "openDevice");
        declareAttribute("close", null, "closeDevice");
        declareAttribute("write", null, "writeToDevice");
        declareAttribute("port", null, "setPort");
    }

    public char calculateCrc(byte[] data, int len)
    {
        int i, j = 0;
        char crc = 0;

        while(len != 0)
        {
            i = (crc >> 12) ^ (data[j] >> 4);
            crc = (char)(crc_table[i & 0x0F] ^ (crc << 4));
            i = (crc >> 12) ^ (data[j] >> 0);
            crc = (char)(crc_table[i & 0x0F] ^ (crc << 4));
            j++;
            len--;
        }
        return (char)(crc & 0xFFFF);
    }

    public void inlet(int input)
    {
        outlet(0, input);
    }

    protected void setIDs(Atom[] args)
    {
        vendorId = args[0].toInt();
        productId = args[1].toInt();
    }

    protected void setVendorID(Atom[] args)
    {
        vendorId = args[0].toInt();
    }

    protected void setProductID(Atom[] args)
    {
        productId = args[0].toInt();
    }

    protected void openBootloader()
    {
        try {
            hid_mgr = new HIDManagerTest();
            dev = hid_mgr.openById(MICROCHIP_VID, HID_BOOTLOADER_PID, null);
            dev.enableBlocking();
            threadFlag = true;
            outlet(1, "Succeeded to open HID Bootloader(AN1388)!");
        } catch(IOException ioe) {
            outlet(1, "Not connected HID Bootloader(AN1388)!");
        }
    }

    protected void openDevice()
    {
        try {
            hid_mgr = new HIDManagerTest();
            dev = hid_mgr.openById(MICROCHIP_VID, PICROUTER_PID, null);
            dev.enableBlocking();
            threadFlag = true;
            outlet(1, "Succeeded to open PICrouter!");
        } catch(IOException ioe) {
            outlet(1, "Not connected PICrouter!");
        }
    }

    protected void closeDevice() throws IOException
    {
        threadFlag = false;
        close_flag = true;
        Atom[] atom = new Atom[1];
        atom[0] = Atom.newAtom(0x01);
        writeToDevice(atom);
        dev.disableBlocking();
        dev.close();
        hid_mgr.release();
        System.gc();
        //outlet(1, "Disconnected.");
    }

    protected void writeToDevice(Atom[] args) throws IOException
    {
        int idx = 0, ch, l;
        byte[] outBuffer = new byte[64];
        int[] chs;
        outBuffer[0] = (byte)args[0].toInt();
        switch(outBuffer[0])
        {
            case (byte)0x00:
                fr = new FileReader(args[1].toString());
                l = 0;
                chs = new int[2];
                hexLength = 0;
                while((ch = fr.read()) != -1)
                {
                    if(ch != 0x3a && ch != 0x0a)
                    {
                        l++;
                        if(l == 2)
                        {
                            hexLength++;
                            l = 0;
                        }
                    }
                }
                fr = new FileReader(args[1].toString());
                outlet(1, "Load Hex file Successfully.");
                hexLength0 = hexLength;
                pgm_completed_flag = false;
                //outlet(0, hexLength);
                break;
            case (byte)0x01: // READ_BOOT_INFO
            case (byte)0x02: // ERASE_FLASH
            case (byte)0x03: // PROGRAM_FLASH
            case (byte)0x04: // VERIFY
            case (byte)0x05: // JUMP_TO_APP
                byte SOH = 1;
                byte EOT = 4;
                byte DLE = 16;
                int bufflen = 0;
                int txpacketlen = 0;
                byte[] buff = new byte[1000];
                byte[] txpacket = new byte[1000];
                buff[bufflen++] = (byte)outBuffer[0];

                if(outBuffer[0] == (byte)0x03) // PROGRAM_FLASH
                {
                    l = 0;
                    chs = new int[2];
                    int totalRecords = 10;
                    bufflen = 0;
                    buff[bufflen++] = (byte)outBuffer[0];
                    while((ch = fr.read()) != -1)
                    {
                        if(ch != 0x3a && ch != 0x0a)
                        {
                            chs[l] = ch;
                            l++;
                            if(l == 2)
                            {
                                buff[bufflen] = 0;
                                for(int i = 0; i < 2; i++)
                                {
                                    if(chs[i] >= 0x30 && chs[i] <= 0x39)
                                    {
                                        if(i == 0)
                                            buff[bufflen] += (chs[i] - 0x30) << 4;
                                        else
                                            buff[bufflen] += (chs[i] - 0x30);
                                    }
                                    else if(chs[i] >= 0x61 && chs[i] <= 0x66)
                                    {
                                        if(i == 0)
                                            buff[bufflen] += (0x0a + chs[i] - 0x61) << 4;
                                        else
                                            buff[bufflen] += (0x0a + chs[i] - 0x61);
                                    }
                                }
                                bufflen++;
                                l = 0;
                            }
                        }
                        if(ch == 0x0a)
                            totalRecords--;
                        if(totalRecords == 0)
                        {
                            char crc = calculateCrc(buff, bufflen);
                            buff[bufflen++] = (byte)crc;
                            buff[bufflen++] = (byte)(crc >> 8);
                            hexLength0 -= bufflen;
                            int percentage = (int)(100.0 - (((double)hexLength0 / (double)hexLength) * 100.0));
                            if(percentage > 100)
                                percentage = 100;
                            if(percentage != percentage0)
                                outlet(2,  percentage);
                            percentage0 = percentage;

                            txpacketlen = 0;
                            txpacket[txpacketlen++] = SOH;
                            for(int i = 0; i < bufflen; i++)
                            {
                                if((buff[i] == EOT) || (buff[i] == SOH) || buff[i] == DLE)
                                {
                                    txpacket[txpacketlen++] = DLE;
                                }
                                txpacket[txpacketlen++] = buff[i];
                            }
                            txpacket[txpacketlen++] = EOT;
                            dev.write(txpacket);

                            bufflen = 0;
                            totalRecords = 10;
                            break;
                        }
                    }
                    if(ch == -1)
                    {
                        if(!pgm_completed_flag)
                        {
                            outlet(1, "Program Completed.");
                            pgm_completed_flag = true;
                        }
                        break;
                    }
                }
                if(outBuffer[0] == (byte)0x04) // PROGRAM_VERIFY
                {
                }

                char crc = calculateCrc(buff, 1);
                buff[bufflen++] = (byte)crc;
                buff[bufflen++] = (byte)(crc >> 8);

                txpacket[txpacketlen++] = SOH;
                for(int i = 0; i < bufflen; i++)
                {
                    if((buff[i] == EOT) || (buff[i] == SOH) || buff[i] == DLE)
                    {
                        txpacket[txpacketlen++] = DLE;
                    }
                    txpacket[txpacketlen++] = buff[i];
                }
                txpacket[txpacketlen++] = EOT;
                dev.write(txpacket);
                if(outBuffer[0] == (byte)0x05)
                    closeDevice();
                break;
            case (byte)0xF1:
            case (byte)0xF2:
                if(outBuffer[1] == 1)
                {
                    int index = -1;
                    String inStr = args[2].toString();

                    for(int i = 2; i < 6; i++)
                    {
                        inStr = inStr.substring(index + 1, inStr.length());
                        index = inStr.indexOf(".");
                        if(index == -1)
                            outBuffer[i] = Integer.valueOf(inStr).byteValue();
                        else
                            outBuffer[i] = Integer.valueOf(inStr.substring(0, index)).byteValue();
                    }
                }
                dev.write(outBuffer);
                break;
            case (byte)0xF3:
            case (byte)0xF4:
                if(outBuffer[1] == 1)
                {
                    outBuffer[2] =  (byte)(args[2].toInt() >> 0);
                    outBuffer[3] =  (byte)(args[2].toInt() >> 8);
                }
                dev.write(outBuffer);
                break;
            case (byte)0xF6:
                dev.write(outBuffer);
                break;
            default:
                break;
        }
    }

    protected void setPollingInterval(Atom[] args)
    {
        interval = args[0].toLong();
        outlet(0, "Changed interval time(msec)");
    }

    protected void setLocalPort(Atom[] args)
    {

    }

    protected void setPort(Atom[] args)
    {
        outlet(1, args[0].toInt());
    }

    public void run()
    {
        try
        {
            try
            {
                while(true)
                {
                    if(threadFlag)
                    {
                        int[] outs = new int[8];
                        String[] str = new String[2];
                        int n = dev.read(inBuffer);

                        switch(inBuffer[0])
                        {
                            case (byte)0xF1:
                                for(int i = 0; i < outs.length; i++)
                                {
                                    if(inBuffer[i + 1] < 0)
                                        outs[i] = inBuffer[i + 1] + 256;
                                    else
                                        outs[i] = inBuffer[i + 1];
                                }
                                str[0] = "localIP";
                                str[1] = new String(outs[0] + "." + outs[1] + "." + outs[2] + "." + outs[3]);
                                outlet(0,str);
                                break;
                            case (byte)0xF2:
                                for(int i = 0; i < outs.length; i++)
                                {
                                    if(inBuffer[i + 1] < 0)
                                        outs[i] = inBuffer[i + 1] + 256;
                                    else
                                        outs[i] = inBuffer[i + 1];
                                }
                                str[0] = "remoteIP";
                                str[1] = new String(outs[0] + "." + outs[1] + "." + outs[2] + "." + outs[3]);
                                outlet(0, str);
                                break;
                            case (byte)0xF3:
                                str[0] = "localPort";
                                if(inBuffer[1] < 0)
                                    outs[0] = inBuffer[1] + 256;
                                else 
                                    outs[0] = inBuffer[1];
                                if(inBuffer[2] < 0)
                                    outs[1] = inBuffer[2] + 256;
                                else 
                                    outs[1] = inBuffer[2];
                                str[1] = String.valueOf(outs[0] | (outs[1] << 8));
                                outlet(0, str);
                                break;
                            case (byte)0xF4:
                                str[0] = "remotePort";
                                if(inBuffer[1] < 0)
                                    outs[0] = inBuffer[1] + 256;
                                else
                                    outs[0] = inBuffer[1];
                                if(inBuffer[2] < 0)
                                    outs[1] = inBuffer[2] + 256;
                                else 
                                    outs[1] = inBuffer[2];
                                str[1] = String.valueOf(outs[0] | (outs[1] << 8));
                                outlet(0, str);
                                break;
                            case (byte)0xF6:
                                for(int i = 0; i < outs.length; i++)
                                {
                                    if(inBuffer[i + 1] < 0)
                                        outs[i] = inBuffer[i + 1] + 256;
                                    else
                                        outs[i] = inBuffer[i + 1];
                                }
                                str[0] = "remoteIP";
                                str[1] = new String(outs[0] + "." + outs[1] + "." + outs[2] + "." + outs[3]);
                                outlet(0, str);

                                str[0] = "localPort";
                                str[1] = String.valueOf(outs[4] | (outs[5] << 8));
                                outlet(0, str);

                                str[0] = "remotePort";
                                str[1] = String.valueOf(outs[6] | (outs[7] << 8));
                                outlet(0, str);
                                break;
                            default:
                                if(inBuffer[1] == 16)
                                {
                                    if(close_flag)
                                    {
                                        outlet(1, "Disconnected.");
                                        close_flag = false;
                                    }
                                    else
                                    {
                                        str[0] = "Bootloader Ver.";
                                        if(inBuffer[3]  != 16)
                                            str[1] = new String(inBuffer[3] + "." + inBuffer[4]);
                                        else
                                            str[1] = new String(inBuffer[4] + "." + inBuffer[5]);
                                        outlet(1, str);
                                    }
                                }
                                else if(inBuffer[1] == 2)
                                {
                                    outlet(1, "Program Erased");
                                }
                                else if(inBuffer[1] == 3)
                                {
                                    Atom[] atom = new Atom[1];
                                    atom[0] = Atom.newAtom(3);
                                    writeToDevice(atom);
                                }
                                break;
                        }
/*
                        if(inBuffer[1] == prevBuffer[1])
                            continue;
                        prevBuffer[1] = inBuffer[1];
*/
                        if(inBuffer[0] == (byte)0x81)
                        {
                            outlet(0, 1 - inBuffer[1]);
                        }
                    }
                    try
                    {
                        Thread.sleep(interval);
                    }
                    catch(InterruptedException ire)
                    {}
                }
            }
            finally
            {
                threadFlag = false;
                close_flag = true;
                dev.disableBlocking();
                dev.close();
                //hid_mgr.release();
                //System.gc();

                //threadFlag = false;
                outlet(1, "Disconnected.");
                //dev.close();
                //hid_mgr.release();
                //System.gc();
            }
        }
        catch(IOException ioe)
        {}
    }
}

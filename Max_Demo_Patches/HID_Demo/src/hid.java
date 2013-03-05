import java.io.*;
import com.cycling74.max.*;

public class hid extends MaxObject
{
    static
    {
        System.loadLibrary("hidapi-jni");
    }

    // Default VID/PID is PICrouter and Bootloader ID.
    private static final int MICROCHIP_VID      = 0x04D8;
    private static final int PICROUTER_PID      = 0xF81C;
    private static final int BUFSIZE = 2048;
    private static final int MAXOBJECTS = 100;

    int vendorId, productId;

    HIDDevice dev;
    HIDManager hid_mgr;

    long interval = 10;

    byte[] inBuffer = new byte[BUFSIZE];
    byte[] prevBuffer = new byte[BUFSIZE];

    public hid(int inlets, int outlets)
    {
        declareIO(inlets, outlets);
        declareAttribute("id", null, "setIDs");
        declareAttribute("vid", null, "setVendorID");
        declareAttribute("pid", null, "setProductID");
        declareAttribute("interval", null, "setPollingInterval");
        declareAttribute("open", null, "openDevice");
        declareAttribute("write", null, "writeToDevice");
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

    protected void openDevice()
    {
        try {
            hid_mgr = HIDManager.getInstance();
            dev = hid_mgr.openById(MICROCHIP_VID, PICROUTER_PID, null);
            outlet(1, "Succeeded to open PICrouter!");
        } catch(IOException ioe) {
            outlet(1, "Not connected PICrouter!");
        }
    }

    protected void writeToDevice(Atom[] args) throws IOException
    {
        byte[] outBuffer = new byte[64];
        int[] chs;
        outBuffer[0] = (byte)args[0].toInt();
        switch(outBuffer[0])
        {
/*
            case (byte)0x83:
                break;
            case (byte)0x84:
                break;
            case (byte)0x85:
                break;
            case (byte)0x86:
                break;
            case (byte)0x87:
                break;
 */
            case (byte)0xF0: // Host IP
                dev.write(outBuffer);
                readInputBuffer();
                break;
            case (byte)0xF1: // Remote IP
                if((outBuffer[1] = (byte)args[1].toInt()) == 1)
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
                if(outBuffer[1] == 0)
                    readInputBuffer();
                break;
            case (byte)0xF2: // Host Port
            case (byte)0xF3: // Remote Port
                if((outBuffer[1] = (byte)args[1].toInt()) == 1)
                {
                    outBuffer[2] =  (byte)(args[2].toInt() >> 0);
                    outBuffer[3] =  (byte)(args[2].toInt() >> 8);
                }
                dev.write(outBuffer);
                if(outBuffer[1] == 0)
                    readInputBuffer();
                break;
            case (byte)0xF4: // Mac Address
                if(args.length > 1)
                {
                    outBuffer[1] = (byte)args[1].toInt();
                    dev.write(outBuffer);
                    readInputBuffer();
                }
                break;
            case (byte)0xF5: // Soft Reset
                if(args.length > 1)
                {
                    outlet(1, "Disconnected");
                    outBuffer[1] = (byte)args[1].toInt();
                    dev.write(outBuffer);
                    //dev.close();
                    //hid_mgr.release();
                    //System.gc();
                }
                break;
/*
            case (byte)0xF6:
                dev.write(outBuffer);
                break;
 */
            default:
                for(int i = 1; i < args.length; i++)
                {
                    outBuffer[i] = (byte)args[i].toInt();
                }
                dev.write(outBuffer);
                if(args.length > 1 && outBuffer[1] == 0)
                    readInputBuffer();
                break;
        }
    }

    protected void setPollingInterval(Atom[] args)
    {
        interval = args[0].toLong();
        outlet(0, "Changed interval time(msec)");
    }

    protected void readInputBuffer() throws IOException
    {
        int[] outs = new int[8];
        String[] str = new String[2];

        outlet(3, "read...");
        dev.enableBlocking();
        int n = dev.read(inBuffer);
        dev.disableBlocking();
        outlet(3, "finish...");

        switch(inBuffer[0])
        {
            case (byte)0xF0:
                for(int i = 0; i < outs.length; i++)
                {
                    if(inBuffer[i + 2] < 0)
                        outs[i] = inBuffer[i + 2] + 256;
                    else
                        outs[i] = inBuffer[i + 2];
                }
                str[0] = "hostIP";
                str[1] = new String(outs[0] + "." + outs[1] + "." + outs[2] + "." + outs[3]);
                outlet(0, str);
                break;
            case (byte)0xF1:
                for(int i = 0; i < outs.length; i++)
                {
                    if(inBuffer[i + 2] < 0)
                        outs[i] = inBuffer[i + 2] + 256;
                    else
                        outs[i] = inBuffer[i + 2];
                }
                str[0] = "remoteIP";
                str[1] = new String(outs[0] + "." + outs[1] + "." + outs[2] + "." + outs[3]);
                outlet(0, str);
                break;
            case (byte)0xF2:
                str[0] = "hostPort";
                if(inBuffer[2] < 0)
                    outs[0] = inBuffer[2] + 256;
                else 
                    outs[0] = inBuffer[2];
                if(inBuffer[3] < 0)
                    outs[1] = inBuffer[3] + 256;
                else 
                    outs[1] = inBuffer[3];
                str[1] = String.valueOf(outs[0] | (outs[1] << 8));
                outlet(0, str);
                break;
            case (byte)0xF3:
                str[0] = "remotePort";
                if(inBuffer[2] < 0)
                    outs[0] = inBuffer[2] + 256;
                else 
                    outs[0] = inBuffer[2];
                if(inBuffer[3] < 0)
                    outs[1] = inBuffer[3] + 256;
                else 
                    outs[1] = inBuffer[3];
                str[1] = String.valueOf(outs[0] | (outs[1] << 8));
                outlet(0, str);
                break;
            case (byte)0xF4:
                str[0] = "macAddress";
                str[1] = new String(Integer.toHexString(inBuffer[2] & 0xFF) + ":" + 
                                    Integer.toHexString(inBuffer[3] & 0xFF) + ":" + 
                                    Integer.toHexString(inBuffer[4] & 0xFF) + ":" + 
                                    Integer.toHexString(inBuffer[5] & 0xFF) + ":" + 
                                    Integer.toHexString(inBuffer[6] & 0xFF) + ":" + 
                                    Integer.toHexString(inBuffer[7] & 0xFF));
                outlet(0, str);
                break;
/*
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
 */
            default:
                //String[] str0 = new String[inBuffer[1] + 1];
                int[] outlist = new int[inBuffer[1] + 1];
                if(inBuffer[0] < 0)
                    //str0[0] = Integer.toString(inBuffer[0] + 256);
                    outlist[0] = inBuffer[0] + 256;
                else
                    //str0[0] = Integer.toString(inBuffer[0]);
                    outlist[0] = inBuffer[0];
                //for(int i = 0; i < str0.length - 1; i++)
                for(int i = 0; i < outlist.length - 1; i++)
                {
                    if(inBuffer[i + 2] < 0)
                        //str0[i + 1] = Integer.toString(inBuffer[i + 2] + 256);
                        outlist[i + 1] = inBuffer[i + 2] + 256;
                    else
                        //str0[i + 1] = Integer.toString(inBuffer[i + 2]);
                        outlist[i + 1] = inBuffer[i + 2];
                }
                //outlet(0, str0);
                outlet(0, outlist);
                break;
        }
    }
}

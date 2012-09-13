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
    public void deviceAdded( HIDDeviceInfo dev )
    {
       System.out.print("Added:" + "\n" + dev + "\n");
    }
    
    /**
     * Callback method which will be called when HID device is
     * disconnected.
     *
     * @param dev Reference to the <code>HIDDeviceInfo</code> object.
     * @throws IOException
     */
    public void deviceRemoved( HIDDeviceInfo dev)
    {
        System.out.print("Removal:" + "\n" + dev + "\n");
    }
}
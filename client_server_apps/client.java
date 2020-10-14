import java.net.*;
import java.io.*;

public class client{
    public static void main(String args[]) throws Exception {
        Socket c = new Socket("127.0.0.1", 5050);
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        DataInputStream sockIn = new DataInputStream(c.getInputStream());
        DataOutputStream sockOut = new DataOutputStream(c.getOutputStream());

        String sir = reader.readLine();
        int len = sir.length();
        sockOut.writeShort(len);
        sockOut.writeBytes(sir);
        sockOut.flush();
        
        int count;
        count = sockIn.readUnsignedShort();
        System.out.println("Numarul de spatii este: " + count);
        c.close();
    }
}
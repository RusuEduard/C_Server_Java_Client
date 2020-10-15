import java.net.*;
import java.io.*;

public class Client{
    public static void main(String[] args) throws IOException {
        String address = args[0];
        int port = Integer.parseInt(args[1]);
        System.out.println(address);
        System.out.println(port);
        
        Socket c = new Socket(address, port);
        
        int len1, len2;
        String s1, s2;
        
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        DataOutputStream cOut = new DataOutputStream(c.getOutputStream());

        System.out.println("Read the 1st string:");
        s1 = reader.readLine();
        len1 = s1.length();

        cOut.writeShort(len1+1);
        cOut.writeBytes(s1 + '\0');
        cOut.flush();

        System.out.println("Read the second string: ");
        s2 = reader.readLine();
        len2 = s2.length();

        cOut.writeShort(len2 + 1);
        cOut.writeBytes(s2 + '\0');
        cOut.flush();
        byte[] b = new byte[100];
        c.getInputStream().read(b);
        System.out.println("[Client] received: " +new String(b));
        
        c.close();
    }
}
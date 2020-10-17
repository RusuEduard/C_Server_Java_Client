import java.net.*;
import java.io.*;

public class Client{
    public static void main(String[] args) throws IOException {
        Socket c = new Socket("127.0.0.1", 5050);
        
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        DataInputStream sIn = new DataInputStream(c.getInputStream());
        DataOutputStream sOut = new DataOutputStream(c.getOutputStream());

        System.out.println("[Client] Read the string: ");
        String str = reader.readLine();
        str = str + "\0";
        int len = str.length();
        sOut.writeShort(len);
        sOut.writeBytes(str);

        System.out.println("[Client] Read the character: ");
        String chr = reader.readLine();
        sOut.writeBytes(chr);

        int size = sIn.readUnsignedShort();
        System.out.println("[Client] Number of appearances returned form server: " + size);
        System.out.println("Appearances on positions:");
        for (int i = 0; i < size; i++) {
            int nr = sIn.readUnsignedShort();
            System.out.println(nr);
        }
        c.close();
    }
}
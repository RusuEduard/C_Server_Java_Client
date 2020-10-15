import java.net.*;
import java.io.*;

public class Client{
    public static void main(String[] args) throws IOException {
        Socket c = new Socket("127.0.0.1", 5050);

        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        DataOutputStream cOut = new DataOutputStream(c.getOutputStream());
        
        System.out.println("Citeste sirul de numere: (separate prin spatiu)");
        String sir = reader.readLine();

        String nou[] = sir.split(" ");
        
        int count = nou.length;
        System.out.println("[CLIENT] Number of elements in array: " + count);
        cOut.writeShort(count);

        for (int i = 0; i < count; i++) {
            int nr = Integer.parseInt(nou[i]);
            cOut.writeShort(nr);
        }
        c.close();
    }
}
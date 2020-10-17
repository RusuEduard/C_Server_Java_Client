import java.net.*;
import java.io.*;

public class Client{
    public static void main(String[] args) throws IOException {
        Socket c = new Socket("127.0.0.1", 5050);

        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        DataOutputStream cOut = new DataOutputStream(c.getOutputStream());
        DataInputStream cIn = new DataInputStream(c.getInputStream());
        
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

        System.out.println("Citeste al doilea sir de numere: ");
        String sir2 = reader.readLine();

        String nou2[] = sir2.split(" ");

        int count2 = nou2.length;
        System.out.println("[CLIENT] the numbers of elems in the 2nd array: " + count2);
        cOut.writeShort(count2);

        for(int i = 0; i < count2; i++){
            int nr = Integer.parseInt(nou2[i]);
            cOut.writeShort(nr);
        }

        int finalLength = cIn.readUnsignedShort();
        System.out.println("[Client] size of final array is: " + finalLength);
        int finalArray[] = new int[finalLength];
        cIn.readUnsignedShort();
        for(int i = 0; i < finalLength; i++){
            finalArray[i] = cIn.readUnsignedShort();
        } 
        
        System.out.println("Final array: ");
        for (var el : finalArray){
            System.out.println(el);
        }

        c.close();
    }
}
import java.net.*;
import java.io.*;

public class Client{
    public static void main(String[] args) throws IOException {
        String IP = args[0];
        int PORT = Integer.parseInt(args[1]);
        
        Socket s = new Socket(IP, PORT);
        
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        DataInputStream sIn = new DataInputStream(s.getInputStream());
        DataOutputStream sOut = new DataOutputStream(s.getOutputStream());

        System.out.println("Read the number: ");
        int number = Integer.parseInt(reader.readLine());
        sOut.writeShort(number);
        sOut.flush();

        int count;
        count = sIn.readUnsignedShort();
        if(count == 0){
            System.out.println("The number is a prime number!");
        }
        else{
//            int aux = sIn.readUnsignedShort();
            int numbers;
            String divs = "";
            for (int i = 0; i < count; i++) {
                numbers = sIn.readUnsignedShort();
                System.out.println(numbers);
                divs = divs + Integer.toString(numbers) + " ";
            }
            System.out.println("Divisors are " + divs);
        }
        s.close();
    }
}
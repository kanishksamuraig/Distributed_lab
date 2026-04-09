import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class CalculatorClient {
    public static void main(String[] args) {
        try {
            // Connect to the RMI registry on the server (use server's IP if remote)
            Registry registry = LocateRegistry.getRegistry("127.0.0.1", 1099);

            // Look up the service by the name the server used
            Calculator stub = (Calculator) registry.lookup("CalculatorService");

            // Execute the Remote Procedure Call!
            System.out.println("Sending math problem to server...");
            int result = stub.add(15, 27);

            System.out.println("Response from RPC Server: 15 + 27 = " + result);
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
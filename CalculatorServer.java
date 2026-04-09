import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;

public class CalculatorServer implements Calculator {

    // The actual execution happens here on the server
    @Override
    public int add(int a, int b) throws RemoteException {
        System.out.println("Server executing add(" + a + ", " + b + ")");
        return a + b;
    }

    public static void main(String[] args) {
        try {
            CalculatorServer serverObj = new CalculatorServer();

            // Export the object so it can receive incoming network calls
            Calculator stub = (Calculator) UnicastRemoteObject.exportObject(serverObj, 0);

            // Create the RMI registry on port 1099 (the default RMI port)
            Registry registry = LocateRegistry.createRegistry(1099);

            // Bind our service to the registry with a name so the client can find it
            registry.rebind("CalculatorService", stub);

            System.out.println("RPC Server is ready and waiting for calls...");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
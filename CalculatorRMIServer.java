// CalculatorRMIServer.java

import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.*;

public class CalculatorRMIServer extends UnicastRemoteObject implements CalculatorInterface {

    public CalculatorRMIServer() throws RemoteException {
        super();
    }

    @Override
    public int add(int a, int b) throws RemoteException {
        System.out.println("Server: Adding " + a + " + " + b);
        return a + b;
    }

    @Override
    public int subtract(int a, int b) throws RemoteException {
        System.out.println("Server: Subtracting " + a + " - " + b);
        return a - b;
    }

    @Override
    public int multiply(int a, int b) throws RemoteException {
        System.out.println("Server: Multiplying " + a + " * " + b);
        return a * b;
    }

    @Override
    public String getMessage(String name) throws RemoteException {
        System.out.println("Server: Getting message for " + name);
        return "Welcome " + name + " to RMI!";
    }

    public static void main(String[] args) {
        try {
            // Create RMI Registry on port 1099
            Registry registry = LocateRegistry.createRegistry(1099);

            // Create server object
            CalculatorRMIServer server = new CalculatorRMIServer();

            // Bind server object to registry
            registry.rebind("CalculatorService", server);

            System.out.println("RMI Server is running...");
            System.out.println("Waiting for client requests...");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

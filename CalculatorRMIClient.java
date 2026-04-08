// CalculatorRMIClient.java

import java.rmi.registry.*;

public class CalculatorRMIClient {

    public static void main(String[] args) {
        try {
            // Get registry
            Registry registry = LocateRegistry.getRegistry("localhost", 1099);

            // Look up remote object
            CalculatorInterface calculator = (CalculatorInterface) registry.lookup("CalculatorService");

            System.out.println("Connected to RMI Server");

            // Call remote methods
            System.out.println("10 + 20 = " + calculator.add(10, 20));
            System.out.println("50 - 30 = " + calculator.subtract(50, 30));
            System.out.println("7 * 8 = " + calculator.multiply(7, 8));
            System.out.println(calculator.getMessage("Alice"));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

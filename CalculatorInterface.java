// CalculatorInterface.java
import java.rmi.*;

public interface CalculatorInterface extends Remote {
    int add(int a, int b) throws RemoteException;
    int subtract(int a, int b) throws RemoteException;
    int multiply(int a, int b) throws RemoteException;
    String getMessage(String name) throws RemoteException;
}
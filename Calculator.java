import java.rmi.Remote;
import java.rmi.RemoteException;

// The interface must extend Remote
public interface Calculator extends Remote {
    // Every method must throw RemoteException to handle network failures
    int add(int a, int b) throws RemoteException;
}
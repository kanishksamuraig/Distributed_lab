import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Client {
    private static final String SERVER_ADDRESS = "127.0.0.1"; // Use the server's IP if on a different machine
    private static final int SERVER_PORT = 5000;

    public static void main(String[] args) {
        try (
                Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                Scanner scanner = new Scanner(System.in)
        ) {
            System.out.println("Connected to the server at " + SERVER_ADDRESS + ":" + SERVER_PORT);
            System.out.println("Type your message (type 'exit' to quit):");

            // Background Thread: Listen for messages coming from the server
            Thread readThread = new Thread(() -> {
                try {
                    String serverResponse;
                    while ((serverResponse = in.readLine()) != null) {
                        System.out.println(serverResponse);
                    }
                } catch (IOException e) {
                    // This will trigger when the socket is closed upon exiting
                    System.out.println("Disconnected from server.");
                }
            });
            readThread.start();

            // Main Thread: Read from user console and send to server
            String userInput;
            while (true) {
                userInput = scanner.nextLine();
                out.println(userInput);

                if ("exit".equalsIgnoreCase(userInput.trim())) {
                    break;
                }
            }
        } catch (IOException e) {
            System.err.println("Client connection error: " + e.getMessage());
        }
    }
}
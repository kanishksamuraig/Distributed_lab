import java.io.*;
import java.net.*;

public class MultiThreadedServer {
    // Port number the server will listen on
    private static final int PORT = 5000;

    public static void main(String[] args) {
        System.out.println("Server starting on port " + PORT + "...");

        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server is listening. Waiting for clients...");

            // Infinite loop to continuously accept incoming connections
            while (true) {
                Socket clientSocket = serverSocket.accept();
                String clientAddress = clientSocket.getInetAddress().getHostAddress();
                System.out.println("New client connected: " + clientAddress);

                // Spawn a new thread to handle this specific client
                ClientHandler clientHandler = new ClientHandler(clientSocket);
                new Thread(clientHandler).start();
            }
        } catch (IOException e) {
            System.err.println("Server exception: " + e.getMessage());
            e.printStackTrace();
        }
    }
}

// Runnable task that handles communication for a single client
class ClientHandler implements Runnable {
    private final Socket socket;

    public ClientHandler(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        String clientAddress = socket.getInetAddress().getHostAddress();

        try (
                // Setup input and output streams
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true) // true = auto-flush
        ) {
            String inputLine;

            // Continuously read messages from this client
            while ((inputLine = in.readLine()) != null) {
                System.out.println("Received from [" + clientAddress + "]: " + inputLine);

                // Echo the message back to the client
                out.println("Server echo: " + inputLine);

                // Disconnect condition
                if ("exit".equalsIgnoreCase(inputLine.trim())) {
                    System.out.println("Client [" + clientAddress + "] requested to disconnect.");
                    break;
                }
            }
        } catch (IOException e) {
            System.err.println("Client handler exception for [" + clientAddress + "]: " + e.getMessage());
        } finally {
            // Ensure the socket is always closed gracefully to prevent resource leaks
            try {
                if (socket != null && !socket.isClosed()) {
                    socket.close();
                }
            } catch (IOException e) {
                System.err.println("Error closing socket: " + e.getMessage());
            }
            System.out.println("Connection closed for [" + clientAddress + "].");
        }
    }
}
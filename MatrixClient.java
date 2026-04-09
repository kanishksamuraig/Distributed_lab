import java.io.*;
import java.net.*;
import java.util.Scanner;

public class MatrixClient {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        try (Socket socket = new Socket("localhost", 5000);
             ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
             ObjectInputStream in = new ObjectInputStream(socket.getInputStream())) {

            System.out.print("Enter rows and columns for Matrix A: ");
            int r1 = sc.nextInt();
            int c1 = sc.nextInt();
            System.out.print("Enter rows and columns for Matrix B (Note: rows must be " + c1 + "): ");
            int r2 = sc.nextInt();
            int c2 = sc.nextInt();

            if (c1 != r2) {
                System.out.println("Matrix multiplication not possible!");
                return;
            }

            int[][] matrixA = new int[r1][c1];
            int[][] matrixB = new int[r2][c2];

            System.out.println("Enter elements for Matrix A:");
            fillMatrix(matrixA, sc);
            System.out.println("Enter elements for Matrix B:");
            fillMatrix(matrixB, sc);

            
            out.writeObject(matrixA);
            out.writeObject(matrixB);
            out.flush();

            
            int[][] result = (int[][]) in.readObject();

            System.out.println("Resulting Matrix from Server:");
            printMatrix(result);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void fillMatrix(int[][] matrix, Scanner sc) {
        for (int i = 0; i < matrix.length; i++)
            for (int j = 0; j < matrix[0].length; j++)
                matrix[i][j] = sc.nextInt();
    }

    private static void printMatrix(int[][] matrix) {
        for (int[] row : matrix) {
            for (int val : row) System.out.print(val + " ");
            System.out.println();
        }
    }
}

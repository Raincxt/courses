import java.net.ServerSocket;
import java.net.Socket;

public class Main { 
    public static void main(String[] args) {
        try {
            ServerSocket serverSocket = new ServerSocket(2635);
            while (true) {
                Socket socket = serverSocket.accept();
                new HttpServer(socket, "D:/net/").start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
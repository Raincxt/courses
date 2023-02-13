import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.text.ParseException;

public class HttpServer extends Thread {
    private InputStream req;
    private OutputStream res;
    private String root;
    private static final int BUFFER_SIZE = 4096;
    private static final String user = "3200102635";
    private static final String password = "2635";
    
    public HttpServer(Socket socket, String root) {
        try {
            req = socket.getInputStream();
            res = socket.getOutputStream();
            this.root = root;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            System.out.println(Thread.currentThread().getName() + "start processing request...");
            Request request = getRequestHeader(); 
            if (request.getMethod() == Request.Method.GET)
                responseGET(request.getURL());
            else if (request.getMethod() == Request.Method.POST)
                responsePOST(request);
            else
                throw new ParseException("Method" + request.getMethod() + "not supported", 0);
            System.out.println(Thread.currentThread().getName() + "execute finish.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void responseGET(String filepath) throws IOException {
        File file = new File(root, filepath);
        System.out.println("An user request for file: " + file.getName());
        if (!filepath.equals("/") && file.exists()) {
            String extension;
            InputStream in = new FileInputStream(file);
            byte[] bytes = new byte[BUFFER_SIZE];
            int len;
            res.write("HTTP/1.1 200 OK\r\n".getBytes());
            int index = filepath.lastIndexOf('.');
            extension = filepath.substring(index);
            if (extension.equals(".jpg")) {
                res.write("content-type:image/jpg \r\n".getBytes());
            } else if (extension.equals(".html")) {
                res.write("content-type:text/html;charset=UTF-8\r\n".getBytes());
            } else {
                res.write("content-type:text/plain;charset=UTF-8\r\n".getBytes());
            }
            res.write("\r\n".getBytes());
            while ((len = in.read(bytes)) != -1) {
                res.write(bytes, 0, len);
            }
            res.flush();
            res.close();
            in.close();
        } else { // file not exist
            String err = "HTTP/1.1 404 file not found \r\n" + "Content-Type:text/html;charset=UTF-8\r\n" +
                "Content-Length:22 \r\n" + "\r\n" + "<h1>404 not found</h1>";
            res.write(err.getBytes());
            res.flush();
            res.close();
        }
    }

    private void responsePOST(Request request) throws IOException {
        if (request.getURL().equals("/dopost") && request.params.containsKey("login") && request.params.containsKey("pass")) {
            String msg;
            String login = (String) request.params.get("login");
            String pass = (String) request.params.get("pass");
            if (login.equals(user) && pass.equals(password)) {
                msg = "<html><head><meta charset=\"UTF-8\"></head><body>login success!</body></html>";
            } else {
                msg = "<html><head><meta charset=\"UTF-8\"></head><body>login fail!</body></html>";
            }
            msg = "HTTP/1.1 200 OK\r\n" + "Content-Type:text/html \r\n" + 
                "Content-Length:" + msg.length() + "\r\n" + "\r\n" + msg;
            res.write(msg.getBytes());
            res.flush();
            res.close();
        } else {
            String err = "HTTP/1.1 404 file not found \r\n" + "Content-Type:text/html \r\n" +
                "Content-Length:22 \r\n" + "\r\n" + "<h1>404 not found</h1>";
            res.write(err.getBytes());
            res.flush();
            res.close();
        }
    }

    private Request getRequestHeader() throws IOException, ParseException, NumberFormatException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(req));
        String line = reader.readLine();
        String[] list = line.split(" ");
        if (list.length != 3)
            throw new ParseException("HTTP RequestHeader parse error", 0);
        Request request = new Request(list[1], Request.Method.valueOf(list[0].trim())); // trim: delete blank at head + tail
        if (request.getMethod() == Request.Method.POST) {
            int contentLength = 0;
            while ((line = reader.readLine()) != null) {
                if ("".equals(line)) { // at the end of the request
                    break;
                } else if (line.contains("Content-Length")) { // calculate length
                    contentLength = Integer.parseInt(line.substring(line.indexOf("Content-Length") + 16));
                }
            }
            char[] buf;
            if (contentLength != 0) {
                buf = new char[contentLength];
                reader.read(buf, 0, contentLength);
                String[] params = new String(buf).split("&");
                for (String item : params) {
                    String key = item.substring(0, item.indexOf("="));
                    String val = item.substring(item.indexOf("=") + 1);
                    request.params.put(key, val);
                }
            }
        }
        return request;
    }
}

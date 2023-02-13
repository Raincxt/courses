import java.util.*;

public class Request {
    public enum Method {
        GET, POST
    } 

    private String URL;  
    private Method RequestMethod;  
    public Map<String, String> params;
    public Request(String URL, Method mythod) {
        this.URL = URL;
        this.RequestMethod = mythod;
        this.params = new HashMap<>();
    }

    public String getURL() {
        return URL;
    }

    public Method getMethod() {
        return RequestMethod;
    }
}
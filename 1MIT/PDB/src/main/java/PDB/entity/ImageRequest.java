package PDB.entity;

/**
 * Trida obsahujic strukturu pro navraceni obrazku klientovi
 * @author David KoLeckar
 */
public class ImageRequest{
    private String data;

    public ImageRequest() {};

    /**
     * Konstruktor tridy
     * @param data
     */
    public ImageRequest(String data) {
        this.data = data;
    }

    /**
     * Vraci data base64
     * @return data
     */
    public String getData() {
        return data;
    }

    /**
     * Nastavi data base64
     * @param data
     */
    public void setData(String data) {
        this.data = data;
    }
}

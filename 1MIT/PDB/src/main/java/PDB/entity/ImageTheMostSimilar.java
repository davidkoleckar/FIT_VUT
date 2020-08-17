package PDB.entity;

/**
 * Trida obsahujici strukturu, ktera je vracena pri operaci
 * hledani nejpodobnejsiho obrazku
 * @author David Koleckar
 */
public class ImageTheMostSimilar {

    private String data;
    private Long id;

    public ImageTheMostSimilar() {};

    public ImageTheMostSimilar(String data, Long id) {
        this.data = data;
        this.id = id;
    }

    /**
     * Obsahuje base64 string
     * @return string
     */
    public String getData() {
        return data;
    }

    /**
     * Nastavi base64 string
     * @param data
     */
    public void setData(String data) {
        this.data = data;
    }

    /**
     * ID vozidla
     * @return id
     */
    public Long getId() {
        return id;
    }

    /**
     * Nastavi ID vozidla
     * @param id
     */
    public void setId(Long id) {
        this.id = id;
    }
}

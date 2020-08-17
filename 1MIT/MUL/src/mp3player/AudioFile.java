/*
 * MUL - MP3 PLAYER
 * Autor - David Koleckar - xkolec07
 */
package mp3player;


import java.io.File;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.scene.image.Image;

/**
 * Trida reprezentujici zvukovy soubor a jeho informace
 * @author David Koleckar
 */
public class AudioFile {
    private StringProperty name;
    private StringProperty genre;
    private StringProperty album;
    private StringProperty artist;
    private StringProperty year;
    private StringProperty title;
    private File f;
    private Image img;
    
    /**
     * Konstruktor pro inicializaci promenych
     */
    public AudioFile() {
        this.name = new SimpleStringProperty("");
        this.genre = new SimpleStringProperty("");
        this.album = new SimpleStringProperty("");
        this.artist = new SimpleStringProperty("");
        this.year = new SimpleStringProperty("");
        this.title = new SimpleStringProperty("");
        this.f = null;
        this.img = null;
    }
    
    /**
     * Setter file
     * @param file
     */
    public void setFile(File file){
        this.f = file;
    }
    
    /**
     * Getter file
     * @return file
     */
    public File getFile(){
        return this.f;
    }
    
    /**
     * Getter image
     * @return image
     */
    public Image getImage(){
        return this.img;
    }
    
    /**
     * Setter image
     * @param image
     */
    public void setImage(Image image){
        this.img = image;
    }
    
    //-------------NAME  
    
    public String getName() {
        return name.get();
    }

    public void setName(String name) {
        this.name.set(name);
    }

    public StringProperty nameProperty() {
        return name;
    }
    //-------------LENGTH
    
    public String getGenre() {
        return genre.get();
    }

    public void setGenre(String genre) {
        this.genre.set(genre);
    }

    public StringProperty genreProperty() {
        return genre;
    } 
    
    //-------------ARTIST
    
    public String getArtist() {
        return artist.get();
    }

    public void setArtist(String artist) {
        this.artist.set(artist);
    }

    public StringProperty artistProperty() {
        return artist;
    }
    //-------------ALBUM
    
    public String getAlbum() {
        return album.get();
    }

    public void setAlbum(String album) {
        this.album.set(album);
    }

    public StringProperty albumProperty() {
        return album;
    }
    //-------------TITLE
    
    public String getTitle() {
        return title.get();
    }

    public void setTitle(String title) {
        this.title.set(title);
    }

    public StringProperty titleProperty() {
        return title;
    }
    
    //-------------YEAR
    
    public String getYear() {
        return year.get();
    }

    public void setYear(String year) {
        this.year.set(year);
    }

    public StringProperty yearProperty() {
        return year;
    }
}

/*
 * MUL - MP3 PLAYER
 * Autor - David Koleckar - xkolec07
 */
package mp3player;

import java.io.File;
import java.io.FilenameFilter;
import javafx.beans.Observable;
import javafx.collections.FXCollections;
import javafx.collections.MapChangeListener;
import javafx.collections.ObservableList;
import javafx.scene.image.Image;
import javafx.scene.media.Media;
import javafx.stage.DirectoryChooser;
import javafx.stage.Stage;

/**
 * Trida pro praci s adresarem
 * @author David Koleckar
 */
public class FilesRepository {
    
    private File dir;
    private File[] files;
    private ObservableList<AudioFile> data; 
    private Media media;
    private int position;

    /**
     * Konstruktor pro inicializaci promenych
     */
    public FilesRepository(){
        this.dir = null;
        this.files = null;
        this.data =FXCollections.observableArrayList(p -> new Observable[] 
                    {p.nameProperty(), p.albumProperty(), p.artistProperty()
                    ,p.genreProperty(),p.yearProperty(),p.titleProperty()});
        this.position = 0;
    }
    
    /**
     * Funkce pro vytvoreni dialogu pro vyber slozky
     * @param stage 
     */
    public void openDialogFolder(Stage stage){
        DirectoryChooser directoryChooser = new DirectoryChooser();
        configuringDirectoryChooser(directoryChooser);
        this.dir = directoryChooser.showDialog(stage);
        loadAllFiles();
    }
    
    /**
    * Funkce pro ziskani media
    * @return media
    */   
    public Media getMedia(){
        return this.media;
    }
    
    /**
     * Funkce pro ziskani obrazku alba
     * @return image
     */
    public Image getImage(){
        return this.data.get(this.position).getImage();
    }
    
    /**
     * Funkce pro ziskani aktualni pozice pisnicky
     * @return position (int)
     */
    public int getPosition(){
        return this.position;
    }
    
    /**
     * Funkce pro ziskani pisnicky po kliknuti do tabulky na dane pozici
     * @param name (String) nazev souboru v tabulce
     * @return boolean
     */
    public boolean setClickTable(String name){
        for(AudioFile current : this.data) {
            if(current.getName().equals(name)){
                this.position = this.data.indexOf(current);
                this.media = new Media(this.data.get(this.position).getFile().toURI().toString());
                return true;
            }
        }
        return false;
    }
    
    /**
     * Funkce pro konfiguraci dialogoveho okna
     * @param directoryChooser
     */
    private void configuringDirectoryChooser(DirectoryChooser directoryChooser){
        directoryChooser.setTitle("Select some folder");
        directoryChooser.setInitialDirectory(new File(System.getProperty("user.home")));
    }
    
    /**
     * Funkce pro ziskani vsech audio souboru ve slozce
     * Formaty: mp3,wav,aac,pcm
     */
    private void loadAllFiles(){
        this.files = dir.listFiles(new FilenameFilter() {
        @Override
        public boolean accept(File dir, String name) {
            return name.toLowerCase().endsWith(".mp3") ||
                   name.toLowerCase().endsWith(".wav") ||
                   name.toLowerCase().endsWith(".aac") ||
                   name.toLowerCase().endsWith(".pcm");
        }
        });
        if(this.files != null)
            this.media = new Media(this.files[this.position].toURI().toString());
    }
    
    /**
     * Funkce pro posun pisnicky do leva
     */
    public void changeLeftMedia(){
        this.position = Math.floorMod(this.position-1, this.data.size());
        this.media = new Media(this.data.get(this.position).getFile().toURI().toString());
    }
    
    /**
     * Funkce pro posun pisnicky do prava
     */
    public void changeRightMedia(){
        this.position = Math.floorMod(this.position+1, this.data.size());
        this.media = new Media(this.data.get(this.position).getFile().toURI().toString());
    }
    
    /**
     * Funkce pro vyplneni tabulky daty
     * V pripade MP3 nacita ID3TAGY
     */
    public void fillTableData(){
        Media m;
        for(File f : this.files){
            AudioFile audioF = new AudioFile();
            audioF.setName(f.getName());
            audioF.setFile(f);
            if(fileExtension(f.getName()).equals("mp3")){
                m = new Media(f.toURI().toString());
                m.getMetadata().addListener(new MapChangeListener<String, Object>() {
                  @Override
                  public void onChanged(Change<? extends String, ? extends Object> ch) {
                    if (ch.wasAdded()) {
                      handleMetadata(ch.getKey(), ch.getValueAdded(), audioF);
                    }
                  }
                });
            }
            this.data.add(audioF);
        }
    }
    
    /**
     * Funkce pro cteni ID3TAG
     * @param key (String)
     * @param value (Object)
     * @param audioF (AudioFile)
     */
    private void handleMetadata(String key, Object value, AudioFile audioF) {
        if (key.equals("album")) {
            audioF.setAlbum(value.toString());
        } else if (key.equals("artist")) {
            audioF.setArtist(value.toString());
        } else if (key.equals("title")) {
            audioF.setTitle(value.toString());
        } else if (key.equals("year")) {
            audioF.setYear(value.toString());
        } else if(key.equals("image")){
            audioF.setImage((Image) value);
        } else if(key.equals("genre")){
            audioF.setGenre(value.toString());
        }
    }
    
    /**
     * Funkce pro ziskani listu s pisnickami
     * @return list s daty
     */
    public ObservableList<AudioFile> getTableData(){
        return this.data;
    }
    
    /**
     * Funkce pro ziskani jmena souboru
     * @param f (File)
     * @return filename (String)
     */
    private String fileName(File f){
        String fileName = f.getName();
        int pos = fileName.lastIndexOf(".");
        if (pos > 0 && pos < (fileName.length() - 1)) {
            fileName = fileName.substring(0, pos);
        }
        return fileName;
    } 
    
    /**
     * Funkce pro ziskani pripony souboru
     * @param name (String)
     * @return pripona souboru (String)
     */
    private String fileExtension(String name) {
        name = name.toLowerCase();
        try {
            return name.substring(name.lastIndexOf(".") + 1);
        } catch (Exception e) {
            return "";
        }
    }   
}

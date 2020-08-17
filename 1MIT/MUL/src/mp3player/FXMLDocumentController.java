/*
 * MUL - MP3 PLAYER
 * Autor - David Koleckar - xkolec07
 */
package mp3player;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.beans.binding.Bindings;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaPlayer.Status;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

/**
 * Trida pro praci s GUI - controller
 * @author David Koleckar
 */
public class FXMLDocumentController implements Initializable {
    
    private Stage stage;
    private FilesRepository frep;
    private MediaPlayer mediaPlayer;
    
    //Inicializace promenych reprezentujici casti GUI 
    @FXML
    private Button play;
    @FXML
    private ImageView image;
    @FXML
    private Slider volume;
    @FXML
    private Slider duration;
    @FXML
    private Label start;
    @FXML
    private Label end;
    @FXML
    private TableView<AudioFile> table;
    @FXML
    private TableColumn<AudioFile, String> nameColumn;
    @FXML
    private TableColumn<AudioFile, String> titleColumn;
    @FXML
    private TableColumn<AudioFile, String> genreColumn;
    @FXML
    private TableColumn<AudioFile, String> albumColumn;
    @FXML
    private TableColumn<AudioFile, String> yearColumn;
    @FXML
    private TableColumn<AudioFile, String> artistColumn;
    @FXML
    private BarChart<String,Number> bc;
    @FXML
    private CategoryAxis  xAxis;
    @FXML
    private NumberAxis  yAxis; 
    
    /**
     * Handler tlacitka play
     * @param event
     */
    @FXML
    private void handleButtonPlay(ActionEvent event) {
        if(this.mediaPlayer != null){
            if (mediaPlayer.getStatus() == Status.PAUSED || mediaPlayer.getStatus() == Status.READY){
                this.mediaPlayer.play();
                play.setText("Pause");
            }
            else if(mediaPlayer.getStatus() == Status.PLAYING){
                this.mediaPlayer.pause();
                play.setText("Play");
            }
        }
    }
    
    /**
     * Handler tlacitka pro posun skladby do leva
     * @param event
     */
    @FXML
    private void handleButtonLeft(ActionEvent event) {
        if(this.mediaPlayer != null){
            this.mediaPlayer.stop();
            this.frep.changeLeftMedia();
            changeSong();
        }
    }
    
    /**
     * Handler tlacitka pro posun skladby do prava
     * @param event
     */
    @FXML
    private void handleButtonRight(ActionEvent event) {
        if(this.mediaPlayer != null){
            this.mediaPlayer.stop();
            this.frep.changeRightMedia();
            changeSong();
        }
    }
    
    /**
     * Funkce pro zmenu pisnicky 
     * Zmeni text tlacitka Play/Pause
     * Zmeni pozici v ramci tabulky
     */
    private void changeSong(){
        if (mediaPlayer.getStatus() == Status.PLAYING){
            this.mediaPlayer = new MediaPlayer(this.frep.getMedia());
            this.mediaPlayer.play();
            play.setText("Pause");
        }
        else if (mediaPlayer.getStatus() == Status.PAUSED || mediaPlayer.getStatus() == Status.READY || mediaPlayer.getStatus() == Status.STOPPED){
            this.mediaPlayer = new MediaPlayer(this.frep.getMedia());
            play.setText("Play");
        }
        table.getSelectionModel().select(frep.getPosition());
        mediaPlayerHandlerDuration();
        mediaVisualition();
    }
    
    /**
     * Handler tlacitka pro otevreni adresare
     * @param event
     */
    @FXML
    private void handleButtonOpen(ActionEvent event) {
        
        if(this.mediaPlayer != null){
            this.mediaPlayer.stop();
        }
        this.frep = new FilesRepository();
        this.frep.openDialogFolder(this.stage);
        this.frep.fillTableData();
        
        nameColumn.setCellValueFactory(new PropertyValueFactory<AudioFile,String>("name"));
        genreColumn.setCellValueFactory(new PropertyValueFactory<AudioFile,String>("genre"));
        titleColumn.setCellValueFactory(new PropertyValueFactory<AudioFile,String>("title"));
        albumColumn.setCellValueFactory(new PropertyValueFactory<AudioFile,String>("album"));
        yearColumn.setCellValueFactory(new PropertyValueFactory<AudioFile,String>("year"));
        artistColumn.setCellValueFactory(new PropertyValueFactory<AudioFile,String>("artist"));
        table.setItems(this.frep.getTableData());
        table.getSelectionModel().select(0);

        this.mediaPlayer = new MediaPlayer(this.frep.getMedia());
        mediaPlayerHandlerVolume();
        mediaPlayerHandlerDuration();
        tableClick();
        mediaVisualition();
    }
    
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        
    }  
    
    /**
     * Funkce pro zajisteni listeneru pri kliknuti do tabulky
     */
    private void tableClick(){
        table.setOnMouseClicked((MouseEvent event) -> {
            if (event.getButton().equals(MouseButton.PRIMARY) && event.getClickCount() == 2){
                if(this.frep.setClickTable(table.getSelectionModel().getSelectedItem().getName())){
                    this.mediaPlayer.stop();
                    changeSong();
                }
            }
        });
    }
    
    /**
     * Funkce pro nastaveni stage
     * @param stage
     */
    public void setStage(Stage stage){
        this.stage=stage;
    }
    
    /**
     * Funkce pro listener umoznujici zmenu hlasitosti (slider)
     */
    private void mediaPlayerHandlerVolume(){
        volume.setValue(this.mediaPlayer.getVolume() * 100);
        volume.valueProperty().addListener(new InvalidationListener() {
            @Override
            public void invalidated(Observable observable) {
                mediaPlayer.setVolume(volume.getValue()/100);
            }
        });
    }
    
    /**
     * Funkce pro nastaveni obrazku alba,
     * Dale provadi zmenu labelu pro zacatek a konec casu skladby
     */
    private void mediaPlayerLabel(){
        this.mediaPlayer.setOnReady(() -> {
           //Set image 
           image.setImage(frep.getImage());

           //Label start
           start.textProperty().bind(
           Bindings.createStringBinding(() -> {
                   javafx.util.Duration time = this.mediaPlayer.getCurrentTime();                  
                   return String.format("%02d:%02d",
                       (int) ((time.toSeconds() % (60*60))/60),
                       (int) (time.toSeconds() % 60));
               },
               this.mediaPlayer.currentTimeProperty()));
           //Label end
           end.textProperty().bind(
           Bindings.createStringBinding(() -> {
                   javafx.util.Duration time = this.mediaPlayer.getTotalDuration();
                   return String.format("%02d:%02d",
                       (int) ((time.toSeconds() % (60*60))/60),
                       (int) (time.toSeconds() % 60));
           }));

       });    
    }
    
    /**
     * Funkce pro listener umoznujici posun skladby v case (slider)
     */
    private void mediaPlayerHandlerDuration(){  
        mediaPlayerLabel();
       
        this.mediaPlayer.totalDurationProperty().addListener((obs, oldDuration, newDuration) -> duration.setMax(newDuration.toSeconds()));
        duration.valueChangingProperty().addListener((obs, wasChanging, isChanging) -> {
            if (! isChanging) {
                this.mediaPlayer.seek(javafx.util.Duration.seconds(duration.getValue()));
            }
        });

        duration.valueProperty().addListener((obs, oldValue, newValue) -> {
            if (! duration.isValueChanging()) {
                double currentTime = this.mediaPlayer.getCurrentTime().toSeconds();
                if (Math.abs(currentTime - newValue.doubleValue()) > 0.5) {
                    this.mediaPlayer.seek(javafx.util.Duration.seconds(newValue.doubleValue()));
                }
            }
        });

        this.mediaPlayer.currentTimeProperty().addListener((obs, oldTime, newTime) -> {
            if (! duration.isValueChanging()) {
                duration.setValue(newTime.toSeconds());
            }
        });
        
        this.mediaPlayer.setOnEndOfMedia(new Runnable() {
            @Override
            public void run() {
                mediaPlayer.stop();
                frep.changeRightMedia();
                changeSong();
                return;
            }
        });
    }
    
    /**
     * Funkce pro vizualizaci skladby
     * xAxis - Frekvence
     * yAxis - dB
     * Reprezentace jako sloupcovy graf
     */
    private void mediaVisualition(){
        bc.getData().clear();
        yAxis = new NumberAxis(0,50,10);
        bc.setVisible(true);
        bc.setLegendVisible(false);
        bc.setAnimated(false);
        bc.setBarGap(0);
        bc.setCategoryGap(0);
        bc.setVerticalGridLinesVisible(false);
        bc.setHorizontalGridLinesVisible(false);
        bc.setHorizontalZeroLineVisible(false);
        bc.setVerticalZeroLineVisible(false);
        bc.setMaxSize(360, 150);
        bc.setMinSize(360, 150);
        yAxis.setTickLabelFormatter(new NumberAxis.DefaultFormatter(yAxis,null,"dB"));
        xAxis.setTickLabelFill(Color.TRANSPARENT);
        yAxis.setTickLabelFill(Color.TRANSPARENT);
        
        
        XYChart.Series<String,Number>  series1 =new XYChart.Series<> ();
        series1.setName("Series");      
        XYChart.Data[] series1Data = new XYChart.Data[128];
     
        for (int i=0; i<series1Data.length; i++) {
            series1Data[i] = new XYChart.Data<>( Integer.toString(i+1),0);
            series1.getData().add(series1Data[i]);
        }
     
        bc.getData().add(series1);
        
        
        mediaPlayer.setAudioSpectrumListener((double timestamp, double duration, float[] magnitudes , float[] phases) -> {
         
          for(int i=0;i<magnitudes.length;i++){
            series1Data[i].setYValue((magnitudes[i]+60)); //Top Series
          }
        });
    }
    
}

/*
 * MUL - MP3 PLAYER
 * Autor - David Koleckar - xkolec07
 */
package mp3player;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 * Main class
 * @author David Koleckar
 */
public class Mp3player extends Application {
    
    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("FXMLDocument.fxml"));
        Parent root = (Parent) fxmlLoader.load();
        ((FXMLDocumentController) fxmlLoader.getController()).setStage(stage);

        Scene scene = new Scene(root);
        
        stage.setScene(scene);
        stage.setTitle("MP3 PLAYER");
        stage.show();
    }

    /**
     * @param args argumenty programu (command line)
     */
    public static void main(String[] args) {
        launch(args);
    }
    
}

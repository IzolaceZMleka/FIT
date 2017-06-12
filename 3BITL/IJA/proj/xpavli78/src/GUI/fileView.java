/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package GUI;

import java.io.File;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 *
 * @author Patrik Chukir, xchuki00
 * Třída otevře okno se seznamem souborou ve složce saves a po stisku OK vrátí název vybraného,
 * při stisku cancel nebo zavření křížkem vrátí null.
 * 
 */
public class fileView{

    /**
     *
     * @return
     */
    public static String runFileView() {
        Stage stage = new Stage();
        stage.setTitle("Vyberte slot");
        File f = new File("./examples");
        File[] paths = f.listFiles();
        BorderPane root = new BorderPane();
        ObservableList<String> list = FXCollections.observableArrayList();
        for (File p : paths) {
            list.add(p.toString());
        }
        ListView<String> listView = new ListView<String>(list);
        final String[] ret = new String[1];
        listView.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                ret[0] = new String(newValue);
            }
        });
          stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent event) {
                         ret[0] = "non";
                  
            }
        });
        Button b = new Button("  Ok  ");

        b.setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);

        b.setOnAction(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent t) {
                stage.close();
            }
        });
        Button can = new Button("Cancel");
        can.setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);
        can.setOnAction(new EventHandler<ActionEvent>() {
            public void handle(ActionEvent t) {
                // Platform.exit();
                ret[0] = "non";
                stage.close();
            }
        });
        root.setCenter(listView);
        HBox p = new HBox(2);
        b.setPrefWidth(75);
        can.setPrefWidth(75);
        p.setAlignment(Pos.BASELINE_CENTER);
        p.setSpacing(25);
        p.getChildren().addAll(b, can);

        root.setBottom(p);

        stage.setScene(new Scene(root, 250, 300));
        stage.showAndWait();
       // System.out.println(ret[0]);
        if (ret[0] == "non") {
            return null;
        }
        return ret[0];
    }
}

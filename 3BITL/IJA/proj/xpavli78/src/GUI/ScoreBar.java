/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package GUI;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;

/**
 *
 * @author Patrik Chukir, xchuki00
 * Třída rozšiřující GridPane, v kostruktoru se sestaví do 1x8, 
 * kdy do každého sloupce se vloží buď Label s nápisem Score a číslem plátna, 
 * nebo přímo aktualní score.
 */
public class ScoreBar  extends GridPane {

    /**
     *Reference na měnitelné labely, které se pak předávájí GameMenu a dále plátnům k zobrazování score.  
     */
    public Label[] scorePlace = new Label[4];

    /**
     *
     */
    public ScoreBar() {
        for (int i = 0; i < 8; i++) {
            ColumnConstraints col1 = new ColumnConstraints();
            col1.setHgrow(Priority.ALWAYS); // allow column to grow
            col1.setFillWidth(true); // ask nodes to fill space for column
            col1.setPercentWidth(50);
            this.getColumnConstraints().add(col1);
        }
        for(int i = 0; i<4;i++){
            Label l =new Label("Score "+i+":");
            l.setAlignment(Pos.CENTER);
            scorePlace[i]= new Label("0.0");
            this.add(l, i*2, 0);
            this.add(scorePlace[i], i*2+1, 0);
        }
       this.setAlignment(Pos.BOTTOM_LEFT);
        this.setPadding(new Insets(5.0));
    }

}

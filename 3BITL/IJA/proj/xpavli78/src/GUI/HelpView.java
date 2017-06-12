/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package GUI;

import java.awt.Desktop;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import javafx.scene.control.Hyperlink;
import javafx.scene.image.Image;

/**
 *
 * @author Patrik Chukir, xchuki00
 * 
 * Třída rozšiřující Stage, je vní umístněná nápověda a odkaz na pravidla na wikipedii.
 */
public class HelpView extends Stage{

    /**
     *
     */
    public HelpView() {
        //Stage stage = new Stage();
        this.setTitle("Nápověda");
        BorderPane root = new BorderPane();
        VBox vb = new VBox();
        final Hyperlink hpl = new Hyperlink("Wikipedie.");

        hpl.setFont(Font.font("Arial", 14));

        hpl.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {

                if (Desktop.isDesktopSupported()) {
                    try {
                        Desktop.getDesktop().browse(new URI("https://en.wikipedia.org/wiki/Klondike_(solitaire)"));

                    } catch (URISyntaxException ex) {
                        Logger.getLogger(HelpView.class.getName()).log(Level.SEVERE, null, ex);
                         }   catch (IOException ex) {   
                        Logger.getLogger(HelpView.class.getName()).log(Level.SEVERE, null, ex);
                    }   
            }}
        });
        TextArea rule = new TextArea();
        rule.setText("Pravidla hry Solitair Klondike\n"
                + "Cílem hry je premistit vsech 52 karet do hornich 4 polí.\n"
                + "Kazde toto pole ma svoji barvu, a pouze karty této barvy\n"
                + " tam mohou by vlozeny v vzestupném pořadí, bez přeskakování.\n"
                + "Dolnich 7 polich slouží k přeskládávání a odkryvani karet, zde\n"
                + "se karty skladají sestupně se střídáním černé a červené.\n"
                + "Body se přičítají za každou přesunutou nebo otočenou kartu.\n"
                + "Odečítají se za otčení dobíracího balíčku a použití nápovědy.\n"
                + "Pokud by jste chteli podrobnější pravidla níže přikládám odkaz\n"
                + " na jejich popis na Wikipedii.\n");
        vb.getChildren().addAll(rule, hpl);
        root.setCenter(vb);
        this.getIcons().add(new Image(getClass().getResourceAsStream("/spades_bot.png")));
        this.setScene(new Scene(root, 400, 300));
        this.show();
    }

}

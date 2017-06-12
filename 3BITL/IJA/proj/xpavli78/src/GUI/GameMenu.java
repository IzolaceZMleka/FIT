package GUI;

import Boards.Platno;
import Cards.KlondikeCard;
import Other.Move;
import java.util.ArrayList;
import java.util.List;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.CacheHint;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.effect.ColorAdjust;
import javafx.scene.image.ImageView;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.util.Duration;

/**
 * GameMenu Třída určená k vytvoření lišty s menu pro jednotlivá plátna a
 * realizaci příkazů. Rozšiřuje třídu MenuBar. Zde tedy vznikaji nové instance
 * Platen a k nim příslušných GameView.
 *
 * @author Patrik Chukir, xchuki00
 */
public class GameMenu extends MenuBar {

    /**
     * menu Entityta MenuBar z Javafx, je korenovým uzel pro všechny prvky v
     * liště.
     *
     */
    //public MenuBar menu;
    /**
     * games Pole 4 GameView, zde se uchovava ukazatel na jednotlive rozehrane
     * hry. Slouzi k pristupu k hram pro realiza přikazů, jako je například
     * Hint/Nápověda
     */
    public GameView[] games = new GameView[4];

    /**
     * pane GridPane z javafx, vytvari se v tride Klondike a je předán v
     * konstruktoru GameMenu. Tvoří síť 2x2 a do jeho jednotlivých oken se
     * dynamicky vládají instance GameView z games.
     */
    GridPane pane;
    /**
     * platnos Pole 4 instancí tridy Platno, které uchovávají logické informace
     * o stavu hry pro příslušné GameView.
     */
    Platno[] platnos = new Platno[4];
    /**
     * scorePlace Pole 4 polí pro label s scorem jednotlivých her, label jsou
     * vytvoreny a nastylovány v ScoreBar. Zde jsou ulozeny pouze reference,
     * které se predávají konstruktorům platno().
     */
    Label[] scorePlace;
    /**
     * countOfGame Počet aktuálně rozehraných her, pomáhá při rozhodovací logice
     * jak mám být GameView velké, jestli přes jeden čtverec pane nebo všechny
     * čtyři.
     */
    int countOfGame = 0;

    /**
     *
     * @param gridPaneInput
     * @param pPrimaryStage Jediný konstuktor GameMenu, zde se vytvori menuBar a
     * MenuItems plus navázání MenuItems k jednotlivým ovladačům.
     */
    public GameMenu(GridPane gridPaneInput, Stage pPrimaryStage) {
        gridPaneInput.setStyle("-fx-background-color: green;");
        EventHandler<ActionEvent> nw;
        EventHandler<ActionEvent> save;
        EventHandler<ActionEvent> load;
        EventHandler<ActionEvent> undo;
        EventHandler<ActionEvent> quit;
        EventHandler<ActionEvent> hint;
        for (int i = 0; i < 4; i++) {
            games[i] = null;
        }
        //menu = new MenuBar();
        pane = gridPaneInput;
        nw = (ActionEvent t) -> {
            NewButtonHandler(t);
        };
        save = (ActionEvent t) -> {
            SaveButtonHandler(t);
        };
        load = (ActionEvent t) -> {
            NewButtonHandler(t);
        };
        undo = (ActionEvent t) -> {
            UndoButtonHandler(t);
        };
        quit = (ActionEvent t) -> {
            QuitButtonHandler(t);
        };
        hint = (ActionEvent t) -> {
            HintButtonHandler(t);
        };

        MenuItem nwI;
        MenuItem saveI;
        MenuItem loadI;
        MenuItem undoI;
        MenuItem quitI;
        MenuItem hintI;
        
        Menu file = new Menu("Game");
        MenuItem q = new MenuItem("Quit");
        q.setOnAction(new EventHandler<ActionEvent>() {
            public void handle(ActionEvent t) {
                System.exit(0);
            }
        });
        MenuItem h = new MenuItem("Help");
        h.setOnAction(new EventHandler<ActionEvent>() {
            public void handle(ActionEvent t) {
                new HelpView();
            }
        });
        file.getItems().add(h);
        file.getItems().add(q);
        this.getMenus().add(file);
        for (int i = 0; i < 4; i++) {
            Menu p1 = new Menu("Platno " + (i + 1));

            nwI = new MenuItem("New");
            nwI.setOnAction(nw);
            saveI = new MenuItem("Save");
            saveI.setOnAction(save);
            loadI = new MenuItem("Load");
            loadI.setOnAction(load);
            undoI = new MenuItem("Undo");
            undoI.setOnAction(undo);
            hintI = new MenuItem("Hint");
            hintI.setOnAction(hint);
            quitI = new MenuItem("Quit");
            quitI.setOnAction(quit);

            p1.getItems().add(nwI);
            p1.getItems().add(saveI);
            p1.getItems().add(loadI);
            p1.getItems().add(undoI);
            p1.getItems().add(hintI);
            p1.getItems().add(quitI);

            this.getMenus().add(p1);
        }

        this.prefWidthProperty().bind(pPrimaryStage.widthProperty());

    }

    /**
     *
     * @param s
     * @return K nastavení pole scorePlace, očekává již vytvořené a naplněné
     * pole.
     */
    public boolean setScorePlace(Label[] s) {
        scorePlace = s;
        return true;
    }

    /**
     *
     * @param t
     *
     * Obsluhuje vytvorení nové hry, tzn nové Platno a GameView, případné
     * zavření předchozí. Obsluhuje také načtení hry, tedy talčítko Load, kdy
     * jediný rozdíl oproti New je ze v tomto případě se nad Platno zavolá
     * funkce load() s návratovou hodnotou FileView jako parametrem. Zde a v
     * metodě QuitButtonHandler se řeší případný resize GameView dle počtu her v
     * countOfGame.
     *
     */
    private void NewButtonHandler(ActionEvent t) {
        String p = ((MenuItem) t.getSource()).getParentMenu().getText();
        ((MenuItem) t.getSource()).getText();
        int j = 0;
        switch (p) {
            case "Platno 1":
                j = 0;
                break;
            case "Platno 2":
                j = 1;
                break;
            case "Platno 3":
                j = 2;
                break;
            case "Platno 4":
                j = 3;
                break;
        }

        if (((MenuItem) t.getSource()).getText() == "Load") {
            String path = fileView.runFileView();
            if (path == null) {
                return;
            }
            if (platnos[j] == null) {
                platnos[j] = new Platno(scorePlace[j]);
            }
            platnos[j].LoadData(path);
        } else {
            platnos[j] = new Platno(scorePlace[j]);
        }
        if (games[j] != null) {
            pane.getChildren().remove(games[j].root);

        } else {
            countOfGame++;
        }

        games[j] = new GameView(platnos[j]);

        if (countOfGame > 2) {            ///vic jak dve hry
            pane.add(games[j].root, j / 2, j % 2);
        } else if (countOfGame <= 1) {
            pane.add(games[j].root, 0, 0, 2, 2);
        } else {
            //druha hra     
            for (int i = 0; i < 4; i++) {
                if (games[i] != null && i != j) {
                    pane.getChildren().remove(games[i].root);
                    games[i] = null;
                    games[i] = new GameView(platnos[i]);
                    pane.add(games[i].root, i / 2, i % 2);
                }
            }
            pane.add(games[j].root, j / 2, j % 2);
        }
    }

    /**
     *
     * @param t Tato metoda obsluhuje tlačítko Save, tedy volá na přislušné
     * Platno jeho metodu SaveData.
     */
    private void SaveButtonHandler(ActionEvent t) {

        String p = ((MenuItem) t.getSource()).getParentMenu().getText();
        String path = fileView.runFileView();
        int j = 0;
        switch (p) {
            case "Platno 1":
                j = 0;
                break;
            case "Platno 2":
                j = 1;
                break;
            case "Platno 3":
                j = 2;
                break;
            case "Platno 4":
                j = 3;
                break;
        }
        if (platnos[j] != null) {
            platnos[j].SaveData(path);
        }

    }

    /**
     *
     * @param t Obsluje tlačítko Undo a podobně jako save volá metodu undo z
     * Platno.
     */
    private void UndoButtonHandler(ActionEvent t) {

        String p = ((MenuItem) t.getSource()).getParentMenu().getText();
        int j = 0;
        switch (p) {
            case "Platno 1":
                j = 0;
                break;
            case "Platno 2":
                j = 1;
                break;
            case "Platno 3":
                j = 2;
                break;
            case "Platno 4":
                j = 3;
                break;
        }
        platnos[j].Undo();
        //TODO games[j].undo();
    }

    /**
     *
     * @param t Obsluje tlačítko Quit pro jedno z platen a kromně smazani
     * GameView z games a Platno z platnos, mění i velikost zbylé hry, jeli
     * poslední.
     */
    private void QuitButtonHandler(ActionEvent t) {
        String p = ((MenuItem) t.getSource()).getParentMenu().getText();
        int j = 0;
        switch (p) {
            case "Platno 1":
                j = 0;
                break;
            case "Platno 2":
                j = 1;
                break;
            case "Platno 3":
                j = 2;
                break;
            case "Platno 4":
                j = 3;
                break;
        }
        if (games[j] != null) {
            pane.getChildren().remove(games[j].root);
            games[j] = null;
            countOfGame--;
        }
        if (countOfGame == 1) {
            for (int i = 0; i < 4; i++) {
                if (games[i] != null) {
                    j = i;
                }
            }

            pane.getChildren().remove(games[j].root);
            games[j] = null;
            games[j] = new GameView(platnos[j]);
            pane.add(games[j].root, 0, 0, 2, 2);

        }
    }

    /**
     *
     * @param t
     *
     * Obsluhuje tlačítko Hint, po jeho stisku zjistí nejlepší kartu k tahu a
     * zvýrazní ji blikáním. K najití nejlepšího tahu používá instanci Advisor v
     * atributu advisor v Platno. Návratovou hodnotu pak použije k najití
     * příslušného uzlu metodou FindButton.
     */
    private void HintButtonHandler(ActionEvent t) {
        String p = ((MenuItem) t.getSource()).getParentMenu().getText();
        int j = 0;
        switch (p) {
            case "Platno 1":
                j = 0;
                break;
            case "Platno 2":
                j = 1;
                break;
            case "Platno 3":
                j = 2;
                break;
            case "Platno 4":
                j = 3;
                break;
        }
        if (platnos[j] != null) {
            platnos[j].UpdateScore(-2);
            Move coord = platnos[j].getHint();
            if (coord == null) {
                return;
            }
            Button b = findButton(coord, j);
            Timeline fiveSecondsWonder = new Timeline(new KeyFrame(Duration.seconds(0.4), new EventHandler<ActionEvent>() {
                int counter = 0;

                @Override
                public void handle(ActionEvent event) {
                    counter++;
                    if (b.getPadding() == Insets.EMPTY) {
                        b.setPadding(new Insets(2));
                        ImageView imageView = (ImageView) b.getGraphic();
                        ColorAdjust bloom = new ColorAdjust();
                        bloom.setBrightness(-1.0);
                        imageView.setEffect(bloom);
                        imageView.setCache(true);
                        imageView.setCacheHint(CacheHint.SPEED);
                    } else {
                        b.setPadding(Insets.EMPTY);
                        ImageView imageView = (ImageView) b.getGraphic();
                        ColorAdjust bloom = new ColorAdjust();
                        bloom.setBrightness(0.75);
                        if (counter == 4) {
                            bloom.setBrightness(0.0);

                        }
                        imageView.setEffect(bloom);
                        imageView.setCache(true);
                        imageView.setCacheHint(CacheHint.SPEED);
                    }
                }
            }));
            fiveSecondsWonder.setCycleCount(4);
            fiveSecondsWonder.play();
        }

    }

    /**
     *
     * @param m
     * @param j
     * @return Na základě parametru m vyhledá v GameView uzel příslušící kartě v
     * předaném Move. Tento uzel vrati jako Button z javaFx. Využívá se
     * například v rámci funkce HintButtonHandler k získání odkazu na CardView
     * pro jeho zvýraznění. Na základě m.befor vybere DeckView/DiscardPileView
     * nebo všechny TargetView/StackView a vnich pak vyhledá CardView
     * reprezentující m.card.
     */
    private Button findButton(Move m, int j) {
        Class clas = null;
        switch (m.befor.GameIndex) {
            case 0:
                for (Node n : games[j].root.getChildren()) {
                    if (n.getClass() == Pane.class) {
                        return (Button) ((Pane) n).getChildren().get(0);
                    }
                }
                break;
            case 1:
                for (Node n : games[j].root.getChildren()) {
                    if (n.getClass() == DiscardPileView.class) {
                        return (Button) ((Pane) n).getChildren().get(((Pane) n).getChildren().size() - 1);
                    }
                }
                break;
            case 2:
                clas = TargetView.class;
                break;
            case 3:
                clas = StackView.class;
                break;
        }
        List<Node> a = new ArrayList<Node>();
        for (Node n : games[j].root.getChildren()) {
            if (n.getClass() == clas) {
                a.add(n);
            }
        }
        for (Node n : a) {
            for (Node b : ((Pane) n).getChildren()) {
                KlondikeCard c = ((CardView) b).representedCard;
                if (c == m.card) {
                    return (Button) b;
                }
            }
        }

        return null;
    }


}

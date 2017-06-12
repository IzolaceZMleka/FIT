package Game;

import GUI.GameMenu;
import GUI.ScoreBar;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.Priority;
import javafx.scene.layout.RowConstraints;

/**
 *
 * @author Jan Pavlica, xpavli78
 *      * Hlavní třída, zde se i sestavuje hlavní okno. 
     * -Stage 
     *      - scene 
     *          - BordePane 
     *              - top = GridPane 
     *                  - GameMenu
     *                  - ScoreBar
     *                  .
     *              - center = GridPane
     *                  - 4x GameView
     *                  .
     *              .
     *          .
     *     .        
     * .
 */
public class Klondike extends Application {

    private static final int WIDTH = 800;
    private static final int HEIGHT = 600;
    private static final String TITLE = "Klondike";
    private static final String VERSION = "1.0";

    /**
     * Launches the application.
     *
     * @param pArgs This program takes no argument.
     * 

     */
    public static void main(String[] pArgs) {
        launch(pArgs);
    }
/**
 * 
 * @param pPrimaryStage
 * 
 * 
 */
    @Override
    public void start(Stage pPrimaryStage) {
        pPrimaryStage.setTitle(TITLE + " " + VERSION);
        //pPrimaryStage.getIcons().add(new Image(getClass().getResourceAsStream("/spades_bot.png")));
        BorderPane root = new BorderPane();
        GridPane grid = new GridPane();
        grid.setGridLinesVisible(true);
        for (int i = 0; i < 2; i++) {
            RowConstraints row1 = new RowConstraints();
            ColumnConstraints col1 = new ColumnConstraints();
            col1.setHgrow(Priority.ALWAYS); // allow column to grow
            col1.setFillWidth(true); // ask nodes to fill space for column
            col1.setPercentWidth(50);
            row1.setVgrow(Priority.ALWAYS); // allow column to grow
            row1.setFillHeight(true); // ask nodes to fill space for column
            row1.setPercentHeight(50);
            grid.getRowConstraints().add(row1);
            grid.getColumnConstraints().add(col1);
        }
        GameMenu menu = new GameMenu(grid, pPrimaryStage);
        ScoreBar sb = new ScoreBar();
        menu.setScorePlace(sb.scorePlace);
        root.setCenter(grid);
        GridPane forMenu = new GridPane();
        ColumnConstraints col1 = new ColumnConstraints();
        ColumnConstraints col2 = new ColumnConstraints();
        col1.setHgrow(Priority.ALWAYS); // allow column to grow
        col1.setFillWidth(true); // ask nodes to fill space for column
        col1.setPercentWidth(50);
        col2.setHgrow(Priority.ALWAYS); // allow column to grow
        col2.setFillWidth(true); // ask nodes to fill space for column
        col2.setPercentWidth(50);
        forMenu.getColumnConstraints().addAll(col1, col2);
        forMenu.add(menu,0,0);
        forMenu.add(sb,1,0);
        root.setTop(forMenu);

        pPrimaryStage.setResizable(true);
        pPrimaryStage.setScene(new Scene(root, WIDTH, HEIGHT));
        pPrimaryStage.show();
    }
}

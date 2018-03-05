import controller.LoginFormController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 * Class MainApplication
 * Main application class. Creates dbContext, controllers and scene.
 *
 * @author Ondřej Zemánek
 */
public class MainApplication extends Application {

    private static final String connectionString = "jdbc:oracle:thin:@//gort.fit.vutbr.cz:1521/gort.fit.vutbr.cz";

    private LoginFormController loginFormController;

    /**
     * Application main method
     *
     * @author Ondřej Zemánek
     * @param args Application input arguments.
     */
    public static void main(String[] args) {
        launch(args);
    }

    /**
     * Method called when application starts.
     *
     * @author Ondřej Zemánek
     * @param stage Main application Stage object.
     */
    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("view/LoginForm.fxml"));

        Parent root = fxmlLoader.load();

        loginFormController = fxmlLoader.getController();
        loginFormController.init(connectionString);

        Scene scene = new Scene(root);
        stage.setTitle("Zoo - Přihlášení do systému");
        stage.setScene(scene);
        stage.show();
    }

    /**
     * Method called when application being terminated.
     * Method close connection to DB.
     *
     * @author Ondřej Zemánek
     */
    @Override
    public void stop() throws Exception {
        if(loginFormController != null &&
           loginFormController.GetDbContext() != null) {
            loginFormController.GetDbContext().CloseConnection();
        }
    }

}

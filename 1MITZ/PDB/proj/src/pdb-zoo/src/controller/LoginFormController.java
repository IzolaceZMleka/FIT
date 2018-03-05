package controller;

import java.sql.SQLException;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import model.DbContext;

/**
 * Class LoginFormController
 * Controller for LoginForm view
 *
 * @author Ondřej Zemánek
 */
public class LoginFormController {

    private DbContext dbContext;

    @FXML
    private TextField tfLogin;
    @FXML
    private TextField tfPassword;
    @FXML
    private CheckBox chbInitSql;

    /**
     * Method for init DbContext.
     *
     * @author Ondřej Zemánek
     * @param dbConnectionString DB connection string.
     */
    public void init(String dbConnectionString) {
        this.dbContext = new DbContext(dbConnectionString);
    }

    /**
     * Method returns controller DbContext.
     *
     * @author Ondřej Zemánek
     * @return returns controller DbContext.
     */
    public DbContext GetDbContext() {
        return dbContext;
    }

    /**
     * Method handle SignIn button action.
     * Method create connection to db and eventually run init SQL script and show main window and close login form.
     *
     * @author Ondřej Zemánek
     * @param action Button ActionEvent.
     */
    public void HandlebtnSignIn(ActionEvent action) {
        if (tfLogin.getText().isEmpty() || tfPassword.getText().isEmpty()) {
            ShowErrorAlert("Přihlášení selhalo.", "Pro přihlášení musíte vyplnit login a heslo.");
            return;
        }

        if(dbContext.OpenConnection(tfLogin.getText(), tfPassword.getText()) == null) {
            ShowErrorAlert(
                    "Přihlášení selhalo.",
                    "Nepodařilo se navázat spojení s databází. Zkontrolujte přihlašovací údaje.");
            return;
        }

        if(chbInitSql.isSelected()) {
            try {
                dbContext.RunScript();
            }catch(SQLException ex) {
                ShowErrorAlert(
                        "Chyba při aplikaci inicializačního skriptu.",
                        "Nastala chyba během inicializačního skriptu. Zkontrolujte přihlašovací údaje a připojení k internetu.");
                return;
            }

        }

        ShowMainForm();

        Stage stage = (Stage)((Button)action.getSource()).getScene().getWindow();
        stage.close();
    }

    /**
     * Method hanlde Storno button action.
     * Method for closing login form window.
     *
     * @author Ondřej Zemánek
     * @param action Button ActionEvent.
     */
    public void HandlebtnStorno(ActionEvent action) {
        Stage stage = (Stage) ((Button)action.getSource()).getScene().getWindow();
        stage.close();
    }

    /**
     * Method for showing Error alert dialog.
     *
     * @author Ondřej Zemánek
     * @param title dialog title text.
     * @param header dialog header text.
     */
    private void ShowErrorAlert(String title, String header) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(header);
        alert.show();
    }

    /**
     * Method for showing Main form window.
     *
     * @author Ondřej Zemánek
     */
    public void ShowMainForm() {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("/view/MainForm.fxml"));
            Parent root = (Parent)fxmlLoader.load();

            MainFormController controller = fxmlLoader.getController();
            controller.setDbConnection(dbContext.GetConnection());
            controller.makeMap();
            Stage stage = new Stage();
            stage.setScene(new Scene(root));
            stage.show();
        } catch(Exception e) {
            System.out.println(e);
        }
    }
}

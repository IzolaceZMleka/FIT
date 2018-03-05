package controller;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.net.URL;
import java.sql.Connection;
import java.time.LocalDate;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.ResourceBundle;

import gui.Animal;
import gui.HistorieVybehu;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.DatePicker;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import model.DruhZvireteModel;
import model.ZvireModel;
import model.entity.DruhZvireteEntity;
import model.entity.HistorieUmisteni;
import model.entity.ZvireEntity;


/**
 * Class AnimalController
 * Controller for animal form.
 * @author Jan Pavlica
 *
 */
public class AnimalController implements Initializable{

	private Animal myAnimal;
	private Connection dbConnection;
	private ZvireEntity zEnt;
	private Image pic = null;
	private File imgFile = null;
	Map<String, Integer> mapDruh = new HashMap<String, Integer>();

	@FXML
	private Label zPohlavi;
	@FXML
	private DatePicker datePickerDeleteTo;
	@FXML
	private DatePicker datePickerDeleteFrom;
	@FXML
	private Button btnDeleteInterval;

	@FXML
	private TextField ediJmeno;

	@FXML
	private Button btnEdit;

	@FXML
	private ChoiceBox<String> editPohlavi;

	@FXML
	private TableView<HistorieVybehu> historieTable;

	@FXML
	private TableColumn<HistorieVybehu, String> cDo;

	@FXML
	private Label zDatumNarozeni;

	@FXML
	private TableColumn<HistorieVybehu, String> cOd;

	@FXML
	private Label zJmeno;

	@FXML
	private DatePicker editDatumNarozeni;

	@FXML
	private Button btnStorno;

	@FXML
	private TextField txtFieldX;

	@FXML
	private Button removePhoto;

	@FXML
	private TextField txtFieldY;

	@FXML
	private Button uploadPhotoFromFile;

	@FXML
	private Label zDatumUmrti;

	@FXML
	private Button btnResize;

	@FXML
	private Button btnFindSimiliar;

	@FXML
	private Label zDruh;

	@FXML
	private ImageView picture;

	@FXML
	private Button btnSave;

	@FXML
	private Label labelX;

	@FXML
	private DatePicker editDatumUmrti;

	@FXML
	private Label labelY;

	@FXML
	private TableColumn<HistorieVybehu, String> cPavlilon;

	@FXML
	private ChoiceBox<String> editDruh;




	/**
	 * Switching from edit to normal mode.
	 * @author Jan Pavlica
	 * @param off
	 */
	private void makeEditMode(Boolean off) {
		zPohlavi.setVisible(!off);
		zDatumNarozeni.setVisible(!off);
		zDatumUmrti.setVisible(!off);
		zJmeno.setVisible(!off);
		zDruh.setVisible(!off);

		btnEdit.setVisible(!off);
		btnFindSimiliar.setVisible(!off);

		ediJmeno.setVisible(off);
		editDatumNarozeni.setVisible(off);
		editDatumUmrti.setVisible(off);
		editDruh.setVisible(off);
		editPohlavi.setVisible(off);

		btnResize.setVisible(off);   	
		btnSave.setVisible(off);
		btnStorno.setVisible(off);
		labelX.setVisible(off);
		labelY.setVisible(off);
		txtFieldX.setVisible(off);
		txtFieldY.setVisible(off);
		uploadPhotoFromFile.setVisible(off);
		removePhoto.setVisible(off);
	}

	/**
	 * From string to local date in one step
	 * @author Jan Pavlica
	 * @param dateString
	 * @return localDate
	 */
	public static final LocalDate LOCAL_DATE (String dateString){
		DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
		LocalDate localDate = LocalDate.parse(dateString, formatter);
		return localDate;
	}


	/**
	 * Handle edit button
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnEdit(ActionEvent event) {
		makeEditMode(true);

		ZvireModel zModel = new ZvireModel(dbConnection);
		DruhZvireteModel dModel = new DruhZvireteModel(dbConnection);


		ZvireEntity zEnt = zModel.GetZvireById(myAnimal.getAnimalId());

		List<DruhZvireteEntity> typy = dModel.GetAllDruhZvirete();
		ObservableList<String> optionsTyp = FXCollections.observableArrayList();

		for(DruhZvireteEntity pom : typy) {
			optionsTyp.add(pom.jmenoDruhu);
			mapDruh.put(pom.jmenoDruhu,pom.id);

		}
		editDruh.setItems(optionsTyp);
		editDruh.getSelectionModel().select(myAnimal.getDruh());

		ediJmeno.setText(myAnimal.getJmeno());

		editPohlavi.getItems().addAll("samice", "samec");
		editPohlavi.getSelectionModel().select(myAnimal.getPohlavi());


		editDatumNarozeni.setValue(LOCAL_DATE(zEnt.datumNarozeni.toString()));
		if(zEnt.datumUmrti != null)
			editDatumNarozeni.setValue(LOCAL_DATE(zEnt.datumUmrti.toString()));

	}

	/**
	 * Handle save button. Updates new information.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnSave(ActionEvent event) {
		ZvireModel zModel = new ZvireModel(dbConnection);
		if(imgFile != null)
			zModel.UploadPhoto(myAnimal.getAnimalId(), imgFile.getAbsolutePath());

		ZvireEntity zEnOrig = zModel.GetZvireById(myAnimal.getAnimalId());
		ZvireEntity zEn = new ZvireEntity(
				myAnimal.getAnimalId(),
				mapDruh.get(editDruh.getSelectionModel().getSelectedItem()),
				ediJmeno.getText(),
				editPohlavi.getSelectionModel().getSelectedItem(),
				Date.from(editDatumNarozeni.getValue().atStartOfDay(ZoneId.systemDefault()).toInstant()),
				editDatumUmrti.getValue() == null ? null : Date.from(editDatumUmrti.getValue().atStartOfDay(ZoneId.systemDefault()).toInstant()),
						zEnOrig.datumPorizeniFotky,
						zEnOrig.fotka
				);

		zModel.UpdateAnimalInfo(zEn);

		DruhZvireteModel dModel = new DruhZvireteModel(dbConnection);
		this.zEnt = zModel.GetZvireById(myAnimal.getAnimalId());

		try {
			zDruh.setText(dModel.DruhZvireteById(zEnt.idDruh).jmenoDruhu);
			zJmeno.setText(zEnt.jmeno);
			zPohlavi.setText(zEnt.pohlavi);
			zDatumNarozeni.setText(zEnt.datumNarozeni == null ? " " : zEnt.datumNarozeni.toString());
			zDatumUmrti.setText(zEnt.datumUmrti == null ? " " : zEnt.datumUmrti.toString());

			Image im = new Image(new ByteArrayInputStream(zEnt.fotka.getDataInByteArray()));
			txtFieldY.setText(String.valueOf(zEnt.fotka.getHeight()));
			txtFieldX.setText(String.valueOf(zEnt.fotka.getWidth()));

			picture.setImage(im);

		} catch(Exception e) {
		}


		makeEditMode(false);

	}

	/**
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnStorno(ActionEvent event) {
		makeEditMode(false);

	}


	/**
	 * Handle button for scaling photo.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnResize(ActionEvent event) {
		ZvireModel zModel = new ZvireModel(dbConnection);
		zModel.ScalePhoto(myAnimal.getAnimalId(),Integer.parseInt(txtFieldX.getText()),Integer.parseInt(txtFieldY.getText()));
	}

	/**
	 * Handle button for finding similiar photos.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnFindSimiliar(ActionEvent event) {
		ZvireModel zModel = new ZvireModel(dbConnection);
		DruhZvireteModel dModel = new DruhZvireteModel(dbConnection);

		int id = zModel.GetTheMostSimilar(myAnimal.getAnimalId());
		try {
			ZvireEntity choosenAnimal = zModel.GetZvireById(id);

			Animal animal = new Animal(
					dModel.DruhZvireteById(choosenAnimal.idDruh).jmenoDruhu,
					choosenAnimal.jmeno,
					choosenAnimal.pohlavi,
					choosenAnimal.datumNarozeni.toString(),
					id);

			FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("/view/ZvireView.fxml"));
			Parent root1 = (Parent) fxmlLoader.load();
			AnimalController controller = fxmlLoader.getController();
			controller.setMyAnimal(animal);
			controller.setDbConnection(dbConnection);
			controller.feedAnimal();
			Stage stage = new Stage();
			stage.setTitle("Zvíře");
			stage.setScene(new Scene(root1));  
			stage.show();
		} catch(Exception e) {
			System.out.println(e);
		}
	}

	/**
	 * Handle button for removing photo.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnRemovePhoto(ActionEvent event) {
		ZvireModel zModel = new ZvireModel(dbConnection);
		zModel.RemovePhoto(myAnimal.getAnimalId());
		picture.setImage(null);

	}



	public void initialize(URL url, ResourceBundle rb) {
		cPavlilon.setCellValueFactory(
				new PropertyValueFactory<HistorieVybehu,String>("pavilonJmeno")
				);
		cOd.setCellValueFactory(
				new PropertyValueFactory<HistorieVybehu,String>("odDatum")
				);
		cDo.setCellValueFactory(
				new PropertyValueFactory<HistorieVybehu,String>("doDatum")
				);

	}


	/**
	 * Init all data in form.
	 * @author Jan Pavlica
	 */
	public void feedAnimal() {
		ZvireModel zModel = new ZvireModel(dbConnection);
		DruhZvireteModel dModel = new DruhZvireteModel(dbConnection);
		this.zEnt = zModel.GetZvireById(myAnimal.getAnimalId());
		try {
			zDruh.setText(dModel.DruhZvireteById(zEnt.idDruh).jmenoDruhu);
			zJmeno.setText(zEnt.jmeno);
			zPohlavi.setText(zEnt.pohlavi);
			zDatumNarozeni.setText(zEnt.datumNarozeni == null ? " " : zEnt.datumNarozeni.toString());
			zDatumUmrti.setText(zEnt.datumUmrti == null ? " " : zEnt.datumUmrti.toString());

			Image im = new Image(new ByteArrayInputStream(zEnt.fotka.getDataInByteArray()));
			txtFieldY.setText(String.valueOf(zEnt.fotka.getHeight()));
			txtFieldX.setText(String.valueOf(zEnt.fotka.getWidth()));

			picture.setImage(im);

		} catch(Exception e) {
		}

		final ObservableList<HistorieVybehu> data = FXCollections.observableArrayList();


		historieTable.setEditable(true);

		List<HistorieUmisteni> allUm = zModel.GetAnimalEnclosureHistory(zEnt.id);

		for(HistorieUmisteni pom : allUm) {

			data.add(new HistorieVybehu(
					pom.jmenoVybehu,
					pom.umisteniOd.toString(),
					pom.umisteniDo == null ? "" : pom.umisteniDo.toString()
					) 
					);

		}

		historieTable.setItems(data);
	}


	/** 
	 * Gets file for photo from disk.
	 * @author Jan Pavlica
	 * @param action
	 */
	public void handleBtnUploadPhotoFromFile(ActionEvent action) {
		FileChooser fileChooser = new FileChooser();
		fileChooser.setTitle("Open Resource File");
		Stage stage = new Stage();
		imgFile = fileChooser.showOpenDialog(stage);
		if (imgFile != null) {
			pic = new Image(imgFile.toURI().toString());
			picture.setImage(pic);
		}
	}

	public void handleBtnDeleteInterval(ActionEvent action) {
		if(datePickerDeleteFrom.getValue()  != null && datePickerDeleteTo.getValue()  != null)
		{
			ZvireModel zModel = new ZvireModel(dbConnection);
			DateTimeFormatter dateFormatter = DateTimeFormatter.ofPattern("yyyy/MM/dd");

			zModel.DeleteTimeInterval(
					myAnimal.getAnimalId(),
					dateFormatter.format(datePickerDeleteFrom.getValue()).toString(),
					dateFormatter.format(datePickerDeleteTo.getValue()).toString()
					);

			final ObservableList<HistorieVybehu> data = FXCollections.observableArrayList();
			historieTable.setEditable(true);
			List<HistorieUmisteni> allUm = zModel.GetAnimalEnclosureHistory(zEnt.id);

			for(HistorieUmisteni pom : allUm) {

				data.add(new HistorieVybehu(
						pom.jmenoVybehu,
						pom.umisteniOd.toString(),
						pom.umisteniDo == null ? "" : pom.umisteniDo.toString()
						) 
						);

			}
			historieTable.setItems(data);
		}
	}

	/**
	 * Getter for myAnimal.
	 * @author Jan Pavlica
	 * @return myAnimal
	 */
	public Animal getMyAnimal() {
		return myAnimal;
	}


	/**
	 * Setter for myAnimal.
	 * @author Jan Pavlica
	 * @param myAnimal
	 */
	public void setMyAnimal(Animal myAnimal) {
		this.myAnimal = myAnimal;
	}

	public Connection getDbConnection() {
		return dbConnection;
	}

	public void setDbConnection(Connection dbConnection) {
		this.dbConnection = dbConnection;
	}

}

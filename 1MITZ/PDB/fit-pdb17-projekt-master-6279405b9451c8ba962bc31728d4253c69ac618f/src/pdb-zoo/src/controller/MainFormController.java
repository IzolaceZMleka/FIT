package controller;
import java.net.URL;
import java.sql.Connection;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.ResourceBundle;

import gui.Animal;
import gui.Zamestnanec;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.control.DatePicker;
import javafx.scene.control.Label;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableRow;
import javafx.scene.control.TableView;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.effect.DropShadow;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Shape;
import javafx.stage.Stage;
import model.DruhZvireteModel;
import model.TypModel;
import model.VybehModel;
import model.ZamestnanecModel;
import model.ZooModel;
import model.ZvireModel;
import model.entity.TypEntity;
import model.entity.VybehEntity;
import model.entity.ZamestnanecEntity;
import model.entity.ZooEntity;
import model.entity.ZvireEntity;
import oracle.spatial.geometry.JGeometry;
import shapes.Anchor;
import shapes.Draggable;
import shapes.ZOOCircle;
import shapes.ZOOPointCluster;
import shapes.ZOOPolygon;
import shapes.ZOOPolyline;
import shapes.ZOORectangle;


/**
 * Class MainFormController
 * Controller for main form which showing zoo map.
 * @author IzolaceZMleka
 *
 */
public class MainFormController implements Initializable {

	private Connection dbConnection;
	private  ZooModel zooModel;
	private  VybehModel vybehModel;
	private  TypModel typModel;
	private ZvireModel zvireModel;
	/**
	 * All generated and displayed shapes.
	 */
	private List<Shape> allShapes = new ArrayList<Shape>();
	private MouseGestures mg = new MouseGestures();

	/**
	 * Selected object on map.
	 */
	private Shape selectedZoo = null;
	/**
	 * Previously selected object on map used to comparing.
	 */
	private Shape prevSelectedZoo = null;
	/**
	 * New shape draws by user.
	 */
	private Shape newShape = null;
	private String newShapeTyp = null;
	/**
	 * God Mode for editing shapes.
	 */
	private Boolean godMod = false;
	/**
	 * Compare mode.
	 */
	private Boolean compareMod = false;
	/**
	 * List of changed shapes.
	 */
	private List<Shape> changedShapes = new ArrayList<Shape>();
	/**
	 * Hash set for avoiding duplicates.
	 */
	LinkedHashSet<Shape> changedShapesDuplicates = new LinkedHashSet<Shape>();
	/**
	 * Mapping animal names to IDs.
	 */
	Map<String, Integer> map = new HashMap<String, Integer>();
	/**
	 * Selected mode for showing history/present animals in enclosure.
	 */
	private Boolean enHistory = false;
	/**
	 * Variable storing ZOO ID for new created point cluster.
	 */
	private Integer pomID = null;
	/**
	 * Must have variable. Destiny of this application depends on it!
	 */
	private List<Shape> needThisVarible = null;

	@FXML
	private Button btnAddAnimal;
	@FXML
	private Button enHistoryBtn;
	@FXML
	private DatePicker datePickerDo;
	@FXML
	private DatePicker datePickerOd;
	@FXML
	private Label label;
	@FXML
	private  TextField vybehNazev;
	@FXML
	private  TextArea vybehInfo;
	@FXML
	private  Label objektID;
	@FXML
	private Button btnGetAreasCount;
	@FXML
	private Canvas canvas;
	@FXML
	private AnchorPane pane;
	@FXML 
	private Button edit;
	@FXML 
	private Button add;
	@FXML 
	private Button save;
	@FXML 
	private Button btnDelete;
	@FXML 
	private ChoiceBox<String> idVybehuBox;
	@FXML 
	private ChoiceBox<String> idTypuBox;
	@FXML 
	private TableView<Animal> tableZvirata;
	@FXML 
	private TableColumn<Animal, String> tableDruh;
	@FXML 
	private TableColumn<Animal, String> tableJmeno;
	@FXML 
	private TableColumn<Animal, String> tablePohlavi;
	@FXML 
	private TableColumn<Animal, String> tableDatumNarozeni;
	@FXML 
	private TableColumn<Zamestnanec, String> zamID;
	@FXML 
	private TableColumn<Zamestnanec, String> zamJmeno;
	@FXML 
	private TableColumn<Zamestnanec, String> zamPrijmeni;
	@FXML 
	private TableColumn<Zamestnanec, String> zamLogin;
	@FXML 
	private TableView<Zamestnanec> tableZam;
	@FXML 
	private TabPane tabPane;
	@FXML
	private Label redObvodDotyk;
	@FXML
	private Label labelObvodPrunik;
	@FXML
	private Label resObvodPrunik;
	@FXML
	private TextField inDistance;
	@FXML
	private Button btnChangeToCompare;
	@FXML
	private Label labelPlochaDotyk;
	@FXML
	private Button changeToHighlightClose;
	@FXML
	private Label labelDistance;
	@FXML
	private Button btnNormalMod;
	@FXML
	private Label resDistance;
	@FXML
	private TextField editYearForAnimalCount1;
	@FXML
	private TextField editYearForAnimalCount;
	@FXML
	private Label labelAnimalCount2;
	@FXML
	private Label labelAnimalCount;
	@FXML
	private ChoiceBox<String> choiceGender;
	@FXML
	private AnchorPane tab;


	/**
	 * Method for creating DbConnection and models.
	 * @author Jan Pavlica
	 * @param dbConnection
	 */
	public void setDbConnection(Connection dbConnection) {
		this.dbConnection = dbConnection;
		this.zooModel = new ZooModel(dbConnection);
		this.vybehModel = new VybehModel(dbConnection);
		this.typModel = new TypModel(dbConnection);
		this.zvireModel = new ZvireModel(dbConnection);

	}

	/**
	 * Method for udating zoo map.
	 * @author Jan Pavlica
	 */
	public void makeMap() {


		tabPane.getSelectionModel().selectedItemProperty().addListener(
				new ChangeListener<Tab>() {
					public void changed(ObservableValue<? extends Tab> ov, Tab t, Tab t1) {
						if(tabPane.getSelectionModel().getSelectedIndex() == 1 && selectedZoo != null)
						{
							initTable(((Draggable)selectedZoo).getIdVybeh());
							((Draggable) selectedZoo).setAnchorsVisible(false);
							mg.makeUndraggable(selectedZoo);
							idVybehuBox.setVisible(false);
							idTypuBox.setVisible(false);
							godMod = false;
							save.setVisible(false);
						}

					}
				}
				);

		TypModel typModel = new TypModel(dbConnection);
		List<ZooEntity> areas = zooModel.GetAllArea();
		for(ZooEntity pom : areas) {
			drawObject(pom);
		}
		List<TypEntity> typy = typModel.GetAllTyp();
		ObservableList<String> optionsTyp = FXCollections.observableArrayList();
		for(TypEntity pom : typy) {
			optionsTyp.add(pom.jmeno);
		}

		idTypuBox.setItems(optionsTyp);

		List<VybehEntity> vybehy = vybehModel.GetAllVybeh();
		ObservableList<String> optionsVybeh = FXCollections.observableArrayList();
		optionsVybeh.add(" ");

		for(VybehEntity pom : vybehy) {
			optionsVybeh.add(pom.jmeno);
		}

		choiceGender.getItems().addAll("samice", "samec");
		choiceGender.getSelectionModel().select("samec");

		choiceGender.getSelectionModel().selectedIndexProperty().addListener(new ChangeListener<Number>() {
			@Override
			public void changed(ObservableValue<? extends Number> observableValue, Number number, Number number2) {

				ChangeCount(choiceGender.getItems().get((int) number2));
			}
		});

		idTypuBox.getSelectionModel().selectedIndexProperty().addListener(new ChangeListener<Number>() {
			@Override
			public void changed(ObservableValue<? extends Number> observableValue, Number number, Number number2) {
				if(selectedZoo != null) {
					((Draggable)selectedZoo).setIdTyp(typModel.GetTypEntityByName(idTypuBox.getItems().get((Integer) number2)).id);
					if(selectedZoo instanceof ZOOPolyline)
						selectedZoo.setStroke(typModel.GetTypById(((Draggable)selectedZoo).getIdTyp()).barva.deriveColor(1.0, 1.0, 1.0, 0.8));
					else
						selectedZoo.setFill(typModel.GetTypById(((Draggable)selectedZoo).getIdTyp()).barva.deriveColor(1.0, 1.0, 1.0, 0.8));
				}


			}
		});

		idVybehuBox.setItems(optionsVybeh);
		
		
		final ObservableList<Zamestnanec> data = FXCollections.observableArrayList();
		tableZam.setEditable(true);

		ZamestnanecModel zModel = new ZamestnanecModel(dbConnection);


		List<ZamestnanecEntity> allUm = zModel.GetAllZamestnanec();
		
		
		for(ZamestnanecEntity zam : allUm) {

			Zamestnanec newZam = new Zamestnanec(
					String.valueOf(zam.id),
					zam.jmeno,
					zam.prijmeni,
					zam.login
					);

			data.add(newZam);

		}


		tableZam.setItems(data);

	}

	/**
	 * Sets IDs for created shape.
	 * @author Jan Pavlica
	 * @param idZoo - ZOO ID
	 * @param idVybeh - Eclosure ID
	 * @param idTyp - Type ID
	 * @param zooShape - Shape representing object in map
	 */
	private void setStuff(int idZoo, Integer idVybeh, Integer idTyp, Draggable zooShape) {
		zooShape.setIdTyp(idTyp);
		zooShape.setIdVybeh(idVybeh);
		zooShape.setIdZoo(idZoo);
	}

	/**
	 * Draws ZooEntity on pane.
	 * @author Jan Pavlica
	 * @param actual - ZooEntity to draw
	 */
	private void drawObject(ZooEntity actual) {
		double[] coords = actual.prostorovaData.getOrdinatesArray();
		int[] info = actual.prostorovaData.getElemInfo();
		TypModel typModel = new TypModel(dbConnection);

		Color fillWith = typModel.GetTypById(actual.idTyp).barva;
		fillWith = fillWith.deriveColor(1.0, 1.0, 1.0, 0.8);
		//System.out.println(fillWith.toString());

		final int len = coords.length;

		final Double[] boxedArray = new Double[len];

		for (int index = 0; index < len; index++)
			boxedArray[index] = 5 * Double.valueOf(coords[index]);

		if(info[1] == 2)
		{
			ZOOPolyline  mline = new ZOOPolyline ();
			setStuff(actual.id, actual.idVybehu, actual.idTyp, mline);
			mline.setStroke(fillWith);
			mline.setIdZoo(actual.id);
			mline.setStrokeWidth(8.00);
			mline.getPoints().addAll(boxedArray);
			pane.getChildren().addAll(mline);
			allShapes.add(mline);
			pane.getChildren().addAll((mline.createControlAnchorsFor(mline.getPoints())));
			mline.setId(String.valueOf(actual.id));
			mg.makeClickable(mline);
		}
		else if(info[1] == 1)
		{

			for (int index = 0; index < len; index = index + 2)
			{
				ZOOPointCluster pCluster = new ZOOPointCluster(coords[index]*5, coords[index+1]*5, 5.0);
				pCluster.setStroke(Color.BLACK);
				setStuff(actual.id, actual.idVybehu, actual.idTyp, pCluster);
				pCluster.setIdZoo(actual.id);
				pCluster.setFill(fillWith);
				pCluster.setId(String.valueOf(actual.id));
				pane.getChildren().addAll(pCluster);
				allShapes.add(pCluster);
				mg.makeClickable(pCluster);
			}

		}
		else if(info[1] == 1003)
		{
			switch(info[2]) {
			case 1:
				ZOOPolygon polygon = new ZOOPolygon();
				setStuff(actual.id, actual.idVybehu, actual.idTyp, polygon);
				polygon.setIdZoo(actual.id);
				polygon.setStroke(Color.BLACK);
				polygon.setFill(fillWith);
				polygon.getPoints().addAll(boxedArray);
				polygon.setId(String.valueOf(actual.id));
				pane.getChildren().addAll(polygon);
				allShapes.add(polygon);
				pane.getChildren().addAll((polygon.createControlAnchorsFor(polygon.getPoints())));
				mg.makeClickable(polygon);
				break;
			case 3:
				ZOORectangle rect =  new ZOORectangle(coords[0]*5,coords[1]*5,coords[2]*5-coords[0]*5,coords[3]*5-coords[1]*5);
				rect.setStroke(Color.BLACK);
				setStuff(actual.id, actual.idVybehu, actual.idTyp,rect);
				rect.setIdZoo(actual.id);
				rect.setFill(fillWith);
				pane.getChildren().addAll(rect);
				allShapes.add(rect);
				pane.getChildren().addAll((rect.createControlAnchorsFor(rect.getPoints())));
				rect.setId(String.valueOf(actual.id));
				mg.makeClickable(rect);
				break;
			case 4:
				ZOOCircle circ = new ZOOCircle(coords[0]*5, coords[3]*5,((coords[5] - coords[1])/2)*5);
				circ.setStroke(Color.BLACK);
				setStuff(actual.id, actual.idVybehu, actual.idTyp, circ);
				circ.setIdZoo(actual.id);
				circ.setFill(fillWith);
				circ.setId(String.valueOf(actual.id));
				pane.getChildren().addAll(circ);
				pane.getChildren().addAll((circ.createControlAnchorsFor(circ.getPoints())));
				allShapes.add(circ);
				mg.makeClickable(circ);
				break;
			default:
			}
		}

	}

	/**
	 * Highlight selected object on map and display informations about it.
	 * @author Jan Pavlica
	 */
	private void changeZoo() {

		int id = ((Draggable)selectedZoo).getIdZoo();

		objektID.setText(selectedZoo.getId());

		DropShadow borderGlow= new DropShadow();
		borderGlow.setOffsetY(0f);
		borderGlow.setOffsetX(0f);
		borderGlow.setColor(Color.FUCHSIA);
		borderGlow.setWidth(100);
		borderGlow.setHeight(100);

		selectedZoo.toBack();

		if(godMod)
		{
			if (changedShapesDuplicates.add(selectedZoo))
				changedShapes.add(selectedZoo);

			makeEditable(selectedZoo);

			idTypuBox.getSelectionModel().select(typModel.GetTypById(((Draggable)selectedZoo).getIdTyp()).jmeno);

			try {
				idVybehuBox.getSelectionModel().select(vybehModel.GetVybehById(((Draggable)selectedZoo).getIdVybeh()).jmeno);


			}
			catch(Exception e) {
				idVybehuBox.getSelectionModel().select(" ");
			}

			String sTyp = idTypuBox.getSelectionModel().getSelectedItem();
			String sVybeh = idVybehuBox.getSelectionModel().getSelectedItem();	

			((Draggable)selectedZoo).setIdTyp(typModel.GetTypEntityByName(sTyp).id);
			((Draggable)selectedZoo).setIdVybeh(sVybeh == " " ? null : vybehModel.GetVybehEntityByName(sVybeh).id);


		}

		if(compareMod && prevSelectedZoo != null) {
			resDistance.setText(String.format( "%.2f m", zooModel.GetZooObjectsDistance(((Draggable)prevSelectedZoo).getIdZoo(), ((Draggable)selectedZoo).getIdZoo())));
			resObvodPrunik.setText(zooModel.ZooObjectContainsZooObject((((Draggable)prevSelectedZoo).getIdZoo()), ((Draggable)selectedZoo).getIdZoo()) ? "ANO" : "NE");
			redObvodDotyk.setText(zooModel.Intersection(((Draggable)prevSelectedZoo).getIdZoo(), ((Draggable)selectedZoo).getIdZoo()) ? "ANO" : "NE");
		}
		else {
			redObvodDotyk.setText(String.format( "%.2f m^2", zooModel.GetZooArea(id) ));
			resObvodPrunik.setText(String.format( "%.2f m", zooModel.GetZooLength(id) ));
		}


		ZooEntity eZoo = zooModel.GetAreaById( id );
		selectedZoo.setEffect(borderGlow);
		if(tabPane.getSelectionModel().getSelectedIndex() == 1)
			initTable(((Draggable)selectedZoo).getIdVybeh());

		ChangeCount(choiceGender.getSelectionModel().getSelectedItem());
		id = eZoo.idVybehu;

		if(id != 0) {
			vybehNazev.setText((vybehModel.GetVybehById(id)).jmeno);
			vybehInfo.setText((vybehModel.GetVybehById(id)).info);



		}
		else
		{
			switch(eZoo.idTyp) {
			case 1:
				vybehNazev.setText("Strom");
				vybehInfo.setText("O tomto stromu nejsou známy další informace.");
				break;
			case 2:
				vybehNazev.setText("Trávník");
				vybehInfo.setText("O tomto místě nejsou známy další informace.");
				break;
			case 3:
				vybehNazev.setText("Cesta");
				vybehInfo.setText("O této cestě nejsou známy další informace.");
				break;
			case 4:
				vybehNazev.setText("Voda");
				vybehInfo.setText("O této vodní ploše nejsou známy další informace.");
				break;
			case 5:
				vybehNazev.setText("Kámen");
				vybehInfo.setText("O tomto objektu nejsou známy další informace.");
				break;
			case 6:
				vybehNazev.setText("Budova");
				vybehInfo.setText("O této budově nejsou známy další informace.");
				break;

			default:
				vybehNazev.setText("PLACEHOLDER");
				vybehInfo.setText("PLACEHOLDER");

			}

		}
	}

	/**
	 * Displays number of animals in selected object.
	 * @author Jan Pavlica
	 * @param pohlavi - gender of animal
	 */
	private void ChangeCount(String pohlavi) {

		if(selectedZoo != null) {
			Integer id = ((Draggable)selectedZoo).getIdVybeh();

			if(id != null) {
				labelAnimalCount.setText(String.valueOf(zvireModel.AnimalCountInEnclosureInYear(
						editYearForAnimalCount.getText(),
						id)));

				labelAnimalCount2.setText(String.valueOf(zvireModel.GetAnimalCountByGenderAndYearInEnclosure(
						id,
						editYearForAnimalCount1.getText(),
						pohlavi
						)));
			}
			else
			{
				labelAnimalCount.setText("0");
				labelAnimalCount2.setText("0");
			}
		}



	}


	/**
	 * Class MouseGestures
	 * Class responsible for drag and click events on object.
	 * @author Jan Pavlica
	 *
	 */
	public class MouseGestures {

		double orgSceneX, orgSceneY;
		double orgTranslateX, orgTranslateY;
		Shape lastClicked = null;

		public void makeClickable(Node node) {
			node.setOnMousePressed(circleOnMousePressedEventHandler);
		}

		public void makeDraggable(Node node) {
			node.setOnMouseDragged(circleOnMouseDraggedEventHandler);
		}

		public void makeUndraggable(Node node) {
			node.setOnMouseDragged(null);
		}

		EventHandler<MouseEvent> circleOnMousePressedEventHandler = new EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent t) {

				clearMapEffects();

				orgSceneX = t.getSceneX();
				orgSceneY = t.getSceneY();

				Node p = ((Node) (t.getSource()));

				orgTranslateX = p.getTranslateX();
				orgTranslateY = p.getTranslateY();

				if(selectedZoo != null )
				{
					makeUneditable(selectedZoo);
					selectedZoo.setEffect(null);
				}
				prevSelectedZoo = selectedZoo;
				selectedZoo = (Shape) t.getSource();

				changeZoo();


			}


		};

		EventHandler<MouseEvent> circleOnMouseDraggedEventHandler = new EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent t) {

				double offsetX = t.getSceneX() - orgSceneX;
				double offsetY = t.getSceneY() - orgSceneY;

				double newTranslateX = orgTranslateX + offsetX;
				double newTranslateY = orgTranslateY + offsetY;
				Node p = ((Node) (t.getSource()));
				((Draggable) p).moveAnchors(newTranslateX,newTranslateY);

				p.setTranslateX(newTranslateX);
				p.setTranslateY(newTranslateY);


			}
		};

	}

	/**
	 * Method creates JGeomtry from ZOOShape object.
	 * @author Jan Pavlica
	 * @param zoo - Shape of object
	 * @return JGeomtry of ZOOShape
	 */
	private JGeometry getZoo(Shape zoo) {
		if(zoo instanceof ZOORectangle) {
			ZOORectangle rect = (ZOORectangle) zoo;


			return new JGeometry(
					(rect.getX() + rect.getTranslateX())/5,
					(rect.getY() + rect.getTranslateY())/5,
					(rect.getX() + rect.getWidth()+rect.getTranslateX())/5,
					(rect.getY() + rect.getHeight()+rect.getTranslateY())/5,
					0);

		}
		else if(zoo instanceof ZOOCircle)
		{
			ZOOCircle circ = (ZOOCircle) zoo;


			return JGeometry.createCircle(
					(circ.getCenterX()+circ.getTranslateX())/5,
					((circ.getCenterY()+circ.getTranslateY())/5)-(circ.getRadius()/5),
					(circ.getCenterX()+circ.getTranslateX())/5+(circ.getRadius()/5),
					(circ.getCenterY()+circ.getTranslateY())/5,
					(circ.getCenterX()+circ.getTranslateX())/5,
					((circ.getCenterY()+circ.getTranslateY())/5)+(circ.getRadius()/5),
					0);
		}
		else if(zoo instanceof ZOOPolygon)
		{

			ZOOPolygon polygon = (ZOOPolygon) zoo;
			ObservableList<Double> coords = polygon.getPoints();
			final int len = coords.size();
			double[] coordsFixed = new double[len];

			for (int index = 0; index < len; index++)
			{
				if(index % 2 == 0)
					coordsFixed[index] = (coords.get(index) + polygon.getTranslateX()) /5;
				else
					coordsFixed[index] = (coords.get(index) + polygon.getTranslateY()) /5;
			}

			return JGeometry.createLinearPolygon(coordsFixed, 2, 0);

		}
		else if(zoo instanceof ZOOPolyline)
		{
			ZOOPolyline polyline = (ZOOPolyline) zoo;
			ObservableList<Double> coords = polyline.getPoints();
			final int len = coords.size();
			double[] coordsFixed = new double[len];

			for (int index = 0; index < len; index++)
			{
				if(index % 2 == 0)
					coordsFixed[index] = (coords.get(index) + polyline.getTranslateX()) /5;
				else
					coordsFixed[index] = (coords.get(index) + polyline.getTranslateY()) /5;
			}

			return JGeometry.createLinearLineString(coordsFixed, 2, 0);
		}
		else if(zoo instanceof ZOOPointCluster)
		{
			List<Double> listPm = new ArrayList<Double>();

			if(needThisVarible == null) {
				for(Shape shp : allShapes)
				{
					System.out.println(((Draggable)shp).getIdZoo());
					if( ((Draggable)shp).getIdZoo() == ((Draggable)zoo).getIdZoo())
					{
						listPm.add((((ZOOPointCluster) shp).getCenterX() + ((ZOOPointCluster) shp).getTranslateX())/5);
						listPm.add((((ZOOPointCluster) shp).getCenterY() + ((ZOOPointCluster) shp).getTranslateY())/5);
					}
				}
			}
			else
			{
				for(Shape shp : needThisVarible)
				{
					listPm.add((((ZOOPointCluster) shp).getCenterX() + ((ZOOPointCluster) shp).getTranslateX())/5);
					listPm.add((((ZOOPointCluster) shp).getCenterY() + ((ZOOPointCluster) shp).getTranslateY())/5);
				}
			}
			needThisVarible = null;
			final int len = listPm.size();
			double[] coordsFixes = new double[len];

			for (int index = 0; index < len; index++)
				coordsFixes[index] = listPm.get(index);

			int[] x = {1,1,len/2};



			return new JGeometry(2005, 0, x, coordsFixes);
		}



		return null;

	}

	@Override
	public void initialize(URL url, ResourceBundle rb) {
		pane.setStyle("-fx-background-color: #d1a455");

		tableDruh.setCellValueFactory(
				new PropertyValueFactory<Animal,String>("druh")
				);
		tableJmeno.setCellValueFactory(
				new PropertyValueFactory<Animal,String>("jmeno")
				);
		tablePohlavi.setCellValueFactory(
				new PropertyValueFactory<Animal,String>("pohlavi")
				);
		tableDatumNarozeni.setCellValueFactory(
				new PropertyValueFactory<Animal,String>("datumNarozeni")
				);

		zamID.setCellValueFactory(
				new PropertyValueFactory<Zamestnanec,String>("ID")
				);
		zamJmeno.setCellValueFactory(
				new PropertyValueFactory<Zamestnanec,String>("jmeno")
				);
		zamPrijmeni.setCellValueFactory(
				new PropertyValueFactory<Zamestnanec,String>("prijmeni")
				);
		zamLogin.setCellValueFactory(
				new PropertyValueFactory<Zamestnanec,String>("login")
				);

		editYearForAnimalCount.textProperty().addListener((observable, oldValue, newValue) -> {
			ChangeCount(choiceGender.getSelectionModel().getSelectedItem());
		});

		editYearForAnimalCount1.textProperty().addListener((observable, oldValue, newValue) -> {
			ChangeCount(choiceGender.getSelectionModel().getSelectedItem());
		});


	}

	/**
	 * Method reacting to save button action. Saves updated objects to DB.
	 * @author Jan Pavlica
	 * @param action
	 */
	public void handlebtnGetAreasCountAction(ActionEvent action) {

		if(godMod && selectedZoo != null )
		{

			((Draggable) selectedZoo).setAnchorsVisible(false);
			mg.makeUndraggable(selectedZoo);
			idVybehuBox.setVisible(false);
			idTypuBox.setVisible(false);

			godMod = false;
			save.setVisible(false);
			btnDelete.setVisible(false);

			for(Shape zooToUpdate: changedShapes)
			{
				Draggable zoo = ((Draggable)zooToUpdate);
				zooModel.UpdateZoo( zoo.getIdZoo(),  zoo.getIdVybeh(), zoo.getIdTyp(), getZoo(zooToUpdate));
			}	
		}


	}


	/**
	 * Making shape editable.
	 * @author Jan Pavlica
	 * @param obj - Shape to edit.
	 */
	private void makeEditable(Shape obj) {
		obj.toFront();
		((Draggable) obj).setAnchorsVisible(true);
		mg.makeDraggable(obj);
	}

	/**
	 * Making shape uneditable.
	 * @author Jan Pavlica
	 * @param obj - Shape to not edit.
	 */
	private void makeUneditable(Shape obj) {
		((Draggable) obj).setAnchorsVisible(false);
		mg.makeUndraggable(obj);
		obj.toBack();
	}


	/**
	 * Method reacting to edit button action. Change to edit mode.
	 * @author Jan Pavlica
	 * @param action
	 */
	public void handlebtnGodMod(ActionEvent action) {
		if(selectedZoo != null )
		{
			btnDelete.setVisible(true);
			save.setVisible(true);
			idVybehuBox.setVisible(true);
			idTypuBox.setVisible(true);
			makeEditable(selectedZoo);
		}

		godMod = true;	

	}


	/** Init table and fill it with data based on selected ZOO.
	 * @author Jan Pavlica
	 * @param vybehId - enclosure ID
	 */
	public void initTable(int vybehId) {

		tableZvirata.setRowFactory( tv -> {
			TableRow<Animal> row = new TableRow<>();
			row.setOnMouseClicked(event -> {
				if (event.getClickCount() == 2 && (! row.isEmpty()) ) {
					Animal rowData = row.getItem();
					try {
						FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("/view/ZvireView.fxml"));
						Parent root1 = (Parent) fxmlLoader.load();
						AnimalController controller = fxmlLoader.getController();
						controller.setMyAnimal(rowData);
						controller.setDbConnection(dbConnection);
						controller.feedAnimal();
						Stage stage = new Stage();
						stage.setTitle(rowData.getDruh()+" - "+rowData.getJmeno());
						stage.setScene(new Scene(root1));  
						stage.show();
					} catch(Exception e) {
						System.out.println(e);
					}
				}
			});
			return row ;
		});

		final ObservableList<Animal> data = FXCollections.observableArrayList();


		tableZvirata.setEditable(true);

		ZvireModel zModel = new ZvireModel(dbConnection);
		DruhZvireteModel dModel = new DruhZvireteModel(dbConnection);

		if(!enHistory)
		{
			List<ZvireEntity> allUm = zModel.GetAllZvireInEnclosure(vybehId);
			for(ZvireEntity choosenAnimal : allUm) {

				Animal animal = new Animal(
						dModel.DruhZvireteById(choosenAnimal.idDruh).jmenoDruhu,
						choosenAnimal.jmeno,
						choosenAnimal.pohlavi,
						choosenAnimal.datumNarozeni.toString(),
						choosenAnimal.id);

				data.add(animal);


			}
		}
		else
		{
			List<Integer> historyAnimalId = zModel.GetEnclosureAnimalHistory(vybehId);

			for(Integer choosenAnimalId : historyAnimalId) {

				ZvireEntity choosenAnimal = zModel.GetZvireById(choosenAnimalId);

				Animal animal = new Animal(
						dModel.DruhZvireteById(choosenAnimal.idDruh).jmenoDruhu,
						choosenAnimal.jmeno,
						choosenAnimal.pohlavi,
						choosenAnimal.datumNarozeni.toString(),
						choosenAnimal.id);

				data.add(animal);


			}

		}	

		tableZvirata.setItems(data);
	}

	/** Delete button handler. Deletes selected shape.
	 * @author Jan Pavlica
	 * @param action
	 */
	public void handleBtnDelete(ActionEvent action) {

		if(selectedZoo != null) {
			zooModel.RemoveZoo(((Draggable)selectedZoo).getIdZoo());

			for( Anchor anch : ((Draggable)selectedZoo).getAnchors() )
			{
				pane.getChildren().remove(anch);
			}
			allShapes.remove(selectedZoo);
			pane.getChildren().remove(selectedZoo);
			selectedZoo = null;
		}

	}

	/** Change mod to comparing shapes.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnChangeToCompare(ActionEvent event) {
		btnNormalMod.setVisible(true);
		btnChangeToCompare.setVisible(false);
		labelDistance.setVisible(true);
		resDistance.setVisible(true);
		labelObvodPrunik.setText("Je obsažen:");
		labelPlochaDotyk.setText("Dotýká se:");
		edit.setVisible(false);
		compareMod = true;

	}

	/** Change mod to normal.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnNormalMod(ActionEvent event) {
		btnNormalMod.setVisible(false);
		btnChangeToCompare.setVisible(true);
		resDistance.setVisible(false);
		labelDistance.setVisible(false);
		labelObvodPrunik.setText("Obvod objektu:");
		labelPlochaDotyk.setText("Obsah objektu:");
		edit.setVisible(true);
		compareMod = false;
	}


	/** Clears all effect on map.
	 * @author Jan Pavlica
	 */
	private void clearMapEffects() {

		for(Shape generatedShapes : allShapes)
		{

			if(!generatedShapes.equals(selectedZoo))
			{
				if(generatedShapes instanceof ZOOPolyline)
					generatedShapes.setStroke(typModel.GetTypById(((Draggable)generatedShapes).getIdTyp()).barva.deriveColor(1.0, 1.0, 1.0, 0.8));
				else
					generatedShapes.setStroke(Color.BLACK);

				generatedShapes.setEffect(null);
			}

		}
	}

	/**
	 * Making and adding new shapes to map.
	 * @author Jan Pavlica
	 * @param tvar - Type of shape to make.
	 */
	private void createShape(String tvar) {
		Pane platno = new Pane();
		platno.setPrefSize(500,500);
		pane.getChildren().addAll(platno);
		platno.toFront();
		tab.setVisible(false);
		idVybehuBox.setVisible(true);
		idTypuBox.setVisible(true);
		idTypuBox.getSelectionModel().select(1);
		idVybehuBox.getSelectionModel().select(" ");
		newShapeTyp = tvar;


		platno.setOnMouseClicked(e -> {
			if (e.getButton() == MouseButton.PRIMARY) {
				switch(newShapeTyp) {
				case "Polygon":
					if (newShape == null) {
						newShape = new ZOOPolygon();
						newShape.setStroke(Color.BLACK);
						platno.getChildren().add(newShape);
					}
					((ZOOPolygon)newShape).getPoints().addAll(e.getX(), e.getY());
					break;
				case "Shluk bodů":
					if(pomID == null)
					{
						needThisVarible = new ArrayList<Shape>();
						pomID = zooModel.generateNextId("zoo", "id");
					}

					newShape = new ZOOPointCluster(e.getX(), e.getY(), 5);
					newShape.setStroke(Color.BLACK);
					needThisVarible.add(newShape);
					((Draggable)newShape).setIdZoo(pomID);
					platno.getChildren().add(newShape);
					break;
				case "Kruh":
					if (newShape == null) {
						newShape = new ZOOCircle(e.getX(), e.getY(),50);
						newShape.setStroke(Color.BLACK);
						platno.getChildren().add(newShape);
					}
					else
					{
						platno.getChildren().remove(newShape);
						newShape = new ZOOCircle(e.getX(), e.getY(),50);
						newShape.setStroke(Color.BLACK);
						platno.getChildren().add(newShape);
					}
					break;
				case "Obdélník":
					if (newShape == null) {
						newShape = new ZOORectangle(e.getX(), e.getY(),50,50);
						newShape.setStroke(Color.BLACK);
						platno.getChildren().add(newShape);
					}
					else
					{
						platno.getChildren().remove(newShape);
						newShape = new ZOORectangle(e.getX(), e.getY(),50,50);
						newShape.setStroke(Color.BLACK);
						platno.getChildren().add(newShape);
					}
					break;
				case "Lomená čára":
					if (newShape == null) {
						newShape = new ZOOPolyline();
						newShape.setStroke(Color.BLACK);
						newShape.setStrokeWidth(10);
						platno.getChildren().add(newShape);
					}
					((ZOOPolyline)newShape).getPoints().addAll(e.getX(), e.getY());
					break;
				}

			} else {
				String sTyp = idTypuBox.getSelectionModel().getSelectedItem();
				String sVybeh = idVybehuBox.getSelectionModel().getSelectedItem();	

				int res = zooModel.generateNextId("zoo", "id");


				zooModel.InsertZoo(
						res,
						sVybeh == " " ? null : vybehModel.GetVybehEntityByName(sVybeh).id, 
								typModel.GetTypEntityByName(sTyp).id, 
								getZoo(newShape)
						);

				drawObject(zooModel.GetAreaById(res));

				newShape = null ;
				tab.setVisible(true);
				pane.getChildren().remove(platno);
				pomID = null;
				//platno.getChildren().clear();
			}
		});


	}

	/**
	 * Handle for add button. Adds new shape to map.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleAddBtn(ActionEvent event) {
		List<String> choices = new ArrayList<>();
		choices.add("Polygon");
		choices.add("Shluk bodů");
		choices.add("Kruh");
		choices.add("Obdélník");
		choices.add("Lomená čára");

		ChoiceDialog<String> dialog = new ChoiceDialog<>("Polygon", choices);
		dialog.setTitle("Nový útvar");
		dialog.setContentText("Vyberte tvar, který chcete vytvořit:");

		Optional<String> result = dialog.showAndWait();
		// The Java 8 way to get the response value (with lambda expression).

		if(result.isPresent())
		{
			createShape(result.get());
		}
		else
			return;	
	}

	/**
	 * Handle to showing close object button.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnChangeToHighlightClose(ActionEvent event) {

		int dist = Integer.parseInt(inDistance.getText());
		zooModel.GetAllZooCloserThen(((Draggable)selectedZoo).getIdZoo(), dist);

		DropShadow borderGlow = new DropShadow(100, Color.CRIMSON);

		clearMapEffects();

		for(ZooEntity zooInRange : zooModel.GetAllZooCloserThen(((Draggable)selectedZoo).getIdZoo(), dist))
		{
			for(Shape generatedShapes : allShapes)
			{
				if(zooInRange.id == ((Draggable)generatedShapes).getIdZoo())
				{
					generatedShapes.setEffect(borderGlow);
					generatedShapes.setStroke(Color.CRIMSON);
				}

			}
		}

	}


	/**
	 * Handle for history/present mod for showing animals in enclosure.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleEnHistoryBtn(ActionEvent event) {
		if(enHistory)
		{
			enHistory = false;
			initTable(((Draggable)selectedZoo).getIdVybeh());
			enHistoryBtn.setText("Zobraz historii zvířat ve výběhu");

		}
		else
		{
			enHistory = true;
			initTable(((Draggable)selectedZoo).getIdVybeh());
			enHistoryBtn.setText("Zobraz aktuální zvířata ve výběhu");
		}

	}


	/**
	 * Adding new animal in choosen enclosure.
	 * @author Jan Pavlica
	 * @param event
	 */
	@FXML
	void handleBtnAddAnimal(ActionEvent event) {
		if(selectedZoo != null && datePickerOd.getValue()  != null)
		{

			List<String> choices = new ArrayList<>();
			for(ZvireEntity zvire : zvireModel.GetAllZvire()) {
				choices.add(zvire.jmeno);
				map.put(zvire.jmeno,zvire.id);
			}

			ChoiceDialog<String> dialog = new ChoiceDialog<>(" " ,choices);
			dialog.setTitle("Nové zvíře");
			dialog.setContentText("Vyberte zvíře, který chcete vložit:");

			Optional<String> result = dialog.showAndWait();

			if(result.isPresent())
			{

				DateTimeFormatter dateFormatter = DateTimeFormatter.ofPattern("yyyy/MM/dd");


				zvireModel.UpdateUmisteniInInterval(
						map.get(result.get()),
						dateFormatter.format(datePickerOd.getValue()).toString(),
						datePickerDo.getValue() == null ? null : dateFormatter.format(datePickerDo.getValue()).toString(),
								((Draggable)selectedZoo).getIdVybeh());

				initTable(((Draggable)selectedZoo).getIdVybeh());
			}
			else
				return;	
		}

	}


}
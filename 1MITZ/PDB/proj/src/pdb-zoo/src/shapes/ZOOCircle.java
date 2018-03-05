package shapes;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;

/**
 * class ZOOCircle
 * Custom circle. 
 * @author Jan Pavlica
 *
 */
public class ZOOCircle extends Circle implements Draggable{

	private int idZoo;
	private Integer idTyp;
	private Integer idVybeh;

	ObservableList<Anchor> anchors;
	public ObservableList<Anchor> getAnchors() {
		return anchors;
	}

	public void setAnchors(ObservableList<Anchor> anchors) {
		this.anchors = anchors;
	}

	private ZOOCircle pom2;
	private double xRad, yRad;


	public ZOOCircle(double d, double e, double f) {
		super(d,e,f);
		pom2 = this;
		xRad = 0;
		yRad = 0;
	}

	public void setAnchorsVisible(boolean visibility) {
		for(Anchor a : anchors) {
			a.toFront();
			a.setVisible(visibility);
		}
	}

	public void moveAnchors(double newTranslateX, double newTranslateY)
	{
		for(Anchor a : anchors) {
			a.setTranslateX(newTranslateX);
			a.setTranslateY(newTranslateY);
		}
	}

	public ObservableList<Anchor> createControlAnchorsFor(final ObservableList<Double> points) {
		this.anchors = FXCollections.observableArrayList();

		for (int i = 0; i < points.size(); i+=2) {

			DoubleProperty xProperty = new SimpleDoubleProperty(points.get(i));
			DoubleProperty yProperty = new SimpleDoubleProperty(points.get(i + 1));


			xProperty.addListener(new ChangeListener<Number>() {
				@Override public void changed(ObservableValue<? extends Number> ov, Number oldX, Number x) {

					xRad = x.doubleValue() - pom2.getCenterX();

					double radius = Math.sqrt((xRad*xRad)+(yRad*yRad));
					pom2.setRadius(radius);
				}
			});

			yProperty.addListener(new ChangeListener<Number>() {
				@Override public void changed(ObservableValue<? extends Number> ov, Number oldY, Number y) {
					yRad = y.doubleValue() - pom2.getCenterY();

					double radius = Math.sqrt((xRad*xRad)+(yRad*yRad));
					pom2.setRadius(radius);
				}
			});

			anchors.add(new Anchor(Color.GOLD, xProperty, yProperty));
		}

		return anchors;
	}

	public ObservableList<Double> getPoints() {
		ObservableList<Double> list = FXCollections.observableArrayList();

		double x = getCenterX() + getRadius();
		double y = getCenterY() + getRadius();

		list.addAll(x, y);

		return list;
	}

	public int getIdZoo() {
		return idZoo;
	}

	public void setIdZoo(int idZoo) {
		this.idZoo = idZoo;
	}

	public Integer getIdTyp() {
		return idTyp;
	}

	public void setIdTyp(Integer idTyp) {
		this.idTyp = idTyp;
	}

	public Integer getIdVybeh() {
		return idVybeh;
	}

	public void setIdVybeh(Integer idVybeh) {
		this.idVybeh = idVybeh;
	}

}

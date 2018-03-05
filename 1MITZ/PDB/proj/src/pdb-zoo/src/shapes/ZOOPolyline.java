package shapes;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.paint.Color;
import javafx.scene.shape.Polyline;


/**
 * class ZOOPolyline
 * Custom polyline.
 * @author Jan Pavlica
 *
 */
public class ZOOPolyline extends Polyline implements Draggable{
	
	ObservableList<Anchor> anchors;
	private Integer idTyp;
	private Integer idVybeh;
	
	public ObservableList<Anchor> getAnchors() {
		return anchors;
	}

	public void setAnchors(ObservableList<Anchor> anchors) {
		this.anchors = anchors;
	}

	private int idZoo;
	
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
	      final int idx = i;

	      DoubleProperty xProperty = new SimpleDoubleProperty(points.get(i));
	      DoubleProperty yProperty = new SimpleDoubleProperty(points.get(i + 1));

	      xProperty.addListener(new ChangeListener<Number>() {
	        @Override public void changed(ObservableValue<? extends Number> ov, Number oldX, Number x) {
	          points.set(idx, x.doubleValue());
	        }
	      });

	      yProperty.addListener(new ChangeListener<Number>() {
	        @Override public void changed(ObservableValue<? extends Number> ov, Number oldY, Number y) {
	          points.set(idx + 1, y.doubleValue());
	        }
	      });

	      anchors.add(new Anchor(Color.GOLD, xProperty, yProperty));
	    }

	    return anchors;
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

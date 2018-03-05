package shapes;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

/**
 * class ZOORectangle
 * Custom rectangle.
 * @author Jan Pavlica
 *
 */
public class ZOORectangle extends Rectangle implements Draggable{
	
	ObservableList<Anchor> anchors;
	private ZOORectangle pom2;
	private int idZoo;
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

	private Integer idTyp;
	private Integer idVybeh;

	public ZOORectangle(double d, double e, double f, double g) {
		super(d,e,f,g);
		pom2 = this;
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
	      final int idx = i;

	      DoubleProperty xProperty = new SimpleDoubleProperty(points.get(i));
	      DoubleProperty yProperty = new SimpleDoubleProperty(points.get(i + 1));

	      xProperty.addListener(new ChangeListener<Number>() {
	        @Override public void changed(ObservableValue<? extends Number> ov, Number oldX, Number x) {
	        	if(idx == 0)
	        	{
	        		pom2.setWidth(pom2.getWidth() + pom2.getX() - x.doubleValue() );
	        		pom2.setX(x.doubleValue());
	        		
	        	}
	        	else
	        		pom2.setWidth(x.doubleValue()- pom2.getX());
	        }
	      });

	      yProperty.addListener(new ChangeListener<Number>() {
	        @Override public void changed(ObservableValue<? extends Number> ov, Number oldY, Number y) {
	        	if(idx == 0) 
	        	{
	        		pom2.setHeight(pom2.getHeight() + pom2.getY() - y.doubleValue() );
	        		pom2.setY(y.doubleValue());
	        	}
	        	else
	        		pom2.setHeight(y.doubleValue() - pom2.getY());
	        }
	      });

	      anchors.add(new Anchor(Color.GOLD, xProperty, yProperty));
	    }

	    return anchors;
	  }

	public ObservableList<Double> getPoints() {
		ObservableList<Double> list = FXCollections.observableArrayList();
		
		double x = getX();
		double y = getY();
		
		list.addAll(x, y, x + getWidth(), y + getHeight());
		
		return list;
	}

	public int getIdZoo() {
		return idZoo;
	}

	public void setIdZoo(int idZoo) {
		this.idZoo = idZoo;
	}

	public ObservableList<Anchor> getAnchors() {
		return anchors;
	}

	public void setAnchors(ObservableList<Anchor> anchors) {
		this.anchors = anchors;
	}

	  

}

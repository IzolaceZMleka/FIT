package shapes;

import javafx.collections.ObservableList;
import javafx.scene.shape.Circle;

/**
 * class ZOOPointCluster
 * Custom point cluster. 
 * @author Jan Pavlica
 *
 */
public class ZOOPointCluster extends Circle implements Draggable{
	
	private int idZoo;
	private Integer idTyp;
	private Integer idVybeh;
	
	
	ObservableList<Anchor> anchors;
	public ZOOPointCluster(double d, double e, double f) {
		super(d,e,f);
	}

	public ObservableList<Anchor> getAnchors() {
		return anchors;
	}

	public void setAnchors(ObservableList<Anchor> anchors) {
		this.anchors = anchors;
	}
	
	public void moveAnchors(double newTranslateX, double newTranslateY)
	{
	}
	
	public ObservableList<Anchor> createControlAnchorsFor(final ObservableList<Double> points) {
		return null;
	  }

	public ObservableList<Double> getPoints() {
		return null;
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

	@Override
	public void setAnchorsVisible(boolean visibility) {
	}

}

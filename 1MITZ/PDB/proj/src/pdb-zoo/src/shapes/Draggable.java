package shapes;

import javafx.collections.ObservableList;

/**
 * Interface Draggable
 * Interface for shapes with anchors
 * @author Jan Pavlica
 *
 */
public interface Draggable {

	/**
	 * @author Jan Pavlica
	 * @param visibility
	 */
	public void setAnchorsVisible(boolean visibility);
	/**
	 * @author Jan Pavlica
	 * @param newTranslateX
	 * @param newTranslateY
	 */
	public void moveAnchors(double newTranslateX, double newTranslateY);
	/**
	 * Creating control anchors
	 * @author Jan Pavlica
	 * @param points
	 * @return
	 */
	public ObservableList<Anchor> createControlAnchorsFor(final ObservableList<Double> points);
	/**
	 * @author Jan Pavlica
	 * @return
	 */
	public int getIdZoo();
	/**
	 * @author Jan Pavlica
	 * @param idZoo
	 */
	public void setIdZoo(int idZoo);
	/**
	 * @author Jan Pavlica
	 * @return
	 */
	public ObservableList<Anchor> getAnchors();
	/**
	 * @author Jan Pavlica
	 * @param anchors
	 */
	public void setAnchors(ObservableList<Anchor> anchors);
	/**
	 * @author Jan Pavlica
	 * @return
	 */
	public Integer getIdTyp();
	/**
	 * @author Jan Pavlica
	 * @param idTyp
	 */
	public void setIdTyp(Integer idTyp);
	/**
	 * @author Jan Pavlica
	 * @return
	 */
	public Integer getIdVybeh();
	/**
	 * @author Jan Pavlica
	 * @param idVybeh
	 */
	public void setIdVybeh(Integer idVybeh);
	
}

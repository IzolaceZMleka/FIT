package shapes;

import javafx.beans.property.DoubleProperty;
import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.StrokeType;


/**
 * class Anchor
 * Class making anchors for editing shapes.
 * @author Jan Pavlica
 *
 */
public class Anchor extends Circle {
	@SuppressWarnings("unused")
	private final DoubleProperty x, y;

	/**
	 * Constructor
	 * @param color - color to fill
	 * @param x
	 * @param y
	 */
	Anchor(Color color, DoubleProperty x, DoubleProperty y) {
		super(x.get(), y.get(), 5);
		setFill(color.deriveColor(1, 1, 1, 0.5));
		setStroke(color);
		setStrokeWidth(2);
		setStrokeType(StrokeType.OUTSIDE);

		this.x = x;
		this.y = y;

		x.bind(centerXProperty());
		y.bind(centerYProperty());
		enableDrag();
		this.setVisible(false);
	}

	/**
	 * Making anchor draggable.
	 * @author Jan Pavlica
	 */
	private void enableDrag() {
		final Delta dragDelta = new Delta();
		setOnMousePressed(new EventHandler<MouseEvent>() {
			@Override public void handle(MouseEvent mouseEvent) {
				dragDelta.x = getCenterX() - mouseEvent.getX();
				dragDelta.y = getCenterY() - mouseEvent.getY();
				getScene().setCursor(Cursor.MOVE);
			}
		});
		setOnMouseReleased(new EventHandler<MouseEvent>() {
			@Override public void handle(MouseEvent mouseEvent) {
				getScene().setCursor(Cursor.HAND);
			}
		});
		setOnMouseDragged(new EventHandler<MouseEvent>() {
			@Override public void handle(MouseEvent mouseEvent) {
				double newX = mouseEvent.getX() + dragDelta.x;
				if (newX > 0 && newX < getScene().getWidth()) {
					setCenterX(newX);
				}
				double newY = mouseEvent.getY() + dragDelta.y;
				if (newY > 0 && newY < getScene().getHeight()) {
					setCenterY(newY);
				}
			}
		});
		setOnMouseEntered(new EventHandler<MouseEvent>() {
			@Override public void handle(MouseEvent mouseEvent) {
				if (!mouseEvent.isPrimaryButtonDown()) {
					getScene().setCursor(Cursor.HAND);
				}
			}
		});
		setOnMouseExited(new EventHandler<MouseEvent>() {
			@Override public void handle(MouseEvent mouseEvent) {
				if (!mouseEvent.isPrimaryButtonDown()) {
					getScene().setCursor(Cursor.DEFAULT);
				}
			}
		});
	}

	
	/**
	 * class Delta
	 * coords
	 * @author Jan Pavlica
	 *
	 */
	private class Delta { double x, y; }
}

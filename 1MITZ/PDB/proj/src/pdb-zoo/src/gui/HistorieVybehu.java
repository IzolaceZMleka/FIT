package gui;

import javafx.beans.property.SimpleStringProperty;
/**
 * Class HistorieVybehu
 * Class for representing row in table showing enclosure history.
 * @author Jan Pavlica
 *
 */
public class HistorieVybehu {
	private final SimpleStringProperty pavilonJmeno;
    private final SimpleStringProperty odDatum;
    private final SimpleStringProperty doDatum;

    public HistorieVybehu(String pavilonJmeno, String odDatum, String doDatum) {
        this.pavilonJmeno = new SimpleStringProperty(pavilonJmeno);
        this.odDatum = new SimpleStringProperty(odDatum);
        this.doDatum = new SimpleStringProperty(doDatum);
    }

    public String getPavilonJmeno() {
        return pavilonJmeno.get();
    }

    public void setPavilonJmeno(String pavilonJmeno) {
    	this.pavilonJmeno.set(pavilonJmeno);
    }

    public String getOdDatum() {
        return odDatum.get();
    }

    public void setOdDatum(String odDatum) {
    	this.odDatum.set(odDatum);
    }

    public String getDoDatum() {
        return doDatum.get();
    }

    public void setDoDatum(String doDatum) {
    	this.doDatum.set(doDatum);
    }

}

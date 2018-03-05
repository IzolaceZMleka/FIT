package gui;

import javafx.beans.property.SimpleStringProperty;

/**
 * Class Animal
 * Animal class representing row in table.
 * @author Jan Pavlica
 *
 */
public class Animal {
	private int animalId;
	private SimpleStringProperty druh;
    private SimpleStringProperty jmeno;
    private SimpleStringProperty pohlavi;
    private SimpleStringProperty datumNarozeni;
 
    public Animal(String druh, String jmeno, String pohlavi, String datumNarozeni, int id) {
    	this.animalId = id;
        this.druh = new SimpleStringProperty(druh);
        this.jmeno = new SimpleStringProperty(jmeno);
        this.pohlavi = new SimpleStringProperty(pohlavi);
        this.datumNarozeni = new SimpleStringProperty(datumNarozeni);
    }

	public String  getDruh() {
		return druh.get();
	}

	public String  getJmeno() {
		return jmeno.get();
	}

	public String  getPohlavi() {
		return pohlavi.get();
	}

	public String  getDatumNarozeni() {
		return datumNarozeni.get();
	}

	public void setDruh(String druh) {
		this.druh.set(druh);
	}

	public void setJmeno(String jmeno) {
		this.jmeno.set(jmeno);;
	}

	public void setPohlavi(String pohlavi) {
		this.pohlavi.set(pohlavi);
	}

	public void setDatumNarozeni(String datumNarozeni) {
		this.datumNarozeni.set(datumNarozeni);
	}

	public int getAnimalId() {
		return animalId;
	}

	public void setAnimalId(int animalId) {
		this.animalId = animalId;
	}
 

}

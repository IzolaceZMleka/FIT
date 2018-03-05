package fit.gja.examples.jsf;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.enterprise.context.SessionScoped;
import javax.inject.Named;
import javax.faces.event.ValueChangeEvent;

/**
 *
 * @author Filip Pobořil
 */
@Named(value = "language")
@SessionScoped
public class Language implements Serializable {

    private static Map<String, String> available;

    private String selected;

    static {
        available = new HashMap<>();
        available.put("Čeština", "cs");
        available.put("English", "en");
        available.put("日本語", "jp");
    }

    public Map<String, String> getAvailable() {
        return available;
    }

    public String getSelected() {
        return selected == null ? "en" : selected;
    }

    public void setSelected(String selected) {
        this.selected = selected;
    }

    public void selectedChange(ValueChangeEvent e) {
        setSelected(e.getNewValue().toString());
    }

    public String getGreeting() {
        switch (getSelected()) {
            case "en":
            default:
                return "Hello";
            case "cs":
                return "Ahoj";
            case "jp":
                return "こんにちは";
        }
    }

}

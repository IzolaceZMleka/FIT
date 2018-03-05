package fit.gja.examples.pf;

import java.util.ArrayList;
import java.util.List;
import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;

/**
 *
 * @author Filip Pobořil
 */
@ManagedBean
public class DataList {

    private List<String> data;

    @PostConstruct
    public void init() {
        data = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            data.add("Item " + i);
        }
    }

    public List<String> getData() {
        return data;
    }

}

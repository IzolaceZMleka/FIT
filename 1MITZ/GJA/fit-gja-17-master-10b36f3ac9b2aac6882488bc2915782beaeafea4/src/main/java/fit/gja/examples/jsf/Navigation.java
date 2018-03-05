package fit.gja.examples.jsf;

import java.io.Serializable;
import javax.enterprise.context.RequestScoped;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;

/**
 *
 * @author Filip Pobořil
 */
// Here must be older ManagedBean instead of Named because we need eager=true,
// which Named annotation not supports.
@ManagedBean(name = "navigation", eager = true)
@RequestScoped
public class Navigation implements Serializable {

    private static final String PREFIX = "/examples/faces/navigation/";

    /**
     * This managed property will read value from request parameter pageId.
     */
    // Works only when eager is true.
    @ManagedProperty(value = "#{param.pageId}")
    private String pageId;

    public String showPage1() {
        return PREFIX + "page1";
    }

    public String showPage2() {
        return PREFIX + "page2";
    }

    // processPage1() and processPage2() returns same value but forwards to
    // different. Thats because navation-rule config in faces-config.xml.

    public String processPage1() {
        return "page";
    }

    public String processPage2() {
        return "page";
    }

    /**
     * Conditional navigation based on pageId.
     *
     * If pageId is 1 then show page1, if pageId is 2 then show page2, else show
     * index.
     *
     * @return
     */
    public String show() {
        if (pageId == null) {
            return PREFIX + "index";
        }
        if (pageId.equals("1")) {
            return PREFIX + "page1";
        } else if (pageId.equals("2")) {
            return PREFIX + "page2";
        } else {
            return PREFIX + "index";
        }
    }

    public String getPageId() {
        return pageId;
    }

    public void setPageId(String pageId) {
        this.pageId = pageId;
    }

}

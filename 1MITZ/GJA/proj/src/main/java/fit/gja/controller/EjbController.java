package fit.gja.controller;

import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.resolve;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.PathVariable;

/**
 * @author Filip Pobořil
 */
@Controller
@RequestMapping("/ejb")
public class EjbController {

    @RequestMapping("/{name}")
    public String show(@PathVariable("name") String name) {
        return resolve(JSP, "ejb/" + name);
    }
}

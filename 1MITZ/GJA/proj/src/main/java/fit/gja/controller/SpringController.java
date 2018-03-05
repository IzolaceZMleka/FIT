package fit.gja.controller;

import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.resolve;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;


@Controller
@RequestMapping("/spring")
public class SpringController {

    @RequestMapping("/{name}")
    public String show(@PathVariable("name") String name) {
        return resolve(JSP, "spring/" + name);
    }
}

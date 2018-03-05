package fit.gja.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;

import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.Type.THYMELEAF;
import static fit.gja.util.TemplateUtils.resolve;

@Controller
public class HelloController {

    @RequestMapping("/")
    public String index(Model model) {
        model.addAttribute("recipient", "World");
        return resolve(JSP, "index");
    }

    @RequestMapping("/t")
    public String indexT(Model model) {
        model.addAttribute("recipient", "World");
        return resolve(THYMELEAF, "index");
    }
}
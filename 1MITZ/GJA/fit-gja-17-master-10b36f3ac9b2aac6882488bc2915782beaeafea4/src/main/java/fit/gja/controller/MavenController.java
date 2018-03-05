package fit.gja.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.resolve;

@Controller
@RequestMapping("/maven")
public class MavenController {

    @RequestMapping(value={"", "/"})
    public String main() {
        return resolve(JSP, "maven-main");
    }

    @RequestMapping(value="/pom")
    public String pomFile() {
        return resolve(JSP, "maven-pom");
    }
}

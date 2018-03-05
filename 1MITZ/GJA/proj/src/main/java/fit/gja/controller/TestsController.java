package fit.gja.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.resolve;

@Controller
@RequestMapping("/tests")
public class TestsController {

    @RequestMapping(value="/junit")
    public String junitTests() {
        return resolve(JSP, "tests-junit");
    }

    @RequestMapping(value="/integration")
    public String integrationTests() {
        return resolve(JSP, "tests-integration");
    }

    @RequestMapping(value="/acceptable")
    public String functionalTests() {
        return resolve(JSP, "tests-acceptable");
    }
}

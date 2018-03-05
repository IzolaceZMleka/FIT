package fit.gja.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.servlet.NoHandlerFoundException;

import javax.servlet.http.HttpServletRequest;

import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.resolve;

@ControllerAdvice
public class ExceptionController {
    private static final Logger log = LoggerFactory.getLogger(ExceptionController.class);

    @ExceptionHandler(Exception.class)
    public String handleError(HttpServletRequest request, Exception e) {
        log.error("Request: " + request.getRequestURL() + " raised " + e);
        return resolve(JSP, "500");
    }

    @ExceptionHandler(NoHandlerFoundException.class)
    public String handleError404(HttpServletRequest request, Exception e) {
        log.error("Request: " + request.getRequestURL() + " raised " + e);
        return resolve(JSP, "404");
    }
}
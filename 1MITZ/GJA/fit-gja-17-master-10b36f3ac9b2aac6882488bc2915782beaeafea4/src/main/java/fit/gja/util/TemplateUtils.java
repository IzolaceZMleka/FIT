package fit.gja.util;

public class TemplateUtils {
    public enum Type {
        JSP,
        THYMELEAF
    }

    public static String resolve(Type type, String template) {
        switch (type) {
            case JSP:
                return "jsp/" + template;
            case THYMELEAF:
                return "thymeleaf/" + template;
            default:
                return "thymeleaf/" + template;
        }
    }
}

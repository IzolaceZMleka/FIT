package fit.gja.controller;

import fit.gja.dao.UserDao;
import fit.gja.model.User;
import static fit.gja.util.TemplateUtils.Type.JSP;
import static fit.gja.util.TemplateUtils.resolve;
import javax.servlet.http.HttpServletRequest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
public class UserController {

    @Autowired
    private UserDao userDao;
    
    @RequestMapping("/user/show")
    public String show(Model model) {
        model.addAttribute("userList", userDao.list());
        model.addAttribute("message", null);
        
        return resolve(JSP, "spring/example/User/show");
    }
    
    @RequestMapping("/user/add-user")
    public String add(Model model) {
        model.addAttribute("user", new User());
        model.addAttribute("message", null);
                
        return resolve(JSP, "spring/example/User/add");
    }
    
    @RequestMapping(value = "/user/add-user", method = RequestMethod.POST)
    public String addUserAction(Model model, @ModelAttribute("user") User user) {
        userDao.createUser(user);
        model.addAttribute("message", "Uživatel byl úspěšně uložen.");
        
        return resolve(JSP, "spring/example/User/add");
    }
    
    @RequestMapping(value = "/user/remove-user")
    public String removeUserAction(Model model, HttpServletRequest request) {
        userDao.deleteUser(Integer.parseInt(request.getParameter("id")));  
        model.addAttribute("message", "Uživatel byl úspěšně odstraněn.");
        
        return resolve(JSP, "spring/example/User/show");
    }
       
}
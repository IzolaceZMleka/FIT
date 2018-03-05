<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Příklad MVC aplikace"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />
        
        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
    <div class="row">
        <ol class="breadcrumb">
            <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
            <li class="active">Spring / Příklad MVC aplikace</li>
        </ol>
    </div><!--/.row-->

    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Spring: Příklad MVC aplikace</h1>
        </div>
    </div><!--/.row-->

    <div clas="row">
        <div class="col-lg-12">
            <div class="panel panel-primary">
                <div class="panel-heading">Jak bude vypadat výsledná aplikace?</div>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Jako ukázka MVC aplikace vytvořené pomocí frameworku Spring bude sloužit jednoduchá správa uživatelů na základě tabulky <i>user</i>, která byla vytvořena v předchozí kapitole. 
                        Uživatele bude možné prohlížet, přidávat a mazat.
                   </p>
                </div>
            </div>
            <div class="panel panel-info">
               <div class="panel-heading">Model aplikace</div>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Nejdříve vložíme tabulku <i>user</i> do databáze, stačí tedy provést následující SQL skript. 
                    </p>                   
<div class="custom-code-block">
<pre class="custom-code">
<code class="sql hljs">CREATE TABLE `user` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `surname` varchar(255) NOT NULL,
  `username` varchar(255) NOT NULL,
  `password` varchar(1023) NOT NULL,
  `email` varchar(255) NOT NULL,
  `telephone` varchar(15) NOT NULL,
  PRIMARY KEY (id)
);</code>
</pre>
                    </div>   
                    <p class="custom-text-block">
                        Dále budeme potřebovat objekty <i>UserDao</i>, který bude obsahovat typické CRUD metody, <i>UserMapper</i> pro namapování naší tabulky a entitu <i>User</i>.
                    </p>
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">public class UserDao {

    private JdbcTemplate template;

    public void setDataSource(DataSource dataSource) {
        this.template = new JdbcTemplate(dataSource);
    }
    
    public List<User> list() {
        return template.query("SELECT id, name, surname, username, password, email, telephone FROM user", new UserMapper());
    }
    
    public User getUser(int id) {
        return template.queryForObject("SELECT id, name, surname, username, password, email, telephone FROM user WHERE id = ?", new Object[]{id}, new UserMapper());
    }
    
    public void createUser(User user) {
        template.update("INSERT INTO user (name, surname, username, password, email, telephone)VALUES(?, ?, ?, ?, ?, ?)",
                user.getName(), user.getSurname(), user.getUsername(), user.getPassword(), user.getEmail(), user.getTelephone());
    }
    
    public void updateUser(User user) {
        template.update("UPDATE user SET name = ?, surname = ?, username = ?, password = ?, email = ?, telephone = ? WHERE id = ?",
                user.getName(), user.getSurname(), user.getUsername(), user.getPassword(), user.getEmail(), user.getTelephone(), user.getId());
    }

    public void deleteUser(int id) {
        template.update("DELETE FROM user WHERE id = ?", id);
    }

}</code>
</pre>
                    </div>            
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">public class UserMapper implements RowMapper<User> {

    @Override
    public User mapRow(ResultSet rs, int i) throws SQLException {
        User user = new User();
        user.setId(rs.getInt("id"));
        user.setName(rs.getString("name"));
        user.setSurname(rs.getString("surname"));
        user.setUsername(rs.getString("username"));
        user.setPassword(rs.getString("password"));
        user.setEmail(rs.getString("email"));
        user.setTelephone(rs.getString("telephone"));

        return user;
    }

}</code>
</pre>
                    </div>                           
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">public class User {

    private int id;
    private String name;
    private String surname;
    private String username;
    private String password;
    private String email;
    private String telephone;
    
    // get a set metody
}</code>
</pre>
                    </div>   
                </div>
            </div>

           <div class="panel panel-info">
               <div class="panel-heading">Výpis a mazání uživatelů</div>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Nyní když již máme hotový model aplikace, můžeme založit nový pohled - <i>show</i>, který bude odpovídat šabloně pro zobrazení tabulky všech uživatelů.
                    </p>                   
<div class="custom-code-block">
<pre class="custom-code">
<code class="html hljs"> &lt;div class=&quot;col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main&quot;&gt;			
            &lt;div class=&quot;row&quot;&gt;
                &lt;ol class=&quot;breadcrumb&quot;&gt;
                    &lt;li&gt;&lt;a href=&quot;#&quot;&gt;&lt;span class=&quot;glyphicon glyphicon-home&quot;&gt;&lt;/span&gt;&lt;/a&gt;&lt;/li&gt;
                    &lt;li class=&quot;active&quot;&gt;Spring / Př&iacute;klad MVC aplikace&lt;/li&gt;
                &lt;/ol&gt;
            &lt;/div&gt;&lt;!--/.row--&gt;

            &lt;c:if test=&quot;${not empty message}&quot;&gt;
                &lt;div class=&quot;alert bg-success&quot; role=&quot;alert&quot;&gt;${message} &lt;a href=&quot;#&quot; class=&quot;pull-right&quot;&gt;&lt;span class=&quot;glyphicon glyphicon-remove&quot;&gt;&lt;/span&gt;&lt;/a&gt;&lt;/div&gt;
            &lt;/c:if&gt;
            &lt;div class=&quot;row&quot;&gt;
                &lt;div class=&quot;col-md-12&quot;&gt;
                    &lt;div class=&quot;panel panel-default&quot;&gt;
                        &lt;div class=&quot;panel-heading&quot;&gt;
                            &lt;div class=&quot;row&quot;&gt;
                                &lt;div class=&quot;col-md-8&quot;&gt;Seznam uživatelů&lt;/div&gt;
                                &lt;div class=&quot;col-md-4 text-right&quot;&gt;
                                    &lt;a href=&quot;${pageContext.request.contextPath}/add-user&quot;&gt;&lt;button type=&quot;button&quot; class=&quot;btn btn-primary&quot;&gt;Přidat uživatele&lt;/button&gt;&lt;/a&gt;
                                &lt;/div&gt;	
                            &lt;/div&gt;
                        &lt;/div&gt;
                        &lt;div class=&quot;panel-body&quot;&gt;
                            &lt;div class=&quot;table-responsive&quot;&gt;
                                &lt;table class=&quot;table table-striped table-bordered table-hover&quot;&gt;
                                    &lt;thead&gt;
                                        &lt;tr&gt;
                                            &lt;th&gt;
                                                Jm&eacute;no a Př&iacute;jmen&iacute;
                                            &lt;/th&gt;
                                            &lt;th&gt;
                                                Uživatelsk&eacute; jm&eacute;no
                                            &lt;/th&gt;
                                            &lt;th&gt;
                                                Email
                                            &lt;/th&gt;
                                            &lt;th&gt;
                                                Telefon
                                            &lt;/th&gt;
                                            &lt;th&gt;Akce&lt;/th&gt;
                                        &lt;/tr&gt;
                                    &lt;/thead&gt;
                                    &lt;tbody&gt;
                                    &lt;c:if test=&quot;${userList.size() != 0}&quot;&gt;
                                        &lt;c:forEach var=&quot;user&quot; items=&quot;${userList}&quot;&gt;
                                            &lt;tr&gt;
                                                &lt;td&gt;${user.name} ${user.surname}&lt;/td&gt;
                                                &lt;td&gt;${user.username}&lt;/td&gt;
                                                &lt;td&gt;${user.email}&lt;/td&gt;
                                                &lt;td&gt;${user.telephone}&lt;/td&gt;
                                                &lt;td&gt;
                                                    &lt;a href=&quot;${pageContext.request.contextPath}/remove-user?id=${user.id}&quot;&gt;
                                                        &lt;button type=&quot;button&quot; class=&quot;btn btn-default&quot;&gt;
                                                            &lt;span class=&quot;glyphicon glyphicon-remove&quot; aria-hidden=&quot;true&quot;&gt;&lt;/span&gt;
                                                        &lt;/button&gt;
                                                    &lt;/a&gt;
                                                &lt;/td&gt;
                                            &lt;/tr&gt;
                                        &lt;/c:forEach&gt;
                                    &lt;/c:if&gt;
                                    &lt;/tbody&gt;
                                &lt;/table&gt;
                            &lt;/div&gt;
                        &lt;/div&gt;
                    &lt;/div&gt;
                &lt;/div&gt;

            &lt;/div&gt;
        &lt;/div&gt;</code>
</pre>
                    </div>   
<p class="custom-text-block">
    Zobrazení tabulky všech uživatelů a také přidání nového uživatele nebo jeho smazání bude obsluhováno controllerem nazvaným <i>UserController</i>. Lišit se budou pouze jednotlivé akce.
    Pro zobrazení uživatelů vytvoříme metodu <i>show</i>.
</p>
<div class="custom-code-block">
<pre class="custom-code">
<code class="xml hljs">@RequestMapping("/")
    public String show(Model model) {
        model.addAttribute("userList", userDao.list());
        model.addAttribute("message", null);
        
        return "jsp/User/show";
    }</code>
</pre>
                    </div>            
<p class="custom-text-block">Pro smazání uživatele pak bude sloužit metoda <i>removeUserAction</i>.</p>
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">@RequestMapping(value = "/remove-user")
    public String removeUserAction(Model model, HttpServletRequest request) {
        userDao.deleteUser(Integer.parseInt(request.getParameter("id")));  
        model.addAttribute("message", "Uživatel byl úspěšně odstraněn.");
        
        return "jsp/User/show";
    }</code>
</pre>
                    </div>  

                </div>
            </div>
                                                        
           <div class="panel panel-info">
               <div class="panel-heading">Přidat uživatele</div>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Šablona <i>add</i> pro přidání nového uživatele bude obsahovat formulář, který bude obsluhovat metoda <i>addUserAction</i> v příslušném controlleru. Samotné jeho zobrazení pak zajišťuje metoda <i>add</i>.
                    </p>
<div class="custom-code-block">
<pre class="custom-code">
<code class="html hljs">&lt;div class=&quot;col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main&quot;&gt;			
            &lt;div class=&quot;row&quot;&gt;
                &lt;ol class=&quot;breadcrumb&quot;&gt;
                    &lt;li&gt;&lt;a href=&quot;#&quot;&gt;&lt;span class=&quot;glyphicon glyphicon-home&quot;&gt;&lt;/span&gt;&lt;/a&gt;&lt;/li&gt;
                    &lt;li class=&quot;active&quot;&gt;Spring / Př&iacute;klad MVC aplikace / Přidat uživatele&lt;/li&gt;
                &lt;/ol&gt;
            &lt;/div&gt;&lt;!--/.row--&gt;

            &lt;c:if test=&quot;${not empty message}&quot;&gt;
                &lt;div class=&quot;alert bg-success&quot; role=&quot;alert&quot;&gt;${message} &lt;a href=&quot;#&quot; class=&quot;pull-right&quot;&gt;&lt;span class=&quot;glyphicon glyphicon-remove&quot;&gt;&lt;/span&gt;&lt;/a&gt;&lt;/div&gt;
            &lt;/c:if&gt;
            &lt;div class=&quot;row&quot;&gt;
                &lt;div class=&quot;col-md-12&quot;&gt;
                    &lt;div class=&quot;panel panel-default&quot;&gt;
                        &lt;div class=&quot;panel-heading&quot;&gt;
                            Přidat uživatele
                        &lt;/div&gt;
                        &lt;div class=&quot;panel-body&quot;&gt;
                            &lt;div class=&quot;col-md-12&quot;&gt;
                                &lt;form:form action=&quot;add-user&quot; method=&quot;post&quot; modelAttribute=&quot;user&quot;&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;form:label path=&quot;name&quot;&gt;Jm&eacute;no&lt;font color=&quot;red&quot;&gt;&lt;b&gt;*&lt;/b&gt;&lt;/font&gt;&lt;/form:label&gt;
                                        &lt;form:input path=&quot;name&quot; class=&quot;form-control&quot; /&gt;
                                    &lt;/div&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;form:label path=&quot;surname&quot;&gt;Př&iacute;jmen&iacute;&lt;font color=&quot;red&quot;&gt;&lt;b&gt;*&lt;/b&gt;&lt;/font&gt;&lt;/form:label&gt;
                                        &lt;form:input path=&quot;surname&quot; class=&quot;form-control&quot; /&gt;
                                    &lt;/div&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;form:label path=&quot;username&quot;&gt;Uživatelsk&eacute; jm&eacute;no&lt;font color=&quot;red&quot;&gt;&lt;b&gt;*&lt;/b&gt;&lt;/font&gt;&lt;/form:label&gt;
                                        &lt;form:input path=&quot;username&quot; class=&quot;form-control&quot; /&gt;
                                    &lt;/div&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;form:label path=&quot;password&quot;&gt;Heslo&lt;font color=&quot;red&quot;&gt;&lt;b&gt;*&lt;/b&gt;&lt;/font&gt;&lt;/form:label&gt;
                                        &lt;form:input type=&quot;password&quot; path=&quot;password&quot; class=&quot;form-control&quot; /&gt;
                                    &lt;/div&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;form:label path=&quot;email&quot;&gt;Email&lt;font color=&quot;red&quot;&gt;&lt;b&gt;*&lt;/b&gt;&lt;/font&gt;&lt;/form:label&gt;
                                        &lt;form:input type=&quot;email&quot; path=&quot;email&quot; class=&quot;form-control&quot; /&gt;
                                    &lt;/div&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;form:label path=&quot;telephone&quot;&gt;Telefon&lt;/form:label&gt;
                                        &lt;form:input type=&quot;telephone&quot; path=&quot;telephone&quot; class=&quot;form-control&quot; /&gt;
                                    &lt;/div&gt;
                                    &lt;div class=&quot;form-group&quot;&gt;
                                        &lt;button type=&quot;submit&quot; class=&quot;btn btn-primary&quot;&gt;Uložit&lt;/button&gt;
                                    &lt;/div&gt;
                                &lt;/form:form&gt;
                            &lt;/div&gt;
                        &lt;/div&gt;
                    &lt;/div&gt;
                &lt;/div&gt;

            &lt;/div&gt;
        &lt;/div&gt;</code>
</pre>
                    </div>  
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">@RequestMapping("/add-user")
    public String add(Model model) {
        model.addAttribute("user", new User());
        model.addAttribute("message", null);
        
        return "jsp/User/add";
    }</code>
</pre>
                    </div>  
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">@RequestMapping(value = "/add-user", method = RequestMethod.POST)
    public String addUserAction(Model model, @ModelAttribute("user") User user) {
        userDao.createUser(user);
        model.addAttribute("message", "Uživatel byl úspěšně uložen.");
        
        return "jsp/User/add";
    }</code>
</pre>
                    </div>  
  <p class="custom-text-block">Praktickou ukázku projektu <a href="${pageContext.request.contextPath}/example/example-spring-9.zip">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="${pageContext.request.contextPath}/user/show">pak dostupná zde.</a></p>

                    
                </div>
            </div>
        </div>
    </div>

</div>	<!--/.main-->

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>

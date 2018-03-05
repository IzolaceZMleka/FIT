<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: JDBC"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />
        

<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
    <div class="row">
        <ol class="breadcrumb">
            <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
            <li class="active">Spring / JDBC</li>
        </ol>
    </div><!--/.row-->

    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Spring: JDBC</h1>
        </div>
    </div><!--/.row-->

    <div clas="row">
        <div class="col-lg-12">
            <div class="panel panel-primary">
                <div class="panel-heading">Co to je JDBC?</div>
                <div class="panel-body">
                    <p class="custom-text-block">
JDBC je základní API pro přístup k relačním databázím v Jave a poskytuje vrstvu pro komunikaci s databázi. Konkrétní databázový server pak musí mít implementovaný svůj JDBC ovladač. Spring přináší několik základních rozšíření pro jednodušší práci s JDBC.                    </p>
                </div>
            </div>
           <div class="panel panel-info">
               <div class="panel-heading">Nastavení připojení k databázi</div>
                <div class="panel-body">
                    <p class="custom-text-block">
Výchozí databáze v tomto případě bude MySQL. Parametry pro připojení k databázi (DataSource) je nutné specifikovat v bean.xml. Alternativou pak může být vytvoření vlastní konfigurační třídy, jak jsme si ukázali v jedné z předchozích kapitol.
                    </p>                   
<div class="custom-code-block">
<pre class="custom-code">
<code class="xml hljs">&lt;bean id=&quot;dataSource&quot; class=&quot;org.apache.commons.dbcp.BasicDataSource&quot; destroy-method=&quot;close&quot;&gt;
    &lt;property name=&quot;driverClassName&quot; value=&quot;mysql.jdbc.Driver&quot; /&gt;
    &lt;property name=&quot;url&quot; value=&quot;jdbc:mysql://localhost:3306/springjdbc&quot; /&gt;
    &lt;property name=&quot;username&quot; value=&quot;uzivatelske jmeno databaze&quot; /&gt;
    &lt;property name=&quot;password&quot; value=&quot;uzivatelske heslo databaze&quot; /&gt;
&lt;/bean&gt;</code>
</pre>
                    </div>   
                    <p class="custom-text-block">Abychom vůbec mohli používat knihovnu JDBC, musíme ji ještě přidat do seznamu závislosti v souboru pom.xml.</p>
<div class="custom-code-block">
<pre class="custom-code">
<code class="xml hljs">&lt;dependency&gt;
    &lt;groupId&gt;org.springframework&lt;/groupId&gt;
    &lt;artifactId&gt;spring-jdbc&lt;/artifactId&gt;
&lt;/dependency&gt;</code>
</pre>
                    </div>            
                    
                    
                </div>
            </div>
           <div class="panel panel-info">
               <div class="panel-heading">Dotazy do databáze - <i>JdbcTemplate</i></div>
                <div class="panel-body">
                    <p class="custom-text-block">
Pro demonstraci dotazů prováděných nad databázi musíme nejdříve udělat tabulku user. Ta bude sloužit k evidenci uživatelů, kteří se přihlašují do aplikace.
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
);
</code>
</pre>
                    </div>
                    
<p class="custom-text-block">Nyní využijeme vestavěnou třídu ve Springu <i>JdbcTemplate</i>, která nám zjednoduší práci s databázi. Instanci této třídy je možné získat klasicky skrze metodu dependency injection. Naší novou třídu nazvanou <i>UserDao</i> tak zaregistrujeme v souboru beans.xml. 
</p>
         
<div class="custom-code-block">
<pre class="custom-code">
<code class="xml hljs">&lt;bean id=&quot;userDao&quot; class=&quot;fit.gja.dao.UserDao&quot;&gt;
    &lt;property name=&quot;dataSource&quot; ref=&quot;dataSource&quot; /&gt;
&lt;/bean&gt;
</code>
</pre>
                    </div>
<p class="custom-text-block">Do této třídy můžeme vkládat libovolné metody, které budou reprezentovat SQL dotazy. V našem případě se bude jednat o základní CRUD operace, tedy vytvoření nového uživatele, získání daného uživatele z databáze, aktualizace jeho údajů a úplné smazání uživatele. 
</p>
<div class="custom-code-block">
<pre class="custom-code">
<code class="java hljs">public class UserDao {

    private JdbcTemplate template;

    public void setDataSource(DataSource dataSource) {
        this.template = new JdbcTemplate(dataSource);
    }

    public void createUser(User user) {
        template.update("INSERT INTO user (name, surname, username, password, email, telephone)VALUES(?, ?, ?, ?, ?, ?)", user.getName(), user.getSurname(), user.getUsername(), user.getPassword(), user.getEmail(), user.getTelephone());
    }

    public User getUser(int id) {
        return template.queryForObject("SELECT * FROM user WHERE id = ?", new Object[]{ id }, new UserMapper());
    }

    public void updateUser(User user) {
        template.update("UPDATE user SET name = ?, surname = ?, username = ?, password = ?, email = ?, telephone = ? WHERE id = ?", user.getName(), user.getSurname(), user.getUsername(), user.getPassword(), user.getEmail(), user.getTelephone(), user.getId());
    }

    public void deleteUser(User user) {
        template.update("DELETE FROM user WHERE id = ?", user.getId());
    }

}
</code>
</pre>
                    </div>
                </div>
            </div>
           <div class="panel panel-info">
               <div class="panel-heading">Mapování výsledku na objekty - <i>RowMapper&lt;T extends Object&gt;</i></div>
                <div class="panel-body">
                    <p class="custom-text-block">
Ve výše uvedeném kódu si můžete všimnout, že jsme použili dva nové objekty - <i>User</i> a také <i></i>. První z nich je pouze entita, která reprezentuje identitu uživatele a odpovídá struktuře dané tabulky v databázi.</p>
<code class="java hljs">public class User {

    private int id;
    private String name;
    private String surname;
    private String username;
    private String password;
    private String email;
    private String telephone;
    
    // get a set metody
}
</code>         
                    <p class="custom-text-block">Druhý z nich je však zajímavější, umožňuje mapování řádků na danou entitu, v tomto případě <i>User</i>. Takového chování lze docílit vytvořením nové třídy (např. <i>UserMapper</i>) a implementaci rozhraní <i>RowMapper&lt;T extends Object&gt;</i>.Tato činnost sice není na rozdíl od standardních ORM frameworků automatická, za to se však jedná o nekomplikované řešení.                     
                    </p>
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

}
</code>
</pre>
                    </div>
  <p class="custom-text-block">Praktickou ukázku projektu <a href="">lze stáhnout zde</a>. Živá verze tohoto projektu je <a href="">pak dostupná zde.</a></p>

                    
                </div>
            </div>
        </div>
    </div>

</div>	<!--/.main-->

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>

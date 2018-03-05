<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <jsp:include page="../components/header.jsp">
        <jsp:param name="title" value="Spring: Java Persistence API (JPA)"/>
    </jsp:include>
    <body>
        <jsp:include page="../components/top-panel.jsp" />
        <jsp:include page="../components/menu.jsp" />

        <div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
            <div class="row">
                <ol class="breadcrumb">
                    <li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
                    <li class="active">Spring / JPA</li>
                </ol>
            </div><!--/.row-->

            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Spring: Java Persistence API (JPA)</h1>
                </div>
            </div><!--/.row-->

            <div clas="row">
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">Co to je JPA?</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Java Persistence API je standard v Jave EE pro zajištění perzistence dat za pomocí objektově relačního mapování (ORM). V současné době existuje několik implementací, z nichž nejznámější je knihovna Hibernate.
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Entity</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                Entity představují základní jednotky, které odpovidají struktuře databázové tabulky. Také definuje rozhranní, jak je možné nákladat s daty, které zapouzdřují. Narozdíl od objektů reprezentujících hodnotu (value objects) mají svou vlastní identitu (může odpovídat primárnímu klíči).
                            </p>                   
                            <p class="custom-text-block">
                                Entita:
                                1) obsahuje anotaci <i>@Entity</i>
                                2) je identifikována pomocí primárního klíče (anotace @id)
                                3) obsahuje proměnné třídy a metody "get"/"set" 
                            </p>
                            <p class="custom-text-block">Následuje příklad jak může taková jednoduchá Entita vypadat.</p>

                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">@Entity
public class User {
    @Id
    @GeneratedValue
    private Long id;

    private String name;

    private String surname;

    private String username;

    private String password;

    // + set a get metody
} </code>
                                </pre>
                            </div>            
                            <p class="custom-text-block">
                                Do třídy entity můžeme umístit několik anotací, které upřesňují hodnoty u daných sloupců v databázi. 
                            </p>           
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">@Entity
@Table(name = "user")
public class User {
    
    @Id
    @Generated
    private Long id;

    @Column(name = "name", length = 255, nullable = false)
    private String name;

    @Column(name = "surname", length = 255, nullable = false)
    private String surname;

}</code>
                                </pre>
                            </div>  


                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">EntityManager</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                V rámci perzistenčního kontextu jsou spravovány jednotlivé entity správcem entit - EntityManager. Toto rozhraní obsahuje několik důležitých metod jako jsou například <i>persist</i>, <i>remove</i>, <i>detach</i> apod. Následuje příklad práce s instanci třídy, která implementuje toto rozhrání.
                            </p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">EntityManager em = Persistence.createEntityManagerFactory("entityManager").createEntityManager();
em.getTransaction().begin();

User user = new User();
user.setName("Petr");
user.setSurname("Novak");

em.persist(user);
em.getTransaction().commit();
em.close();

</code>
                                </pre>
                            </div>


                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">Dotazy do databáze</div>
                        <div class="panel-body">
                            <p class="custom-text-block">
                                V praktické aplikaci vzniká mnoho vazeb mezi jednotlivými tabulkami, rozlišuji se 3 základní relační vazby - 1:1, 1:N, M:N. JPA poskytuje podporu pro efektivnější implementaci těchto vazeb mezi entitami.
                            </p>

                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">@JoinColumn(name = "user")
private User user;

@ManyToMany(mappedBy = "eaxmple")
private List&lt;User&gt; users;
</code>
                                </pre>
                            </div>
                            <p class="custom-text-block">Obyčejné dotazy je pak možné psát za pomocí JPQL (JPA Query Language). Ten umožňuje na základě standardního dotazu SQL vracet entity či jejich kolekce.</p>
                            <div class="custom-code-block">
                                <pre class="custom-code">
<code class="java hljs">EntityManager em = Persistence.createEntityManagerFactory("entityManager").createEntityManager();
Query query = em.createQuery("SELECT us FROM user us ORDER BY us.id DESC");

// pretypujeme na pozadovanou kolekci
List&lt;User&gt; list = (List&lt;User&gt;) query.getResultList();
</code>
                                </pre>
                            </div>

                        </div>
                    </div>
                </div>
            </div>

        </div>	<!--/.main-->

        <jsp:include page="../components/footer.jsp" />
    </body>
</html>

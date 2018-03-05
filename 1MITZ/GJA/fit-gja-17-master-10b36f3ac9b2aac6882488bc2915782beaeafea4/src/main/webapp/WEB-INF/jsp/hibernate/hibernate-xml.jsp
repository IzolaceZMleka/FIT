<%@page contentType="text/html" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>
<jsp:include page="../components/header.jsp">
    <jsp:param name="title" value="Hlavní strana"/>
</jsp:include>
<body>
<jsp:include page="../components/top-panel.jsp"/>
<jsp:include page="../components/menu.jsp"/>

<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">
    <div class="row">
        <ol class="breadcrumb">
            <li><a href="${pageContext.request.contextPath}"><span class="glyphicon glyphicon-home"></span></a></li>
            <li>Hibernate</li>
            <li class="active">Hibernate a mapování pomocí XML</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Hibernate
                <a href="${pageContext.request.contextPath}/example/HibernateXML.zip" class="pull-right btn btn-success">Zdrojové kódy</a>
            </h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-primary">
            <div class="panel-heading">Co je Hibernate?</div>
            <div class="panel-body">
                Hibernate je framework napsaný v jazyce Java, který umožňuje tzv. objektově-relační mapování (ORM). Usnadňuje řešení otázky zachování dat objektů i po ukončení běhu aplikace. Je jednou z implementací Java Persistence API (JPA).
            </div>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Ukázka mapování pomocí xml souboru</div>
            <div class="panel-body">
                <div class="panel-body">
                    <p class="custom-text-block">
                        Vytvoření tabulky v MySQL.
                    </p>
                </div>
                <pre class="custom-code"><code class="sql">
CREATE TABLE IF NOT EXISTS `BOOK` (
  `isbn` int(13) NOT NULL auto_increment,
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `author` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `genre` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `edition` int(11) NOT NULL,
  PRIMARY KEY (`isbn`)
);

</code></pre>

                <div class="panel-body">
                    <p class="custom-text-block">
                        Vzor souboru pom.xml.
                    </p>
                </div>
                <pre class="custom-code"><code class="xml">

    &lt;build&gt;
        &lt;plugins&gt;
            &lt;plugin&gt;
                &lt;artifactId&gt;maven-compiler-plugin&lt;/artifactId&gt;
                &lt;version&gt;2.3.2&lt;/version&gt;
                &lt;configuration&gt;
                    &lt;source&gt;1.7&lt;/source&gt;
                    &lt;target&gt;1.7&lt;/target&gt;
                &lt;/configuration&gt;
            &lt;/plugin&gt;
            &lt;plugin&gt;
                &lt;groupId&gt;org.codehaus.mojo&lt;/groupId&gt;
                &lt;artifactId&gt;exec-maven-plugin&lt;/artifactId&gt;
                &lt;version&gt;1.6.0&lt;/version&gt;
                &lt;configuration&gt;
                    &lt;executable&gt;${java.home}/bin/java&lt;/executable&gt;
                    &lt;mainClass&gt;cz.vutbr.fit.gja.App&lt;/mainClass&gt;
                &lt;/configuration&gt;
            &lt;/plugin&gt;
        &lt;/plugins&gt;
    &lt;/build&gt;

    &lt;dependencies&gt;
        &lt;dependency&gt;
            &lt;groupId&gt;org.hibernate&lt;/groupId&gt;
            &lt;artifactId&gt;hibernate-core&lt;/artifactId&gt;
            &lt;version&gt;5.2.12.Final&lt;/version&gt;
        &lt;/dependency&gt;
        &lt;dependency&gt;
            &lt;groupId&gt;org.hibernate&lt;/groupId&gt;
            &lt;artifactId&gt;hibernate-entitymanager&lt;/artifactId&gt;
            &lt;version&gt;5.2.12.Final&lt;/version&gt;
        &lt;/dependency&gt;
        &lt;dependency&gt;
            &lt;groupId&gt;mysql&lt;/groupId&gt;
            &lt;artifactId&gt;mysql-connector-java&lt;/artifactId&gt;
            &lt;version&gt;5.1.30&lt;/version&gt;
        &lt;/dependency&gt;
    &lt;/dependencies&gt;

</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Dále je třeba vytvořit POJO třídu, která bude odpovídat námi vytvořené tabulce.
                    </p>
                </div>
                <pre class="custom-code"><code class="java">

public class Book {

    private int isbn;
    private String title;
    private String author;
    private String genre;
    private int edition;

    public Book(){
    }


    public Book(int isbn, String title, String author, String genre, int edition) {
        this.isbn = isbn;
        this.title = title;
        this.author = author;
        this.genre = genre;
        this.edition = edition;
    }

    public int getIsbn() {
        return isbn;
    }

    public void setIsbn(int isbn) {
        this.isbn = isbn;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getGenre() {
        return genre;
    }

    public void setGenre(String genre) {
        this.genre = genre;
    }

    public int getEdition() {
        return edition;
    }

    public void setEdition(int edition) {
        this.edition = edition;
    }
}

</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                       Tuto třídu je pak třeba namapovat v xml souboru "Book.hbm.xml", který se nechází ve složce "resources".
                    </p>
                </div>
                <pre class="custom-code"><code class="xml">
&lt;?xml version="1.0" encoding="utf-8"?&gt;
                    &lt;!DOCTYPE hibernate-mapping PUBLIC 
 "-//Hibernate/Hibernate Mapping DTD//EN"
 "http://www.hibernate.org/dtd/hibernate-mapping-3.0.dtd"&gt; 

&lt;hibernate-mapping&gt;
  &lt;class name="cz.vutbr.fit.gja.Book" table="BOOK"&gt;
    &lt;meta attribute="class-description"&gt;
      This class contains the book detail.
      &lt;/meta&gt;
      &lt;id name="isbn" type="int" column="isbn"&gt;
      &lt;generator class="native"/&gt;
    &lt;/id&gt;
    &lt;property name="author" column="author" type="string"/&gt;
    &lt;property name="genre" column="genre" type="string"/&gt;
    &lt;property name="edition" column="edition" type="int"/&gt;
    &lt;property name="title" column="title" type="string"/&gt;
  &lt;/class&gt;
&lt;/hibernate-mapping&gt;
</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Zbývá jenom vytvoření SessionFactory.
                    </p>
                </div>
                <pre class="custom-code"><code class="java">
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;

public class HibernateUtil {

    private static final SessionFactory sessionFactory = buildSessionFactory();

    private static SessionFactory buildSessionFactory() {
        try {
            // Create the SessionFactory from hibernate.cfg.xml
            return new Configuration().configure().buildSessionFactory();
        } catch (Throwable ex) {
            // Make sure you log the exception, as it might be swallowed
            System.err.println("Initial SessionFactory creation failed." + ex);
            throw new ExceptionInInitializerError(ex);
        }
    }

    public static SessionFactory getSessionFactory() {
        return sessionFactory;
    }

    public static void shutdown() {
        // Close caches and connection pools
        getSessionFactory().close();
    }

}
</code></pre>

                <div class="panel-body">
                    <p class="custom-text-block">
                        A je možné zapisovat do databáze.
                    </p>
                </div>
                <pre class="custom-code"><code class="java">
import java.util.Iterator;
import java.util.List;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.hibernate.query.Query;

public class App {
    public static void main(String[] args) {

        Session session = HibernateUtil.getSessionFactory().openSession();
        session.beginTransaction();

        String stringQuery = "DELETE FROM Book";
        Query query = session.createQuery(stringQuery);
        query.executeUpdate();

        Book book1 = new Book(123, "A House for Mr. Biswas", "V. S. Naipaul" , "novel",1);
        Book book2 = new Book(1245, "A Tale for the Time Being", "Ruth Ozeki" , "comedy",4);
        Book book3 = new Book(13454654, "The Wisdom of No Escape", "Pema Chödrön " , "drana",2);

        session.save(book1);
        session.save(book2);
        session.save(book3);
        session.getTransaction().commit();

        Transaction tx = null;

        try {
            tx = session.beginTransaction();
            List books = session.createQuery("FROM Book").list();
            for (Iterator iterator = books.iterator(); iterator.hasNext();) {
                Book book = (Book) iterator.next();
                System.out.print("Title: " + book.getTitle());
                System.out.print("  Author: " + book.getAuthor());
                System.out.print("  Genre: " + book.getGenre());
                System.out.println("  Edition: " + book.getEdition());
            }
            tx.commit();
        } catch (HibernateException e) {
            if (tx != null) {
                tx.rollback();
            }
            e.printStackTrace();
        }

        session.close();

    }
}
</code></pre>

            </div>
        </div>
    </div>

    <jsp:include page="../components/footer.jsp"/>
</body>
</html>
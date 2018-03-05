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
            <li class="active">Hibernate a mapování pomocí anotací</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">Hibernate
                <a href="${pageContext.request.contextPath}/example/HibernateAnnotationss.zip" class="pull-right btn btn-success">Zdrojové kódy</a>
            </h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">Ukázka mapování pomocí anotací</div>
            <div class="panel-body">
                <div class="panel-body">
                    <p class="custom-text-block">
                        Při mapování pomocí anotací pracujeme se stejnými soubory jako v ukázce s XML.
                        Rozdílem je, že mapování probíhá přímo v dané třídě. Tyto anotace mohou být, jak u atributu, tak u setteru.
                    </p>
                </div>
                <pre class="custom-code"><code class="java">
import java.io.Serializable;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "Book")
public class Book implements Serializable {

    @Id
    @GeneratedValue(strategy=GenerationType.AUTO)
    @Column(name = "isbn")
    private int isbn;
    @Column(name = "title")
    private String title;
    @Column(name = "author")
    private String author;
    @Column(name = "genre")
    private String genre;
    @Column(name = "edition")
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
                        Drouhou úpravou je pak soubor hibernate.cfg.xml, kde musíme zapsat danou třídu.
                    </p>
                </div>
                <pre class="custom-code"><code class="xml">
                    ...

&lt;mapping class="cz.vutbr.fit.gja.Book" /&gt;

                    ...
                </code></pre>

            </div>
        </div>
    </div>

    <jsp:include page="../components/footer.jsp"/>
</body>
</html>
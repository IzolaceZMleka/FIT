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
            <li>JAX-RS JAXB</li>
            <li class="active">JAX-RS JAXB</li>
        </ol>
    </div>
    <div class="row">
        <div class="col-lg-12">
            <h1 class="page-header">JAX-RS JAXB</h1>
        </div>
    </div>
    <div class="row no-margin">
        <div class="panel panel-info">
            <div class="panel-heading">JAXB a XML</div>
            <div class="panel-body">
                <p class="custom-text-block">
                    Ukázka JAXB a RESTEasy při práci s XML.
                </p>
            </div>
            <div class="panel-body">
                <h5>Přidání závislostí do pom.xml</h5>
                <pre class="custom-code"><code class="xml">
&lt;dependency&gt;
	&lt;groupId&gt;org.jboss.resteasy&lt;/groupId&gt;
    &lt;artifactId&gt;resteasy-jaxrs&lt;/artifactId&gt;
    &lt;version&gt;2.2.1.GA&lt;/version&gt;
&lt;/dependency&gt;

&lt;dependency&gt;
	&lt;groupId&gt;org.jboss.resteasy&lt;/groupId&gt;
    &lt;artifactId&gt;resteasy-jaxb-provider&lt;/artifactId&gt;
    &lt;version&gt;2.2.0.GA&lt;/version&gt;
&lt;/dependency&gt;

</code></pre>
                <div class="panel-body">
                    <p class="custom-text-block">
                        Vytvoření providera, který využívá JAXB anotace pro podpory při konverzi na XML.
                    </p>
                </div>
                <h4>JAXB XML Provider</h4>
                <pre class="custom-code"><code class="java">
package fit.gja.examples.jax;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "book")
public class Book {

    @XmlElement
    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    @XmlElement
    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    @XmlAttribute
    public int getISBN() {
        return ISBN;
    }

    public void setISBN(int ISBN) {
        this.ISBN = ISBN;
    }

    String title;
    String author;
    int ISBN;

}
</code></pre>
                <h4>Služba, která konvertuje objekt Book do xml souboru.</h4>
                <pre class="custom-code"><code class="java">
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;

@Path("/book/xml")
public class BookXMLService {

    @GET
    @Path("/get")
    @Produces("application/xml")
    public Book getBookInXML() {

        Book book = new Book();
        book.setTitle("Saturnin");
        book.setAuthor("Zdenek Jirotka");
        book.setPin(123456789);

        return book;

    }

}
</code></pre>

                <a href="${pageContext.request.contextPath}/jax/examples/book/xml/get" class="btn btn-success">Ukázka XML souboru pro vzorový objekt</a>
            </div>
        </div>
    </div><!-- Odstíňovaní závislostí -->

    <jsp:include page="../components/footer.jsp"/>
</body>
</html>